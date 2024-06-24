#pragma once

#include <string>

struct GLFWmonitor;
struct GLFWwindow;

namespace apbr {

class Window
{
public:
    typedef void (*frameBufferSizeFn)(GLFWwindow *window,
                                      int         width,
                                      int         height);

    Window(int              width,
           int              height,
           const std::string& title,
           GLFWmonitor     *monitor = nullptr,
           GLFWwindow *share        = nullptr);

    Window(const Window &)           = delete;
    Window operator=(const Window &) = delete;

    ~Window() { this->destroy(); }

    int         width() const { return m_width; }

    int         height() const { return m_height; }

    GLFWwindow *handle() const { return m_handle; }

    void        use() const;

    void        destroy();

    void        setFramebufferSizeCallBack(frameBufferSizeFn callback);
    void        swapBuffers();

    void        close();

    bool        is_open() const;

    /// @brief Returns the last reported state of a keyboard key for the specified window.
    // This function returns the last state reported for the specified key to the specified window.
    // The returned state is one of `GLFW_PRESS` or `GLFW_RELEASE`.
    // The action `GLFW_REPEAT` is only reported to the key callback.
    // If the `GLFW_STICKY_KEYS` input mode is enabled, this function returns `GLFW_PRESS` the first time you call it for a key that was pressed, even if that key has already been released.
    // The key functions deal with physical keys, with key tokens named after their use on the standard US keyboard layout.
    // If you want to input text, use the Unicode character callback instead. The modifier key bit masks are not key tokens and cannot be used with this function.
    // __Do not use this function__ to implement text input. taken from: [GLFWdocs](https://www.glfw.org/docs/3.3/group__input.html).
    /// @param key The desired keyboard key. `GLFW_KEY_UNKNOWN` is not a valid key for this function.
    /// @return returns one of `GLFW_PRESS` or `GLFW_RELEASE`
    int         getKeyState(int key) const;

private:
    int         m_width  = 0;
    int         m_height = 0;
    GLFWwindow *m_handle = nullptr;
};

}    // namespace apbr