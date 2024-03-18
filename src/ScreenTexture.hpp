#pragma once

#include <array>
#include <glad/glad.h>

#include "Shader.hpp"

namespace vino {

inline GLuint genSimpleBuffer()
{
    GLuint vertex_buffer_obj;
    glGenBuffers(1, &vertex_buffer_obj);
    return vertex_buffer_obj;
}

inline GLuint genVertexArray()
{
    GLuint vertex_array_obj;
    glGenVertexArrays(1, &vertex_array_obj);
    return vertex_array_obj;
}

inline GLuint genElementBuffer()
{
    GLuint element_buffer_obj;
    glGenBuffers(1, &element_buffer_obj);
    return element_buffer_obj;
}

inline GLuint genTexture()
{
    GLuint texture;
    glGenTextures(1, &texture);
    return texture;
}

class ScreenTexture {
public:
    ScreenTexture()
    {
        // clang-format off
        std::array vertices = {
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            -1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
            1.0f,  1.0f,  0.0f, 1.0f, 1.0f,
            1.0f,  -1.0f, 0.0f, 1.0f, 0.0f
        };

        std::array<unsigned int, 6> indices = {
            0, 1, 2, 
            2, 3, 0
        };
        // clang-format on

        _vbo = genSimpleBuffer();
        _vao = genVertexArray();
        _ebo = genElementBuffer();

        // loadBufferObject(vbo, vao, ebo, vertices, indices);
        glBindVertexArray(_vao);

        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
                     vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     indices.size() * sizeof(unsigned int), indices.data(),
                     GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                              reinterpret_cast<void*>(0));
        glEnableVertexAttribArray(0);
        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                              reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    ~ScreenTexture()
    {
        glDeleteVertexArrays(1, &_vao);
        glDeleteBuffers(1, &_vbo);
        glDeleteBuffers(1, &_ebo);
    }

    void render(Shader& shader, unsigned int texture,
                glm::vec4 scrColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))
    {
        // render
        glClearColor(scrColor.x, scrColor.y, scrColor.z, scrColor.w);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind Texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        // render container
        shader.use();
        glBindVertexArray(_vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

private:
    unsigned int _vao{};
    unsigned int _vbo{};
    unsigned int _ebo{};
};

}  // namespace vino