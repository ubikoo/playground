//
// raymarch.h
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#ifndef RAYMARCH_H_
#define RAYMARCH_H_

struct Sphere {
    cl_float4 centre;
    cl_float radius;
};

struct Raymarch {
    Sphere mSphere;
    Graphics::Mesh mGLMesh;
    GLuint mGLTexture;
    GLuint mGLProgram;
    GLuint mGLVao;

    Compute::Device &mDevice;
    Compute::Program mProgram;
    enum {
        KernelRaymarch = 0,
        NumKernels,
    };
    std::vector<Compute::Kernel> mKernels;
    enum {
        BufferSphere = 0,
        NumBuffers,
    };
    std::vector<Compute::Buffer> mBuffers;
    enum {
        ImageRaymarch = 0,
        NumImages
    };
    std::vector<Compute::Image> mImages;

    Raymarch(Compute::Device &device) : mDevice(device) {}
    ~Raymarch() = default;

    void Initialize();
    void Cleanup();
    void Update();
    void Render();
};

#endif // RAYMARCH_H_
