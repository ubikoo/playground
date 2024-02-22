//
// tracer.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef TRACER_H_
#define TRACER_H_

#include <vector>
#include "common.h"
#include "camera.h"
#include "color.h"
#include "film.h"
#include "isect.h"
#include "ray.h"
#include "material.h"
#include "primitive.h"
#include "sampler.h"

struct Tracer {
    Camera mCamera;
    Film mFilm;
    Sampler mSampler;
    size_t mNumSamples;
    std::vector<Primitive> mWorld;

    std::vector<uint8_t> mGLBitmap;
    Graphics::Mesh mGLMesh;
    GLuint mGLTexture;
    GLuint mGLProgram;
    GLuint mGLVao;

    void Initialize();
    void Cleanup();
    void Update();
    void Render();

    Color Radiance(Ray &ray);
};

#endif // TRACER_H_
