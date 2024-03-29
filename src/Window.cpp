#include "Window.hpp"
#include <GLFW/glfw3.h>
#include <cmath>

namespace vino {

void Window::make_current()
{
    glfwMakeContextCurrent(ptrWindow);
}

bool Window::should_close() const
{
    return glfwWindowShouldClose(ptrWindow);
}

void Window::swap_buffers()
{
    glfwSwapBuffers(ptrWindow);
}

int Window::get_attribute(int glfw_attribute)
{
    return glfwGetWindowAttrib(ptrWindow, glfw_attribute);
}

bool Window::is_pressed(int glfw_key) const
{
    return glfwGetKey(ptrWindow, glfw_key);
}

bool Window::is_clicked() const
{
    return glfwGetMouseButton(ptrWindow, GLFW_MOUSE_BUTTON_LEFT);
}

void Window::close()
{
    glfwSetWindowShouldClose(ptrWindow, GLFW_TRUE);
}

std::pair<int, int> Window::get_cursor_pos() const
{
    double xpos = 0;
    double ypos = 0;
    glfwGetCursorPos(ptrWindow, &xpos, &ypos);
    return {std::floor(xpos), 600 - std::floor(ypos)};
}

uint32_t Window::get_width() const
{
    return _width;
}

uint32_t Window::get_height() const
{
    return _height;
}

}  // namespace vino