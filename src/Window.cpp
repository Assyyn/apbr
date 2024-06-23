#include <GLFW/glfw3.h>

#include <stdexcept>
#include <string>

#include "Window.hpp"
#include "Logger.hpp"

namespace apbr {

Window::Window(int                width,
               int                height,
               const std::string &title,
               GLFWmonitor       *monitor,
               GLFWwindow        *share)
    : m_width {width},
      m_height {height},
      m_handle {glfwCreateWindow(width, height, title.c_str(), monitor, share)}
{
    if (!m_handle) {
        throw std::runtime_error("Failed to initialize apbr::Window handle.");
    }
    logger.log("apbr::Window initialized.");
}

void Window::use() const
{
    glfwMakeContextCurrent(m_handle);
}

void Window::destroy()
{
    glfwDestroyWindow(m_handle);
}

void Window::setFramebufferSizeCallBack(frameBufferSizeFn callback)
{
    glfwSetFramebufferSizeCallback(m_handle, callback);
}

void Window::swapBuffers()
{
    glfwSwapBuffers(m_handle);
}

void Window::close()
{
    glfwSetWindowShouldClose(m_handle, GLFW_TRUE);
}

bool Window::is_open() const
{
    return !glfwWindowShouldClose(m_handle);
}

int Window::getKeyState(int key) const
{
    return glfwGetKey(m_handle, key);
}

}    // namespace apbr