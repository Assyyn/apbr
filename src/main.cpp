#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

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
        apbr::initGLFW();
        initWindow();
    }

    ~App() { apbr::terminateGLFW(); }

    void run()
    {
        apbr::display_info();

        loadGL();
        glViewport(0, 0, m_width, m_height);
        render();
    }

private:
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
            apbr::Shader::vertexShaderFromFile("shaders/rect.vert");
        auto fragShader =
            apbr::Shader::fragmentShaderFromFile("shaders/rect.frag");

        auto shaderProgram = apbr::ShaderProgram();
        shaderProgram.attach(vertexShader);
        shaderProgram.attach(fragShader);
        shaderProgram.link();

        /*----------------------BINDING VERTEX DATA AND VERTEX ATTRIBUTES-----------------------------------------------*/

        // clang-format off
        GLfloat vertices[] = {
            // positions    // colors (RGB)     // texture coords
             0.5, 0.5, 0.0, 1.0f, 0.0f, 0.0f,  1.0f, 1.0f,  // top right        (RED) 
            -0.5, 0.5, 0.0, 0.0f, 1.0f, 0.0f,  0.0f, 1.0f,  // top left         (GREEN)
            -0.5,-0.5, 0.0, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f,  // bottom left      (BLUE)
             0.5,-0.5, 0.0, 0.0f, 0.0f, 1.0f,  1.0f, 0.0f,  // bottom right     (BLUE)
        };

        GLuint rect_indices[] = {
            0, 1, 3,
            1, 2, 3,
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

        GLuint EBO;
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(rect_indices),
                     rect_indices,
                     GL_STATIC_DRAW);

        const int positionLocation = 0;
        glVertexAttribPointer(
            positionLocation,
            3,
            GL_FLOAT,
            GL_FALSE,
            // relevant information repeats every eight elements:
            8 * sizeof(GLfloat),
            reinterpret_cast<void *>(0));
        glEnableVertexAttribArray(positionLocation);

        const int colorLocation = 1;
        glVertexAttribPointer(
            colorLocation,
            3,
            GL_FLOAT,
            GL_FALSE,
            // relevant information repeats every eight elements:
            8 * sizeof(GLfloat),
            // color information has an offset of 3 from the start:
            reinterpret_cast<void *>(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(colorLocation);

        const int texCoordLocation = 2;
        glVertexAttribPointer(texCoordLocation,
                              2,
                              GL_FLOAT,
                              GL_FALSE,
                              8 * sizeof(GLfloat),
                              reinterpret_cast<void *>(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(texCoordLocation);

        auto const bgTexture =
            load_texture2D("textures/wooden-container.jpg", GL_RGB);
        auto const fgTexture =
            load_texture2D("textures/awesomeface.png", GL_RGBA);

        shaderProgram.use();
        auto bgTexLocation =
            glGetUniformLocation(shaderProgram.handle(), "bgTexture");
        glUniform1i(bgTexLocation, 0);
        auto fgTexLocation =
            glGetUniformLocation(shaderProgram.handle(), "fgTexture");
        glUniform1i(fgTexLocation, 1);

        // render loop
        while (m_window->is_open()) {
            if (m_window->getKeyState(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                m_window->close();
            }

            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(0.4, 0.3, 0.8, 0.9);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, bgTexture);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, fgTexture);

            shaderProgram.use();
            glBindVertexArray(VAO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            m_window->swapBuffers();
            glfwPollEvents();
        }
    }

    GLuint
    load_texture2D(const char *path, GLenum format, bool flip_on_load = true)
    {
        // load texture using stbi_load
        int    width    = 0;
        int    height   = 0;
        int    channels = 0;

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        // set wrapping/filtering options for the bound texture object
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,
                        GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_set_flip_vertically_on_load(flip_on_load);
        auto bg_tex_handle = load_image(width, height, channels, path);

        glTexImage2D(GL_TEXTURE_2D,         // texture target
                     0,                     // mipmap level
                     GL_RGB,                // texture store format
                     width,
                     height,
                     0,                     // legacy: set 0
                     format,                // format of image
                     GL_UNSIGNED_BYTE,      // data type of image
                     bg_tex_handle          // image data
        );
        glGenerateMipmap(GL_TEXTURE_2D);    // generate mipmaps for the texture

        free_image(bg_tex_handle);

        return texture;
    }

    stbi_uc *
    load_image(int &width, int &height, int &channels, const char *path)
    {
        auto tex_handle = stbi_load(path, &width, &height, &channels, 0);

        if (!tex_handle) {
            logger.logError(std::format("Failed to load texture: {}", path));
            return nullptr;
        }

        logger.log(std::format("Image `{}` loaded successfully.", path));
        return tex_handle;
    }

    void free_image(stbi_uc *image) { stbi_image_free(image); }
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
}