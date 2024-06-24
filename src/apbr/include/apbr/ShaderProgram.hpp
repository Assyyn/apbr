#pragma once

#include <glad/glad.h>

#include <string>

#include <apbr/Shader.hpp>

namespace apbr {

class ShaderProgram
{
public:
    ShaderProgram() : m_handle {glCreateProgram()} {}

    ~ShaderProgram() { glDeleteProgram(m_handle); }

    GLuint handle() const { return m_handle; }

    void attach(const Shader &shader) { glAttachShader(m_handle, shader.id()); }

    GLint getUniformLocation(const std::string &name) const
    {
        return glGetUniformLocation(m_handle, name.c_str());
    }

    bool link()
    {
        glLinkProgram(m_handle);
        return logLinkStatus();
    }

    void use() { glUseProgram(m_handle); }

private:
    bool logLinkStatus() const;

private:
    GLuint m_handle;
};

}    // namespace  apbr
