#include <fstream>
#include <format>
#include <string>

#include "Shader.hpp"
#include "Logger.hpp"

namespace apbr {

auto Shader::to_GL(Shader::Type type)
{
    switch (type) {
        using enum Shader::Type;
    case Vertex:
        return GL_VERTEX_SHADER;
    case Fragment:
        return GL_FRAGMENT_SHADER;
    default:
        logger.logWarn("Unsupported Shader Type");
        return 0;
    }
}

std::string Shader::to_string(Shader::Type type)
{
    switch (type) {
        using enum Shader::Type;
    case Vertex:
        return "VERTEX";
    case Fragment:
        return "FRAGMENT";
    default:
        logger.logWarn("Unsupported Shader Type");
        return "UNKNOWN";
    }
}

Shader::Shader(Shader::Type type)
    : m_type {type},
      m_handle {glCreateShader(to_GL(type))}
{
}

Shader::Shader(Type type, const std::string& source) : Shader {type}
{
    this->compile(source);
}

Shader Shader::from_file(Shader::Type type, const std::string &filepath)
{
    std::ifstream sourceStream(filepath);
    if (!sourceStream) {
        logger.logError(
            std::format("Shader file target `{}` could not be read.",
                        filepath));
    }

    std::string source;
    std::copy(std::istreambuf_iterator<char>(sourceStream),
              std::istreambuf_iterator<char>(),
              std::back_inserter(source));

    return Shader {type, source.c_str()};
}

bool Shader::logCompileStatus() const
{
    int success;
    glGetShaderiv(m_handle, GL_COMPILE_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(m_handle, sizeof(infoLog), nullptr, infoLog);
        // TODO: get the Shader variable name and display it instead of the shader id.
        logger.logError(
            std::format("SHADER::{}::COMPILATION FAILED: id {}\n{}\n",
                        to_string(m_type),
                        m_handle,
                        infoLog));
        glDeleteShader(m_handle);    // don't leak
        return false;
    }

    logger.log(std::format("SHADER::{}::COMPILATION SUCCESS : id {}\n",
                           to_string(m_type),
                           m_handle));
    return true;
}


}    // namespace apbr