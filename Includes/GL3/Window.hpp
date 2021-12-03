#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <functional>
#include <glm/vec2.hpp>
#include <string>
#include <vector>

struct GLFWwindow;

namespace GL3
{
/**
 * @brief Window context and callback managing class
 * This class manage window resources(cross-platform) and handling multiple
 * callback methods. The other classes, by adding callback method to this
 * window class, enable to handle inputs.
 *
 */
class Window
{
 public:
    //! Callback functions
    using KeyCallback = std::function<void(unsigned int)>;
    using CursorPosCallback = std::function<void(double, double)>;
    using ResizeCallback = std::function<void(int, int)>;

    /**
     * @brief Construct a new Window object
     */
    Window();

    /**
     * @brief Construct a new Window object
     *
     * @param title window title to be created
     * @param width application window width
     * @param height  application window height
     */
    Window(const std::string& title, int width, int height);

    /**
     * @brief Destroy the Window object
     */
    ~Window();

    /**
     * @brief Create the GLFWwindow with given arguments
     *
     * @param title window title to be created
     * @param width application window width
     * @param height  application window height
     * @param sharedWindow if sharedWindow is not nullptr(e.g. generated
     * window in advance), create shared context
     * @return true if window creation success
     * @return false if window creation failed
     */
    bool Initialize(const std::string& title, int width, int height,
                    GLFWwindow* sharedWindow = nullptr);

    /**
     * @brief Destroy the created window context
     */
    void CleanUp();

    /**
     * @brief Returns the GLFWwindow pointer
     * @return GLFWwindow* glfw window instance pointer (can be nullptr)
     */
    [[nodiscard]] GLFWwindow* GetGLFWWindow();

    /**
     * @brief Returns the window extent
     * @return glm::ivec2 returns window resolution
     */
    [[nodiscard]] glm::ivec2 GetWindowExtent() const;

    /**
     * @brief process input event handling by glfw
     */
    void ProcessInput() const;

    /**
     * @brief Mouse cursor position callback method.
     * @param xpos cursor position x in screen viewport
     * @param ypos cursor position y in screen viewport
     */
    void ProcessCursorPos(double xpos, double ypos) const;

    /**
     * @brief Screen resize callback method.
     * @param width resized screen resolution x
     * @param height resized screen resolution y
     */
    void ProcessResize(int width, int height);

    /**
     * @brief Add input callback functions
     * @param callback callback method to be called
     */
    void operator+=(const KeyCallback& callback);

    /**
     * @brief Add cursor position callback functions
     * @param callback callback method to be called
     */
    void operator+=(const CursorPosCallback& callback);

    /**
     * @brief Add screen resize callback functions
     * @param callback callback method to be called
     */
    void operator+=(const ResizeCallback& callback);

    /**
     * @brief Returns the window extent aspect ratio.
     * @return float returns aspect ratio(width / height)
     */
    [[nodiscard]] float GetAspectRatio() const;

 private:
    std::vector<KeyCallback> _keyCallbacks;
    std::vector<CursorPosCallback> _cursorPosCallbacks;
    std::vector<ResizeCallback> _resizeCallbacks;
    
    GLFWwindow* _window;
    std::string _windowTitle;
    glm::ivec2 _windowExtent;
};
};  // namespace GL3

#endif  //! end of Window.hpp