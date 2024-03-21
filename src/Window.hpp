#pragma once

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdint>
#include <string>

#include "custom_errors.hpp"
// clang-format on

namespace vino {

/**
@brief Base Window class
@warning Don't actually create glfw window
*/
class Window {
public:
    Window(const Window& other) = delete;

    virtual ~Window() { glfwTerminate(); }

    void make_current();
    bool should_close();
    void close();
    void swap_buffers();
    int get_attribute(int glfw_attribute);
    bool is_pressed(int glfw_key);

    [[nodiscard]] uint32_t get_width() const;
    [[nodiscard]] uint32_t get_height() const;

protected:
    Window(uint32_t width, uint32_t height) : _width(width), _height(height)
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    }

    uint32_t    _width{};
    uint32_t    _height{};
    GLFWwindow* ptrWindow = nullptr;
};

class NonResizableWindow : public Window {
public:
    NonResizableWindow(uint32_t width, uint32_t height,
                       const std::string& title) :
        Window(width, height)
    {
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        ptrWindow =
            glfwCreateWindow(_width, _height, title.c_str(), nullptr, nullptr);
        if (ptrWindow == nullptr) {
            glfwTerminate();
            throw vino::WindowError("Window wasn't successfully initialized");
        }
    }

    ~NonResizableWindow() override = default;

private:
};

// class ResizableWindow : public Window {};

}  // namespace vino