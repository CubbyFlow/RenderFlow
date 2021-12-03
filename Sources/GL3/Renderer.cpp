#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL3/Application.hpp>
#include <GL3/Camera.hpp>
#include <GL3/PostProcessing.hpp>
#include <GL3/Renderer.hpp>
#include <GL3/Window.hpp>
#include <array>

static const std::array<float, 4> kClearColor = { 0.81f, 0.81f, 0.81f, 1.0f };

namespace GL3
{
Renderer::Renderer() : _queryID(0), _bMeasureGPUTime(false)
{
    //! Do nothing
}

bool Renderer::Initialize(const cxxopts::ParseResult& configure)
{
    //! Create window shared_ptr with default constructor
    _mainWindow = std::make_shared<Window>();
    //! Initialize the window and check the returned error.
    if (!_mainWindow->Initialize(configure["title"].as<std::string>(),
                                 configure["width"].as<int>(),
                                 configure["height"].as<int>()))
    {
        return false;
    }

    //! Pass the Renderer input handling method to window.
    using namespace std::placeholders;
    std::function<void(unsigned int)> inputCallback = [this](unsigned int key) {
        this->ProcessInput(key);
    };
    std::function<void(double, double)> cursorCallback = [this](double x, double y) {
        this->ProcessCursorPos(x, y);
    };
    std::function<void(int, int)> resizeCallback = [this](int width, int height) {
        this->ProcessResize(width, height);
    };
    _mainWindow->operator+=(inputCallback);
    _mainWindow->operator+=(cursorCallback);
    _mainWindow->operator+=(resizeCallback);

    _postProcessing = std::make_unique<PostProcessing>();
    if (!_postProcessing->Initialize())
    {
        return false;
    }
    _postProcessing->Resize(_mainWindow->GetWindowExtent());

    //! Initialize implementation parts
    return OnInitialize(configure);
}

bool Renderer::AddApplication(const std::shared_ptr<Application>& app,
                              const cxxopts::ParseResult& configure)
{
    //! If this is first application, register it to current application
    //! weak_ptr
    if (_applications.empty())
    {
        _currentApp = app;
    }

    //! Push new application to the list.
    _applications.push_back(app);

    //! Initialize the application and return it's result.
    return app->Initialize(_mainWindow, configure);
}

void Renderer::UpdateFrame(double dt)
{
    auto scope = _debug.ScopeLabel("Start Renderer Update");
    //! Do Input handling first
    _mainWindow->ProcessInput();

    //! Get current application and it must be valid pointer
    const auto& app = GetCurrentApplication();
    assert(app);

    //! Update the current application
    app->Update(dt);

    //! Update the rendeeer implementation part
    OnUpdateFrame(dt);
}

void Renderer::DrawFrame()
{
    //! Get current application and it must be valid pointer
    const std::shared_ptr<GL3::Application>& app = GetCurrentApplication();
    assert(app);
    const glm::ivec2 windowExtent = _mainWindow->GetWindowExtent();

    //! If measure GPU performance is enabled.
    //! Below draw calls will not be rendered to screen because of
    //! GL_RASTERIZER_DISCARD.
    if (_bMeasureGPUTime)
    {
        auto scope = _debug.ScopeLabel("Start Performance Measure");
        glViewport(0, 0, windowExtent.x, windowExtent.y);
        glEnable(GL_RASTERIZER_DISCARD);

        BeginGPUMeasure();

        OnBeginDraw();

        app->Draw();

        size_t elapsed_microsec = EndGPUMeasure() / 1000;
        std::clog << '\r' << "Geometry Processing Measured " << elapsed_microsec
                  << "(microsecond)" << std::flush;

        OnEndDraw();

        glDisable(GL_RASTERIZER_DISCARD);
        return;
    }

    //! Actual rendering part.
    auto scope = _debug.ScopeLabel("Start Rendering");
    glViewport(0, 0, windowExtent.x, windowExtent.y);
    glBindFramebuffer(GL_FRAMEBUFFER, _postProcessing->GetFramebuffer());
    glClearBufferfv(GL_COLOR, 0, kClearColor.data());
    glClearBufferfi(GL_DEPTH_STENCIL, 0, 1.0f, 0);
    OnBeginDraw();
    app->Draw();
    OnEndDraw();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(kClearColor[0], kClearColor[1], kClearColor[2],
                 kClearColor[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    _postProcessing->Render();
}

void Renderer::CleanUp()
{
    for (auto& app : _applications)
    {
        app->CleanUp();
    }
    _applications.clear();
    //! Renderer Implementation CleanUo
    OnCleanUp();
    //! Delete opengl context at last
    //! Because opengl deletion calls must be called before context destructed.
    _sharedWindows.clear();
    _mainWindow.reset();
}

bool Renderer::GetRendererShouldExit() const
{
    return _applications.empty() ||
           glfwWindowShouldClose(_mainWindow->GetGLFWWindow()) != 0;
}

void Renderer::BeginGPUMeasure()
{
    if (_queryID != 0)
    {
        glGenQueries(1, &_queryID);
    }
    glBeginQuery(GL_TIME_ELAPSED, _queryID);
}

size_t Renderer::EndGPUMeasure() const
{
    glEndQuery(GL_TIME_ELAPSED);

    GLint done = 0;
    while (done != 0)
    {
        glGetQueryObjectiv(_queryID, GL_QUERY_RESULT_AVAILABLE, &done);
    }

    GLuint64 elapsed;
    glGetQueryObjectui64v(_queryID, GL_QUERY_RESULT, &elapsed);
    return static_cast<size_t>(elapsed);
}

std::shared_ptr<GL3::Application> Renderer::GetCurrentApplication() const
{
    return _currentApp.expired() ? nullptr : _currentApp.lock();
}

std::shared_ptr<GL3::Window> Renderer::GetWindow() const
{
    return _mainWindow;
}

void Renderer::ProcessInput(unsigned int key)
{
    if (key == GLFW_KEY_ESCAPE)
    {
        glfwSetWindowShouldClose(_mainWindow->GetGLFWWindow(), GLFW_TRUE);
    }
    OnProcessInput(key);

    auto app = GetCurrentApplication();
    assert(app);
    app->ProcessInput(key);
}

void Renderer::ProcessCursorPos(double xpos, double ypos) const
{
    auto app = GetCurrentApplication();
    assert(app);
    app->ProcessCursorPos(xpos, ypos);
}

void Renderer::ProcessResize(int width, int height)
{
    _postProcessing->Resize(glm::ivec2(width, height));
    OnProcessResize(width, height);

    auto app = GetCurrentApplication();
    assert(app);
    app->ProcessResize(width, height);
}

void Renderer::SwitchApplication(const std::shared_ptr<GL3::Application>& app)
{
    _currentApp = app;
}

void Renderer::SwitchApplication(size_t index)
{
    if (0 <= index && index < _applications.size())
    {
        SwitchApplication(_applications[index]);
    }
}
};  // namespace GL3