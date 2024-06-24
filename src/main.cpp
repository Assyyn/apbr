#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <format>
#include <iostream>
#include <string_view>
#include <cstdlib>
#include <memory>
#include <string>
#include <fstream>

#include <apbr/apbr.hpp>

class App
{
public:
    App(int width, int height, const std::string &title)
        : m_width {width},
          m_height {height},
          m_title {title}
    {
        initGLFW();
        initWindow();
    }

    void run()
    {
        apbr::display_info();

        loadGL();
        glViewport(0, 0, m_width, m_height);
        render();
    }

private:
    void initGLFW()
    {
        if (!glfwInit()) {
            logger.logFatal("Failed to initialize GLFW");
            std::exit(EXIT_FAILURE);
        }
        glfwSetErrorCallback(glfwErrorCallback);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    void initWindow()
    {
        m_window = std::unique_ptr<apbr::Window>(
            new apbr::Window {m_width, m_height, m_title});

        m_window->use();
        m_window->setFramebufferSizeCallBack(
            []([[maybe_unused]] GLFWwindow *m_window,
               int                          width,
               int height) -> void { glViewport(0, 0, width, height); });
    }

    void loadGL()
    {
        if (!gladLoadGLLoader(
                reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            logger.logFatal("Failed to initialize GLAD");
            std::exit(EXIT_FAILURE);
        }
    }

    void render()
    {
        /*----------------------SHADER CREATION-----------------------------------------------*/

        auto vertexShader =
            apbr::Shader::vertexShaderFromFile("shaders/triangle.vert");
        auto fragShader =
            apbr::Shader::fragmentShaderFromFile("shaders/triangle.frag");

        auto shaderProgram = apbr::ShaderProgram();
        shaderProgram.attach(vertexShader);
        shaderProgram.attach(fragShader);
        shaderProgram.link();

        /*----------------------BINDING VERTEX DATA AND VERTEX ATTRIBUTES-----------------------------------------------*/

        // clang-format off
        GLfloat vertices[] = {
            // positions    // colors (RGB)
             0.5, 0.5, 0.0, 1.0f, 0.0f, 0.0f,   // top right (RED) 
            -0.5, 0.5, 0.0, 0.0f, 1.0f, 0.0f,   // top left  (GREEN)
             0.0,-0.5, 0.0, 0.0f, 0.0f, 1.0f    // bottom    (BLUE)
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
        glVertexAttribPointer(
            vertexAttribLocation,
            3,
            GL_FLOAT,
            GL_FALSE,
            // relevant information repeats every six elements:
            6 * sizeof(GLfloat),
            reinterpret_cast<void *>(0));
        glEnableVertexAttribArray(vertexAttribLocation);

        const int vertexColorLocation = 1;
        glVertexAttribPointer(
            vertexColorLocation,
            3,
            GL_FLOAT,
            GL_FALSE,
            // relevant information repeats every six elements:
            6 * sizeof(GLfloat),
            // color information has an offset of 3 from the start:
            reinterpret_cast<void *>(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(vertexColorLocation);

        // unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // render loop
        while (m_window->is_open()) {
            if (m_window->getKeyState(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                m_window->close();
            }

            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(0.4, 0.3, 0.8, 0.9);

            shaderProgram.use();

            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            glfwPollEvents();
            m_window->swapBuffers();
        }
    }

    static void glfwErrorCallback(int code, const char *description)
    {
        logger.logError(std::format("{} | {}", code, description));
    }

private:
    std::unique_ptr<apbr::Window> m_window;
    int                           m_width  = 0;
    int                           m_height = 0;
    std::string                   m_title;
};

int main()
{
    try {
        auto app = App(800, 200, "Triangles - Created with apbr");
        app.run();
        return 0;
    } catch (const std::runtime_error &e) {
        logger.logFatal(e.what());
    } catch (...) {
        logger.logFatal("Exceptional error! 110/100! Go fix your code! :p");
    }

    glfwTerminate();
}