#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <format>
#include <iostream>
#include <string_view>

#include <internal/config/version.hpp>

namespace Log {

namespace Color {
constexpr auto red   = "\033[31m";
constexpr auto reset = "\033[39;49m";
constexpr auto green = "\033[92m";
}    // namespace Color

void errorMessage(const std::string &message)
{
    std::cerr << Log::Color::red << message << Log::Color::reset;
}

void successMessage(const std::string &message)
{
    std::clog << Log::Color::green << message << Log::Color::reset;
}

void programLinkStatus(GLuint program, std::string_view name)
{
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);
        Log::errorMessage(std::format("{}::LINK FAILED:\n{}\n", name, infoLog));
        return;
    }

    Log::successMessage(std::format("{}::LINK SUCCESS\n", name));
}

void shaderCompileStatus(GLuint shader, std::string_view name)
{
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
        Log::errorMessage(
            std::format("{}::COMPILATION FAILED:\n{}\n", name, infoLog));

        return;
    }

    Log::successMessage(std::format("{}::COMPILATION SUCCESS\n", name));
}

}    // namespace Log

int main()
{
    constexpr auto title   = renderer::internal::project::name;
    constexpr auto version = renderer::internal::project::version;

    std::clog << std::format("Welcome to {} version {}\n", title, version);

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW";
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    constexpr int width  = 800;
    constexpr int height = 600;
    GLFWwindow   *window =
        glfwCreateWindow(width, height, title.data(), nullptr, nullptr);

    if (window == nullptr) {
        std::cerr << "Failed to initialize GLFW window";
        return 1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Failed to initialize GLAD";
        return 1;
    }

    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(
        window,
        [](GLFWwindow *window, int width, int height) -> void {
            glViewport(0, 0, width, height);
        });

    // A container for VBOs.
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    // It stores the information about:
    // Vertex attribute configurations via `glVertexAttribPointer`
    // VBOs and their associations with vertex attributes.
    // Calls to enable or disable the vertex attributes array.
    glBindVertexArray(VAO);

    // -1,-1, 0
    //  1, 1, 0
    // 0.5,-0.5, 0
    constexpr float triangle_vertices[] = {-1, -1, 0, 1, 1, 0, 0.5, -0.5, 0};

    GLuint          VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(triangle_vertices),
                 triangle_vertices,
                 GL_STATIC_DRAW);

    // arguments:
    // location of the vector that will hold the vertex attributes,
    // type of vector (vec1/2/3/4),
    // data type, normalized?,
    // stride (space between consecutive vertex attributes),
    // offset
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(float) * 3,
                          reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(0);

    const char *const vertexShaderSource =
        R"(
           #version 330 core
           layout(location = 0) in vec3 pos;

           void main() {
               gl_Position = vec4(pos.x,pos.y,pos.z,1.0);
           } 
        )";

    auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    Log::shaderCompileStatus(vertexShader, "TRIANGLE::SHADER::VERTEX");

    const char *const fragmentShaderSource =
        R"(
            #version 330 core
            out vec4 FragColor;

            void main() {
                FragColor = vec4(1.0,1.0,0.5,1.0);
            }
        )";
    auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    Log::shaderCompileStatus(fragmentShader, "TRIANGLE::SHADER::FRAGMENT");

    auto shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    Log::programLinkStatus(shaderProgram, "shaderProgram");
    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}