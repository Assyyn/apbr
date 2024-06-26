#include <format>
#include <iostream>

#include <apbr/misc.hpp>
#include <internal/config/version.hpp>
#include <apbr/Logger.hpp>
#include <GLFW/glfw3.h>

namespace {

void glfwErrorCallback(int code, const char *description)
{
    logger.logError(std::format("{} | {}", code, description));
}

}    // namespace

void apbr::display_info()
{
    constexpr auto version = apbr::internal::project::version;
    constexpr auto name    = apbr::internal::project::name;

    std::clog << std::format("Welcome to {} version {}\n", name, version);
}

void apbr::initGLFW()
{
    if (!glfwInit()) {
        const char *description = nullptr;
        const int   error       = glfwGetError(&description);
        logger.logFatal(std::format("Failed to initialize GLFW: {} {}",
                                    error,
                                    description));
        std::exit(EXIT_FAILURE);
    }
    glfwSetErrorCallback(glfwErrorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void apbr::terminateGLFW()
{
    glfwSetErrorCallback(nullptr);
    glfwTerminate();
}