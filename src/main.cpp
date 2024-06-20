#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <format>
#include <iostream>
#include <string_view>

#include <internal/config/version.hpp>

int main()
{
    constexpr auto title   = renderer::internal::project::name;
    constexpr auto version = renderer::internal::project::version;

    std::clog << std::format("Welcome to {}, version {}", title, version);

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW";
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    constexpr int width  = 300;
    constexpr int height = width;
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

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    return 0;
}