#include <fstream>
#include <format>

#include "Shader.hpp"

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
        logger.log(Log::Level::Warn, "Unsupported Shader Type");
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
        logger.log(Log::Level::Warn, "Unsupported Shader Type");
        return "UNKNOWN";
    }
}

Shader::Shader(Shader::Type type)
    : m_type {type},
      m_id {glCreateShader(to_GL(type))}
{
}

Shader::Shader(Type type, const char *const source) : Shader {type}
{
    this->compile(source);
}

Shader Shader::from_file(Shader::Type type, const std::string &filepath)
{
    std::ifstream sourceStream(filepath);
    if (!sourceStream) {
        logger.log(Log::Level::Error,
                   std::format("Shader file target `{}` could not be read.",
                               filepath));
    }

    std::string source;
    std::copy(std::istreambuf_iterator<char>(sourceStream),
              std::istreambuf_iterator<char>(),
              std::back_inserter(source));

    return Shader {type, source.data()};
}

bool Shader::logCompileStatus() const
{
    int success;
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(m_id, sizeof(infoLog), nullptr, infoLog);
        // TODO: get the Shader variable name and display it instead of the shader id.
        logger.log(Log::Level::Error,
                   std::format("SHADER::{}::COMPILATION FAILED: id {}\n{}\n",
                               to_string(m_type),
                               m_id,
                               infoLog));
        glDeleteShader(m_id);    // don't leak
        return false;
    }

    logger.log(std::format("SHADER::{}::COMPILATION SUCCESS : id {}\n",
                           to_string(m_type),
                           m_id));
    return true;
}


}    // namespace apbr