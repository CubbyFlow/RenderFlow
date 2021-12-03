#ifndef DEBUG_UTILS_HPP
#define DEBUG_UTILS_HPP

#include <GL3/GLTypes.hpp>
#include <string>

namespace GL3
{
/**
 * @brief Collection of useful debugging util functions.
 */
class DebugUtils
{
 public:
#if defined(_WIN32) || defined(__linux__)
    /**
     * @brief Print stack trace from the current code line.
     */
    static void PrintStack();
#else
    /**
     * @brief Print stack trace from the current code line.
     */
    static void PrintStack(){};
#endif
    /**
     * @brief Debug logging for opengl context.
     * This function will be passed into glDebugMessageCallback call
     * @param source
     * @param type
     * @param id
     * @param severity
     * @param length
     * @param message
     * @param userParam
     */
    static void DebugLog(GLenum source, GLenum type, GLuint id, GLenum severity,
                         GLsizei length, const GLchar* message,
                         const GLvoid* userParam);

    /**
     * @brief Enable object & scope labeling for debug output
     * @param enable
     */
    static void EnabelDebugLabel(bool enable);

    /**
     * @brief Construct a new Debug Utils object
     */
    DebugUtils() = default;

    /**
     * @brief Destroy the Debug Utils object
     *
     */
    ~DebugUtils() = default;

    /**
     * @brief set the debug label on the opengl resource
     * @param identifier The namespace from which the name of the object is
     allocated.
     * @param name The name of the object to label.
     * @param label The address of a string containing the label to assign to
     the object.
     */
    static void SetObjectName(GLenum identifier, GLuint name,
                              const std::string& label);

    /**
     * @brief opengl scoped label using constructor & destructor
     */
    struct ScopedLabel
    {
        //! Manually delete defualt constructor
        ScopedLabel() = delete;

        /**
         * @brief Constructor with pushing scoped label
         * @param message The a string containing the message to be sent to the
         * debug output stream.
         */
        ScopedLabel(const std::string& message);

        /**
         * @brief Destructor with popping scoped label
         */
        ~ScopedLabel();
    };

    /**
     * @brief create ScopedLabel struct instance with given label message
     * @param message The a string containing the message to be sent to the
     * debug output stream.
     * @return ScopedLabel created ScopedLabel struct instance
     */
    [[nodiscard]] ScopedLabel ScopeLabel(const std::string& message) const
    {
        return { message };
    }

 private:
    static GLuint _scopeID;
    static bool _labelEnabled;
};

};  // namespace GL3

#endif  //! end of DebugUtils.hpp