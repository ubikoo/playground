//
// raymarch.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <array>
#include <vector>
#include <string>

#include "common.h"
#include "raymarch.h"

///
/// @brief Create the raymarch model.
///
void Raymarch::Initialize()
{
    // Initialize Graphics data.
    {
        // Intialize sphere dimensions.
        mSphere.centre = kSphereCentre;
        mSphere.radius = kSphereRadius;

        // Create a mesh over a rectangle.
        mGLMesh = Graphics::CreatePlane(
            "quad",             // vertex attributes prefix
            2,                  // n1 vertices
            2,                  // n2 vertices
            -1.0,               // xlo
            1.0,                // xhi
            -1.0,               // ylo
            1.0);               // yhi

        // Create the 2d-texture data store.
        Graphics::Texture2dCreateInfo texture_info = {};
        texture_info.width = kFilmWidth;
        texture_info.height = kFilmHeight;
        texture_info.internalformat = GL_RGBA8;
        texture_info.pixelformat = GL_RGBA;
        texture_info.pixeltype = GL_UNSIGNED_BYTE;
        texture_info.pixels = NULL;
        mGLTexture = Graphics::CreateTexture2d(texture_info);

        glBindTexture(GL_TEXTURE_2D, mGLTexture);
        Graphics::SetTextureMipmap(GL_TEXTURE_2D);
        Graphics::SetTextureWrap(GL_TEXTURE_2D, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
        Graphics::SetTextureFilter(GL_TEXTURE_2D, GL_LINEAR, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        // Create the shader program object.
        std::vector<GLuint> shaders{
            Graphics::CreateShaderFromFile(GL_VERTEX_SHADER, "data/raymarch.vert"),
            Graphics::CreateShaderFromFile(GL_FRAGMENT_SHADER, "data/raymarch.frag")};
        mGLProgram = Graphics::CreateProgram(shaders);
        Graphics::DestroyShaders(shaders);
        std::cout << Graphics::GetProgramInfoString(mGLProgram) << "\n";

        // Create the vertex array object.
        mGLVao = Graphics::CreateVertexArray();
        glBindVertexArray(mGLVao);
        glBindBuffer(GL_ARRAY_BUFFER, mGLMesh.vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mGLMesh.ebo);
        SetVertexAttributes(mGLProgram, mGLMesh.attributes);
        glBindVertexArray(0);
    }

    // Initialize Compute data.
    {
        // Create the program object and associated kernels.
        std::string source;
        source.append(Compute::LoadProgramSource("data/base.cl"));
        source.append(Compute::LoadProgramSource("data/raymarch.cl"));
        mProgram = Compute::CreateProgramWithSource(mDevice, source);
        mKernels.resize(NumKernels);
        mKernels[KernelRaymarch] = Compute::CreateKernel(mProgram, "raymarch");
        // std::cout << mProgram.GetSource() << "\n";
        // std::cout << mProgram.GetKernelNames() << "\n";

        // Create memory buffers.
        mBuffers.resize(NumBuffers);
        mBuffers[BufferSphere] = Compute::CreateBuffer(
            mDevice,
            sizeof(Sphere),
            CL_MEM_READ_WRITE);

        // Create engine device image store from OpenGL texture object.
        glBindTexture(GL_TEXTURE_2D, mGLTexture);
        mImages.resize(NumImages);
        mImages[ImageRaymarch] = Compute::CreateFromGLTexture(
            mDevice,
            GL_TEXTURE_2D,
            0,
            mGLTexture,
            CL_MEM_WRITE_ONLY);
        glBindTexture(GL_TEXTURE_2D, 0);

        // Copy sphere data onto the main buffer
        mBuffers[BufferSphere]->Write(&mSphere);
    }
}

///
/// @brief Destroy the raymarch model.
///
void Raymarch::Cleanup()
{
    Graphics::DestroyMesh(mGLMesh);
    Graphics::DestroyTexture(mGLTexture);
    Graphics::DestroyProgram(mGLProgram);
}

///
/// @brief Update the raymarch model.
///
void Raymarch::Update()
{
    // Wait for OpenGL to finish and acquire memory objects.
    std::vector<cl_mem> mem_objects{mImages[ImageRaymarch]->id};
    Compute::AcquireGLObjects(mDevice, mem_objects);

    // Raymarch the sphere onto the acquired texture.
    const float current_time = glfwGetTime();
    mKernels[KernelRaymarch]->SetArg(0, &kFilmWidth);
    mKernels[KernelRaymarch]->SetArg(1, &kFilmHeight);
    mKernels[KernelRaymarch]->SetArg(2, &kFilmDepth);
    mKernels[KernelRaymarch]->SetArg(3, &current_time);
    mKernels[KernelRaymarch]->SetArg(4, &kTmin);
    mKernels[KernelRaymarch]->SetArg(5, &kTmax);
    mKernels[KernelRaymarch]->SetArg(6, &kMaxSteps);
    mKernels[KernelRaymarch]->SetArg(7, &mBuffers[BufferSphere]->id);
    mKernels[KernelRaymarch]->SetArg(8, &mImages[ImageRaymarch]->id);
    mKernels[KernelRaymarch]->SetRanges2d(
        {kFilmWidth, kFilmHeight},
        {kWorkGroupSize, kWorkGroupSize});
    mKernels[KernelRaymarch]->Run();

    // Wait for OpenCL to finish and release memory objects.
    Compute::ReleaseGLObjects(mDevice, mem_objects);
}

///
/// @brief Render the raymarch model.
///
void Raymarch::Render()
{
    // Specify draw state modes.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    // glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Bind the shader program object and draw.
    glUseProgram(mGLProgram);
    glBindVertexArray(mGLVao);
#if 0
    {
        std::array<GLint,4> viewport = {};
        glGetIntegerv(GL_VIEWPORT, &viewport[0]);
        GLfloat width = viewport[2];
        GLfloat height = viewport[3];
        Graphics::SetUniform(mGLProgram, "u_width", GL_FLOAT, &width);
        Graphics::SetUniform(mGLProgram, "u_height", GL_FLOAT, &height);
    }
#endif
    GLenum texunit = 0;
    Graphics::SetUniform(mGLProgram, "u_texsampler", GL_SAMPLER_2D, &texunit);
    Graphics::BindTexture(GL_TEXTURE_2D, GL_TEXTURE0 + texunit, mGLTexture);
    Graphics::RenderMesh(mGLMesh);
    glBindVertexArray(0);
    glUseProgram(0);
}
