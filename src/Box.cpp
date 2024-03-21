#include "Box.hpp"

#include "Shader.hpp"
#include <array>

namespace vino {

void Box::render_box()
{
    box_shader.use();
    glm::mat4 projection =
        glm::ortho(0.0f, static_cast<float>(_win.get_width()), 0.0f,
                   static_cast<float>(_win.get_height()));
    box_shader.setMat4FloatV("uProjection", projection);

    glBindVertexArray(box_vertex_array);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Box::render_str(const std::string& str, Font& font, glm::vec3 color)
{
    // activate corresponding render state
    text_shader.use();
    glm::mat4 projection =
        glm::ortho(0.0f, static_cast<float>(_win.get_width()), 0.0f,
                   static_cast<float>(_win.get_height()));
    text_shader.setMat4FloatV("uProjection", projection);
    text_shader.setVec3Float("uTextColor", color.x, color.y, color.z);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(text_vertex_array);

    // find FreeTypeFace with such font or create one
    font.render_str(str, text_vertex_buffer, _ll_pos.x + 20,
                    (_ll_pos.y + _height) / 2, 1.0f, color);

    glDisable(GL_BLEND);
}

Box::Box(unsigned int x, unsigned int y, unsigned int width,
         unsigned int height, Window& parent_window) :
    _win(parent_window),
    _ll_pos(x, y),
    _width(width),
    _height(height),
    box_shader("../shaders/boxVertex.glsl", "../shaders/colorFrag.glsl"),
    text_shader("../shaders/charVertex.glsl", "../shaders/charFrag.glsl")
{
    std::cout << "Created Box: " << _width << "x" << _height << std::endl;
    std::array<std::array<unsigned int, 3>, 4> corners = {
        {{_ll_pos.x, _ll_pos.y, 0},
         {_ll_pos.x, _ll_pos.y + _height, 0},
         {_ll_pos.x + _width, _ll_pos.y + _height, 0},
         {_ll_pos.x + _width, _ll_pos.y, 0}}};
    std::array elem_indices = {0, 1, 2, 2, 3, 0};

    glGenBuffers(1, &box_vertex_buffer);
    glGenBuffers(1, &text_vertex_buffer);
    glGenVertexArrays(1, &box_vertex_array);
    glGenVertexArrays(1, &text_vertex_array);
    glGenBuffers(1, &box_element_buffer);

    // init buffers for box
    glBindVertexArray(box_vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, box_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(unsigned int) * corners.size() * corners.data()->size(),
                 corners.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, box_element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * elem_indices.size(),
                 elem_indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_UNSIGNED_INT, GL_FALSE,
                          3 * sizeof(unsigned int), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // init buffers for text
    glBindVertexArray(text_vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, text_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

}  // namespace vino