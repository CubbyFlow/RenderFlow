#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <cxxopts.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace GL3
{
class Camera;
class Shader;
class Window;

/**
 * @brief Main update part of the program.
 * This class represents main application of the program.
 * The program can generate multiple of Application instance, sharing opengl
 * resources like framebuffer, shader, etc..
 */
class Application
{
 public:
    /**
     * @brief Construct a new Application object
     */
    Application() = default;

    /**
     * @brief Destroy the Application object
     */
    virtual ~Application() = default;

    /**
     * @brief Initialize the Application
     * @param window window instance for activating
     * @param configure CLI arguments for app configuration
     * @return true if app initialization success
     * @return false if app initialization failed
     */
    bool Initialize(std::shared_ptr<GL3::Window> window,
                    const cxxopts::ParseResult& configure);

    /**
     * @brief Add camera instance with Perspective or Orthogonal
     * @param camera r-value smart pointer to camera instance for playing
     */
    void AddCamera(std::shared_ptr<GL3::Camera>&& camera);

    /**
     * @brief Update the application with delta time.
     * @param dt delta time in microseconds
     */
    void Update(double dt);

    /**
     * @brief Draw the one frame of the application.
     */
    void Draw();

    /**
     * @brief Clean up the all resources.
     */
    void CleanUp();

    /**
     * @brief Returns the application title
     * @return const char* application title string literal pointer
     */
    virtual const char* GetAppTitle() const = 0;

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

 protected:
    virtual bool OnInitialize(std::shared_ptr<GL3::Window> window,
                              const cxxopts::ParseResult& configure) = 0;
    virtual void OnCleanUp() = 0;
    virtual void OnUpdate(double dt) = 0;
    virtual void OnDraw() = 0;
    virtual void OnProcessInput(unsigned int key) = 0;
    virtual void OnProcessResize(int width, int height) = 0;

    std::vector<std::shared_ptr<GL3::Camera> > _cameras;
    std::unordered_map<std::string, std::shared_ptr<GL3::Shader> > _shaders;
};
};  // namespace GL3

#endif  //! end of Application.hpp