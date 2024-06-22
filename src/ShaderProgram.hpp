#pragma once

#include <glad/glad.h>
#include "Shader.hpp"

namespace apbr {

class ShaderProgram
{
public:
    ShaderProgram() : m_id {glCreateProgram()} {}

    ShaderProgram(const char *const vertexShaderSource,
                  const char *const fragShaderSource)
        : ShaderProgram()
    {
        attach(Shader(Shader::Type::Vertex, vertexShaderSource));
        attach(Shader(Shader::Type::Fragment, fragShaderSource));
    }

    ~ShaderProgram() { glDeleteProgram(m_id); }

    void attach(const Shader &shader) { glAttachShader(m_id, shader.id()); }

    bool link()
    {
        glLinkProgram(m_id);
        return logLinkStatus();
    }

    void use() { glUseProgram(m_id); }

private:
    bool logLinkStatus();

private:
    GLuint m_id;
};

}    // namespace  apbr
