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
#include "ShaderProgram.hpp"
#include "Window.hpp"

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

void glfwErrorCallback(int code, const char *description)
{
    logger.log(Log::Level::Error, std::format("{} | {}", code, description));
}

int main()
{
    try {
        /*----------------------PROJECT SETUP CODE-----------------------------------------------*/
        constexpr auto title   = apbr::internal::project::name;
        constexpr auto version = apbr::internal::project::version;

        std::clog << std::format("Welcome to {} version {}\n", title, version);

        if (!glfwInit()) {
            logger.log(Log::Level::Fatal, "Failed to initialize GLFW");
            std::exit(EXIT_FAILURE);
        }
        glfwSetErrorCallback(glfwErrorCallback);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        constexpr int width  = 800;
        constexpr int height = 600;
        apbr::Window  window {width, height, title};

        window.use();
        window.setFramebufferSizeCallBack(
            []([[maybe_unused]] GLFWwindow *window,
               int                          width,
               int height) -> void { glViewport(0, 0, width, height); });


        if (!gladLoadGLLoader(
                reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            logger.log(Log::Level::Fatal, "Failed to initialize GLAD");
            std::exit(EXIT_FAILURE);
        }

        glViewport(0, 0, width, height);

        /*----------------------SHADER CREATION-----------------------------------------------*/
        auto vertexShader  = apbr::Shader::vertexShader(vertexShaderSource);
        auto fragShader    = apbr::Shader::fragmentShader(fragmentShaderSource);

        auto shaderProgram = apbr::ShaderProgram();
        shaderProgram.attach(vertexShader);
        shaderProgram.attach(fragShader);
        shaderProgram.link();

        /*----------------------BINDING VERTEX DATA AND VERTEX ATTRIBUTES-----------------------------------------------*/
        // clang-format off
        float vertices[] = {
            0.0, 1.0, 0.0,
            0.0, 0.0, 0.0,
            -1.0,-1.0, 0.0
        };
        // clang-format on

        GLuint VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        GLuint VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(vertices),
                     vertices,
                     GL_STATIC_DRAW);

        const int vertexAttribLocation = 0;
        glVertexAttribPointer(vertexAttribLocation,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              3 * sizeof(GLfloat),
                              reinterpret_cast<void *>(0));
        glEnableVertexAttribArray(vertexAttribLocation);

        // unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        while (window.is_open()) {
            if (window.getKeyState(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                window.close();
            }

            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(0.4, 0.3, 0.8, 0.9);

            shaderProgram.use();
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            glfwPollEvents();
            window.swapBuffers();
        }

        glfwTerminate();
        return 0;
    } catch (const std::runtime_error &e) {
        logger.log(Log::Level::Fatal, e.what());
    } catch(...) {
        logger.log("Exceptional error! 110/100! Go fix your code! :p");
    }
}