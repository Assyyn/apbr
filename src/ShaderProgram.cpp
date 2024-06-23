#include <glad/glad.h>

#include <format>

#include "ShaderProgram.hpp"

namespace apbr {

bool ShaderProgram::logLinkStatus() const
{
    int success;
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(m_id, sizeof(infoLog), nullptr, infoLog);
        logger.logError(
            std::format("Program::LINK FAILED: id {}\n{}\n", m_id, infoLog));
        return false;
    }

    logger.log(std::format("Program::LINK SUCCESS : id {}\n", m_id));
    return true;
}

}    // namespace apbr
