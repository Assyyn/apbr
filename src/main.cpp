#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <format>
#include <iostream>
#include <string_view>
#include <cstdlib>

#include <internal/config/version.hpp>
#include "Logger.hpp"
#include "Colors.hpp"
#include "Shader.hpp"

namespace Log {

void programLinkStatus(GLuint program, std::string_view name)
{
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);
        logger.log(Log::Level::Warn,
                   std::format("{}::LINK FAILED:\n{}\n", name, infoLog));
        return;
    }

    logger.log(std::format("{}::LINK SUCCESS\n", name));
}

}    // namespace Log

const char *const vertexShaderSource =
    R"(
           #version 330 core
           layout(location = 0) in vec3 pos;

           void main() {
               gl_Position = vec4(pos.x,pos.y,pos.z,1.0);
           } 
        )";


const char *const fragmentShaderSource =
    R"(
            #version 330 core
            out vec4 FragColor;

            void main() {
                FragColor = vec4(1.0,1.0,0.5,1.0);
            }
        )";

int main()
{
    /*----------------------PROJECT SETUP CODE-----------------------------------------------*/
    constexpr auto title   = renderer::internal::project::name;
    constexpr auto version = renderer::internal::project::version;

    std::clog << std::format("Welcome to {} version {}\n", title, version);

    if (!glfwInit()) {
        logger.log(Log::Level::Fatal, "Failed to initialize GLFW");
        std::exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    constexpr int width  = 800;
    constexpr int height = 600;
    GLFWwindow   *window =
        glfwCreateWindow(width, height, title.data(), nullptr, nullptr);

    if (window == nullptr) {
        logger.log(Log::Level::Fatal, "Failed to initialize GLFW window");
        std::exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        logger.log(Log::Level::Fatal, "Failed to initialize GLAD");
        std::exit(EXIT_FAILURE);
    }

    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(
        window,
        [](GLFWwindow *window, int width, int height) -> void {
            glViewport(0, 0, width, height);
        });

    /*----------------------SHADER CREATION-----------------------------------------------*/
    auto vertexShader =
        apbr::Shader(apbr::Shader::Type::Vertex, vertexShaderSource);
    apbr::Shader fragShader {apbr::Shader::Type::Fragment,
                             fragmentShaderSource};
    
    // works.
    auto testFrag = apbr::Shader::from_file(apbr::Shader::Type::Fragment,
                                            "shaders/test.frag");


    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.4, 0.3, 0.8, 0.9);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}