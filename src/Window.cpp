#include "Window.hpp"
#include <GLFW/glfw3.h>

namespace vino {

void Window::make_current()
{
    glfwMakeContextCurrent(ptrWindow);
}

bool Window::should_close()
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

bool Window::is_pressed(int glfw_key)
{
    return glfwGetKey(ptrWindow, glfw_key);
}

void Window::close()
{
    glfwSetWindowShouldClose(ptrWindow, GLFW_TRUE);
}

uint32_t Window::get_width() const
{
    return _width;
}

uint32_t Window::get_height() const
{
    return _height;
}

} // namespace vino