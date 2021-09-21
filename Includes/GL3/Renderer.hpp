#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <GL3/DebugUtils.hpp>
#include <GL3/GLTypes.hpp>
#include <cxxopts.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace GL3
{
class Application;
class Window;
class PostProcessing;

/**
 * @brief OpenGL Renderer managing whole resources
 * @details nable to have multiple applciation and multiple context with one
 * main shared context. This class provides render & update routine and
 * profiling GPU time features. Pass whole input callbacks such as mouse,
 * keyboard into GLFWwindow callback function collection.
 */
class Renderer
{
 public:
    /**
     * @brief Construct a new Renderer object
     */
    Renderer();

    /**
     * @brief Destroy the Renderer object
     */
    virtual ~Renderer();

    /**
     * @brief Initialize the Renderer
     * @param configure CLI arguments for renderer configuration
     * @return true if renderer initialization success
     * @return false if renderer initialization failed
     */
    bool Initialize(const cxxopts::ParseResult& configure);

    /**
     * @brief Add application implementation
     * @param app app instance which is derived by Application class
     * for playing
     * @param configure CLI arguments for app configuration
     * @return true if app instance initialization success
     * @return false if app instance initialization failed
     */
    bool AddApplication(std::shared_ptr<Application> app,
                        const cxxopts::ParseResult& configure);

    /**
     * @brief Update the application with delta time.
     * @param dt delta time in microseconds
     */
    void UpdateFrame(double dt);

    /**
     * @brief Draw the one frame of the application.
     */
    void DrawFrame();

    /**
     * @brief Clean up the all resources.
     */
    void CleanUp();

    /**
     * @brief Returns the current bound application
     * @return std::shared_ptr<GL3::Application> smart
     * pointer to activated application instance
     */
    [[nodiscard]] std::shared_ptr<GL3::Application> GetCurrentApplication() const;

    /**
     * @brief Returns the current application's window
     * @return std::shared_ptr<GL3::Window> smart
     * pointer to activated window instance
     */
    [[nodiscard]] std::shared_ptr<GL3::Window> GetWindow() const;

    /**
     * @brief Returns whether this renderer should exit or not
     * through glfw3
     * @return true if window should closed
     * @return false if window should not closed
     */
    [[nodiscard]] bool GetRendererShouldExit() const;

    /**
     * @brief Switch the current app to the next given application
     * @param app smart pointer to application instance
     */
    void SwitchApplication(std::shared_ptr<GL3::Application> app);

    /**
     * @brief Switch the current app to the next given application
     * @param app index of application instance in the vector
     */
    void SwitchApplication(size_t index);

 protected:
    virtual bool OnInitialize(const cxxopts::ParseResult& configure) = 0;
    virtual void OnCleanUp() = 0;
    virtual void OnUpdateFrame(double dt) = 0;
    virtual void OnBeginDraw() = 0;
    virtual void OnEndDraw() = 0;
    virtual void OnProcessInput(unsigned int key) = 0;
    virtual void OnProcessResize(int width, int height) = 0;

    /**
     * @brief Begin of GPU Time measurement
     */
    void BeginGPUMeasure();

    /**
     * @brief End of GPU Time measurement and returns elapsed time
     * @return size_t returns elapsed time in milliseconds
     */
    size_t EndGPUMeasure();

    std::weak_ptr<GL3::Application> _currentApp;
    std::vector<std::shared_ptr<GL3::Application> > _applications;
    std::shared_ptr<GL3::Window> _mainWindow;
    std::vector<std::shared_ptr<GL3::Window> > _sharedWindows;
    std::unique_ptr<PostProcessing> _postProcessing;

 private:
    /**
     * @brief Process the input key
     * @param key input key code defined in glfw3
     */
    void ProcessInput(unsigned int key);

    /**
     * @brief Process the mouse cursor positions
     * @param xpos cursor position x in the screen viewport
     * @param ypos cursor position y in the screen viewport
     */
    void ProcessCursorPos(double xpos, double ypos);

    /**
     * @brief Resize the renderer resources
     * @param width resized framebuffer width
     * @param height resized framebuffer height
     */
    void ProcessResize(int width, int height);

    DebugUtils _debug;
    GLuint _queryID;
    bool _bMeasureGPUTime;
};
};  // namespace GL3

#endif  //! end of Renderer.hpp