#ifndef SHADER_HPP
#define SHADER_HPP

#include <GL3/GLTypes.hpp>
#include <string>
#include <unordered_map>

namespace GL3
{
/**
 * @brief Program with attached arbitrary shaders
 * @details Enable to have multiple applciation and multiple context with one
 * main shared context. This class provides render & update routine and
 * profiling GPU time features. Pass whole input callbacks such as mouse,
 * keyboard into GLFWwindow callback function collection.
 */
class Shader
{
 public:
    /**
     * @brief Construct a new Shader object
     */
    Shader();

    /**
     * @brief Destroy the Shader object
     */
    ~Shader();

    /**
     * @brief Compile given shader files link the program
     *
     * @param sources pairs of shader type and shader file path collection.
     * @return true if shader compile successful
     * @return false if shader compile failed
     */
    bool Initialize(const std::unordered_map<GLenum, std::string>& sources);

    /**
     * @brief Bind generated shader program.
     */
    void BindShaderProgram() const;

    /**
     * @brief Unbind shader program
     * @details declared as static because nothing related with member variables
     * or method
     */
    static void UnbindShaderProgram();

    /**
     * @brief Bind uniform block to this program
     * @param blockName uniform block identifier name in the shader
     * @param bindingPoint uniform block binding point in the shader
     */
    void BindUniformBlock(const std::string& blockName,
                          GLuint bindingPoint) const;

    /**
     * @brief Bind frag data location (STANDARD)
     * @param name output shader attributes name for fragments
     * @param location binding point of output shader attributes
     */
    void BindFragDataLocation(const std::string& name, GLuint location) const;

    /**
     * @brief Check this shader program has uniform variable with given name
     * @param name uniform variable name in the shader
     * @return true if uniform variable matched with given name exists
     * @return false if uniform variable matched with given name non exists
     */
    [[nodiscard]] bool HasUniformVariable(const std::string& name);

    /**
     * @brief Returns the uniform variable location matched with given name.
     * @param name uniform variable name for querying
     * @return GLint uniform variable location in the shader.
     * Returns -1 if not exists
     */
    [[nodiscard]] GLint GetUniformLocation(const std::string& name);

    /**
     * @brief Send the uniform variable to the pipeline.
     * @tparam Type uniform variable type correspond to type in ours
     * @param name uniform variable name
     * @param val value for send to uniform variable
     */
    template <typename Type>
    void SendUniformVariable(const std::string& name, Type val);

    /**
     * @brief Clean up the generated resources
     */
    void CleanUp();

    /**
     * @brief Returns the program resource ID
     * @return GLuint program resource ID
     */
    [[nodiscard]] GLuint GetResourceID() const;

 private:
    std::unordered_map<std::string, GLint> _uniformCache;
    GLuint _programID;
};

};  // namespace GL3

#endif  //! end of Shader.hpp