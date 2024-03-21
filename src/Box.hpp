#pragma once

#include "Fonts.hpp"
#include "Window.hpp"
#include "Shader.hpp"
#include <glm.hpp>

namespace vino {

enum class Align { left, center, right };

/// Q: Use pattern Decorator in case we will have many children of Box?
class Box {
public:
    Box(unsigned int x, unsigned int y, unsigned int width, unsigned int height,
        Window& parent_window);

    ~Box()
    {
        glDeleteVertexArrays(1, &box_vertex_array);
        glDeleteBuffers(1, &box_vertex_buffer);
        glDeleteBuffers(1, &box_element_buffer);
        glDeleteVertexArrays(1, &text_vertex_array);
        glDeleteBuffers(1, &text_vertex_buffer);
    }

    void render_box();

    /// TODO: add enum Align
    void render_str(const std::string& str, Font& font, glm::vec3 color);

protected:
    Window&                   _win;
    glm::vec<2, unsigned int> _ll_pos;
    unsigned int              _width;
    unsigned int              _height;
    Shader                    box_shader;
    Shader                    text_shader;
    unsigned int              box_vertex_buffer{};
    unsigned int              box_vertex_array{};
    unsigned int              box_element_buffer{};
    unsigned int              text_vertex_buffer{};
    unsigned int              text_vertex_array{};
    /// TODO: Add classes TextureBox, ColorBox
    // unsigned int texture_id{};
};

}  // namespace vino