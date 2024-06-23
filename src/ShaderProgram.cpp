#include <glad/glad.h>

#include <format>

#include "ShaderProgram.hpp"
#include "Logger.hpp"

namespace apbr {

bool ShaderProgram::logLinkStatus() const
{
    int success;
    glGetProgramiv(m_handle, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(m_handle, sizeof(infoLog), nullptr, infoLog);
        logger.logError(
            std::format("Program::LINK FAILED: id {}\n{}\n", m_handle, infoLog));
        return false;
    }

    logger.log(std::format("Program::LINK SUCCESS : id {}\n", m_handle));
    return true;
}

}    // namespace apbr
