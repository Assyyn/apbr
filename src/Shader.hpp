#pragma once

#include <glad/glad.h>

#include <string_view>
#include <string>

#include "Logger.hpp"

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
    Shader(Type type, const char *const source);
    static Shader from_file(Type type, const std::string &filepath);

    ~Shader()
    {
        // don't leak!
        glDeleteShader(m_id);
    }

    /// @brief Convert a `Shader::Type` enum value to its corresponding OpenGL define.
    /// @param type `Shader::Type`. One of `Shader::Type::Vertex` or `Shader::Type::Fragment`
    /// @return OpenGL `#define`d value corresponding to the `Shader::Type`
    static auto        to_GL(Type type);

    static std::string to_string(Type type);

    Type               type() const { return m_type; }

    GLuint             id() const { return m_id; }

    /// @brief Compile the shader manually.
    /// @return Compilation status.
    bool               compile()
    {
        glCompileShader(m_id);
        return logCompileStatus();
    }

    /// @brief Provide a shader source to compile the shader. (For example: when constructed with the `Shader(Type)` overload).
    /// @param source relevant shader source code.
    /// @return Compilation status.
    bool compile(std::string_view source)
    {
        auto source_cstr = source.data();
        glShaderSource(m_id, 1, &source_cstr, nullptr);
        return compile();
    }
private:
    bool logCompileStatus() const;

private:
    Type   m_type;
    GLuint m_id;
};

}    // namespace apbr