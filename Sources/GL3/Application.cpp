#include <glad/glad.h>
#include <GL3/Application.hpp>
#include <GL3/Camera.hpp>
#include <GL3/DebugUtils.hpp>
#include <GL3/PerspectiveCamera.hpp>
#include <GL3/Shader.hpp>
#include <GL3/Window.hpp>
#include <iostream>

namespace GL3
{
bool Application::Initialize(std::shared_ptr<GL3::Window> window,
                             const cxxopts::ParseResult& configure)
{
    return OnInitialize(std::move(window), configure);
}

void Application::AddCamera(std::shared_ptr<Camera>&& camera)
{
    _cameras.emplace_back(std::move(camera));
}

void Application::Update(double dt)
{
    OnUpdate(dt);
}

void Application::Draw()
{
    OnDraw();
}

void Application::CleanUp()
{
    _shaders.clear();
    _cameras.clear();

    OnCleanUp();
}

void Application::ProcessInput(unsigned int key)
{
    for (auto& camera : _cameras)
    {
        camera->ProcessInput(key);
    }

    OnProcessInput(key);
}

void Application::ProcessCursorPos(double xpos, double ypos)
{
    for (auto& camera : _cameras)
    {
        camera->ProcessCursorPos(xpos, ypos);
    }
}

void Application::ProcessResize(int width, int height)
{
    OnProcessResize(width, height);
}
};  // namespace GL3