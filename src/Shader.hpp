#pragma once

#include <glad/glad.h>

#include <string_view>
#include <string>

namespace apbr {

// Shader as in `Shader Object`. For `Shader Program` see `ShaderProgram.hpp`
class Shader
{
public:
    // only these two for now, because only learnt about them.
    enum class Type : int {
        Vertex,
        Fragment,
    };

    Shader(Type type);
    Shader(Type type, const std::string& source);

    ~Shader()
    {
        // don't leak!
        glDeleteShader(m_handle);
    }

    /// @brief Convert a `Shader::Type` enum value to its corresponding OpenGL define.
    /// @param type `Shader::Type`. One of `Shader::Type::Vertex` or `Shader::Type::Fragment`
    /// @return OpenGL `#define`d value corresponding to the `Shader::Type`
    static auto        to_GL(Type type);

    static std::string to_string(Type type);

    Type               type() const { return m_type; }

    GLuint             id() const { return m_handle; }

    /// @brief Compile the shader manually.
    /// @return Compilation status.
    bool               compile()
    {
        glCompileShader(m_handle);
        return logCompileStatus();
    }

    /// @brief Provide a shader source to compile the shader. (For example: when constructed with the `Shader(Type)` overload).
    /// @param source relevant shader source code.
    /// @return Compilation status.
    bool compile(const std::string& source)
    {
        auto source_cstr = source.c_str();
        glShaderSource(m_handle, 1, &source_cstr, nullptr);
        return compile();
    }

public:
    // factory functions

    static Shader from_file(Type type, const std::string &filepath);

    static Shader vertexShader(const char *const source)
    {
        return Shader {Type::Vertex, source};
    }

    static Shader fragmentShader(const char *const source)
    {
        return Shader {Type::Fragment, source};
    }

    static Shader vertexShaderFromFile(const std::string &filepath)
    {
        return Shader::from_file(Type::Vertex, filepath);
    }

    static Shader fragmentShaderFromFile(const std::string &filepath)
    {
        return Shader::from_file(Type::Fragment, filepath);
    }

private:
    bool logCompileStatus() const;

private:
    Type   m_type;
    GLuint m_handle;
};

}    // namespace apbr