//
// main.cpp
//
// Copyright (c) 2020 Carlos Braga
// This program is free software; you can redistribute it and/or modify it
// under the terms of the MIT License. See accompanying LICENSE.md or
// https://opensource.org/licenses/MIT.
//

#include <iostream>
#include <exception>

#include "common.h"
#include "raymarch.h"

///
/// @brief Constants and globals.
///
Compute::Device gDevice;
std::unique_ptr<Raymarch> gRaymarch;

///
/// @brief Graphics callback functions.
///
void Graphics::OnResize(int width, int height)
{
    glViewport(0, 0, width, height);
}

void Graphics::OnKeyboard(int code, int scancode, int action, int mods)
{
    if (code == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
        Graphics::Close();
    }
}

void Graphics::OnMouseMove(double xpos, double ypos)
{}

void Graphics::OnMouseButton(int button, int action, int mods)
{}

void Graphics::OnInitialize()
{
    gDevice = Compute::CreateDeviceFromGLContext(kDeviceIndex);
    std::cout << Compute::ToString(Compute::GetDeviceInfo(gDevice->id)) << "\n";

    gRaymarch = std::make_unique<Raymarch>(gDevice);
    gRaymarch->Initialize();
}

void Graphics::OnTerminate()
{
    gRaymarch->Cleanup();
}

void Graphics::OnUpdate()
{
    static const uint32_t kMaxFrames = 360;
    static uint32_t FrameCount = 0;
    if (++FrameCount >= kMaxFrames) {
        Graphics::Close();
    }
    gRaymarch->Update();
}

void Graphics::OnRender()
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gRaymarch->Render();
}

///
/// @brief main application client.
///
int main(int argc, char const *argv[])
{
    Graphics::RenderDesc desc = {};
    desc.WindowTitle = "raymarchsphere";
    desc.WindowWidth = 1024;
    desc.WindowHeight = 512;
    desc.GLVersionMajor = 3;
    desc.GLVersionMinor = 3;
    desc.PollTimeout =  0.01;

    try {
        Graphics::RenderLoop(desc);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
