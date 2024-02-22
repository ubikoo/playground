//
// tracer.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <vector>
#include <cfloat>
#include "common.h"
#include "tracer.h"

///
/// @brief Create the tracer and associated objects.
///
void Tracer::Initialize()
{
    // Tracer data.
    {
        mCamera = Camera::Create(
            kCameraEye,
            kCameraCtr,
            kCameraUp,
            kCameraFov,
            (double) kFilmWidth / kFilmHeight,
            kCameraFocus,
            kCameraAperture);
        mFilm = Film::Create(kFilmWidth, kFilmHeight);
        mNumSamples = 0;
        mSampler = Sampler::Create(math::make_random(),
            math::random_uniform<double>());
        mWorld = Primitive::Generate(3);
    }

    // OpenGL data.
    {
        // Create bitmap data.
        mGLBitmap.resize(3 * kFilmWidth * kFilmHeight, 0);

        // Create a mesh over a rectangle.
        mGLMesh = Graphics::CreatePlane(
            "quad",                 // vertex attributes prefix name
            2,                      // n1 vertices
            2,                      // n2 vertices
            -1.0,                   // xlo
             1.0,                   // xhi
            -1.0,                   // ylo
             1.0);                  // yhi

        // Create the 2d-texture data store.
        Graphics::Texture2dCreateInfo info = {};
        info.width = kFilmWidth;
        info.height = kFilmHeight;
        info.internalformat = GL_RGBA8;
        info.pixelformat = GL_RGBA;
        info.pixeltype = GL_UNSIGNED_BYTE;
        info.pixels = NULL;
        mGLTexture = Graphics::CreateTexture2d(info);

        glBindTexture(GL_TEXTURE_2D, mGLTexture);
        Graphics::SetTextureMipmap(GL_TEXTURE_2D);
        Graphics::SetTextureWrap(GL_TEXTURE_2D, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
        Graphics::SetTextureFilter(GL_TEXTURE_2D, GL_LINEAR, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        // Create the shader program object.
        std::vector<GLuint> shaders{
            Graphics::CreateShaderFromFile(GL_VERTEX_SHADER, "data/tracer.vert"),
            Graphics::CreateShaderFromFile(GL_FRAGMENT_SHADER, "data/tracer.frag")};
        mGLProgram = Graphics::CreateProgram(shaders);
        std::cout << Graphics::GetProgramInfoString(mGLProgram) << "\n";

        // Create the vertex array object.
        mGLVao = Graphics::CreateVertexArray();
        glBindVertexArray(mGLVao);
        glBindBuffer(GL_ARRAY_BUFFER, mGLMesh.vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mGLMesh.ebo);
        SetVertexAttributes(mGLProgram, mGLMesh.attributes);
        glBindVertexArray(0);
    }
}

///
/// @brief Destroy the tracer and associated objects.
///
void Tracer::Cleanup()
{}

///
/// @brief Update the tracer.
///
void Tracer::Update()
{
    if (mNumSamples++ == kNumSamples) {
        return;
    }

    // For each pixel in the film, generate a camera ray towards a random point
    // inside the pixel square. Compute the radiance along that ray and add it
    // to the pixel.
    for (size_t y = 0; y < kFilmHeight; ++y) {
        for (size_t x = 0; x < kFilmWidth; ++x) {
            math::vec2d u1 = mSampler.Rand2d();
            math::vec2d u2 = mSampler.Rand2d();
            Ray ray = mCamera.rayto(mFilm.sample(x, y, u1), u2);
            mFilm.add(x, y, Radiance(ray));
        }
    }

    // Update the bitmap.
    uint8_t *px = &mGLBitmap[0];
    for (auto color : mFilm.m_pixels) {
        color /= (double) mNumSamples;
        *px++ = static_cast<uint8_t>(255.0 * std::sqrt(color.r));
        *px++ = static_cast<uint8_t>(255.0 * std::sqrt(color.g));
        *px++ = static_cast<uint8_t>(255.0 * std::sqrt(color.b));
    }
}

///
/// @brief Render the tracer.
///
void Tracer::Render()
{
    // Update the drawable state.
    glBindTexture(GL_TEXTURE_2D, mGLTexture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,                      // level of detail - 0 is base bitmap
        GL_RGB8,                // texture internal format
        kFilmWidth,             // texture width
        kFilmHeight,            // texture height
        0,                      // border parameter - must be 0 (legacy)
        GL_RGB,                 // pixel format
        GL_UNSIGNED_BYTE,       // type of the pixel data(GLubyte)
        &mGLBitmap[0]);         // pointer to the pixel data
    glBindTexture(GL_TEXTURE_2D, 0);

    // Specify draw state modes.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    // glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Bind the shader program object.
    glUseProgram(mGLProgram);
    glBindVertexArray(mGLVao);
#if 0
    // Set window dimensions.
    std::array<GLfloat,2> fbsize = {};
    glfw::GetFramebufferSize(fbsize);
    Graphics::SetUniform(mGLProgram, "u_width", GL_FLOAT, &fbsize[0]);
    Graphics::SetUniform(mGLProgram, "u_height", GL_FLOAT, &fbsize[1]);
#endif
    GLenum texunit = 0;
    Graphics::SetUniform(mGLProgram, "u_texsampler", GL_SAMPLER_2D, &texunit);
    Graphics::BindTexture(GL_TEXTURE_2D, GL_TEXTURE0 + texunit, mGLTexture);
    Graphics::RenderMesh(mGLMesh);
    glBindVertexArray(0);
    glUseProgram(0);
}

/// ---------------------------------------------------------------------------
/// @brief Return the radiance along the primary ray using Monte Carlo
/// integration by tracing a path through the world.
///
/// For each step in the path, compute the closest point of intersection of the
/// ray with the world. At the intersection point x, compute the radiance in the
/// outgoing direction wo as a sum of self-emitted and reflected terms:
///
///  L(x->wo) = Le(x->wo) + Lr(x->wo)
///
/// The term Lr(x->wo) represents the radiance that is reflected by the surface
/// at x in the outgoing direction. It has two contributions: radiance directly
/// reflected from light sources and radiance indirectly reflected from other
/// surfaces in the world.
///
Color Tracer::Radiance(Ray &ray)
{
    Color L = Color::Black;         // path radiance
    Color beta = Color::White;      // path attenuation coefficient
    size_t depth = 0;               // path depth
    while (true) {
        // Stop path tracing if we exceed the maximum path depth.
        if (++depth >= kMaxSampleDepth) {
            L = Color::Red;
            break;
        }

        // Compute closest intersection of ray with the world and return the
        // background color if not primitive is intersected.
        Isect isect;
        double t_min = 0.001;
        double t_max = DBL_MAX;
        if (!Primitive::Intersect(mWorld, ray, t_min, t_max, isect)) {
            // Return the background color if no intersection
            double tx = 0.5 * (ray.d.x + 1.0);
            double ty = 0.5 * (ray.d.y + 1.0);
            Color background = Color{1.0, 1.0, 1.0} * (1.0 - tx - ty) +
                               Color{0.7, 0.7, 0.9} * tx +
                               Color{0.7, 0.9, 0.9} * ty;
            L += beta * background;
            break;
        }

        // Compute scattering direction and corresponding bsdf.
        math::vec2d u = mSampler.Rand2d();
        math::vec3d wo = isect.wo;
        math::vec3d wi;
        Color bsdf;
        double pdf;
        if (!Isect::Scatter(isect, u, wo, wi, bsdf, pdf)) {
            break;
        }
        beta *= bsdf * (Isect::AbsDot(isect.n, wi) / pdf);

        // Spawn a ray in the direction oposite the incident direction
        ray = Isect::Spawn(isect, wi);
    }

    return L;
}
