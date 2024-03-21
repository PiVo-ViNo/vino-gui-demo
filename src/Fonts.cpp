#include "Fonts.hpp"

#include <glad/glad.h>
#include <filesystem>
#include "freetype/fttypes.h"

namespace vino {

void FreeTypeFace::set_pixel_size(unsigned int pixel_width,
                                  unsigned int pixel_height)
{
    if (FT_Error err =
            FT_Set_Pixel_Sizes(_native_ft_face, pixel_width, pixel_height))
    {
        throw WindowError("ERROR::FREETYPE " + std::to_string(err)
                          + "::Couldn't set pixel size");
    }
}

void FreeTypeFace::load_symbol(unsigned char ch, bool in_cycle)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // Load character glyph in render mode
    if (FT_Error err = FT_Load_Char(_native_ft_face, ch, FT_LOAD_RENDER)) {
        throw WindowError("ERROR::FREETYPE " + std::to_string(err)
                          + "::Failed to load Glyph");
    }
    // generate texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, _native_ft_face->glyph->bitmap.width,
                 _native_ft_face->glyph->bitmap.rows, 0, GL_RED,
                 GL_UNSIGNED_BYTE, _native_ft_face->glyph->bitmap.buffer);
    // set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // now store character for later use
    Character character = {
        texture,
        glm::ivec2(_native_ft_face->glyph->bitmap.width,
                   _native_ft_face->glyph->bitmap.rows),
        glm::ivec2(_native_ft_face->glyph->bitmap_left,
                   _native_ft_face->glyph->bitmap_top),
        static_cast<unsigned int>(_native_ft_face->glyph->advance.x)};
    _chars_map.insert(std::pair<char, Character>(ch, character));
    if (!in_cycle) {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

void FreeTypeFace::load_ascii()
{
    for (unsigned char ch = 0; ch < 128; ch++) {
        load_symbol(ch, true);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

Character& FreeTypeFace::get_char(char ch)
{
    return _chars_map[ch];
}

void Font::render_str(const std::string& str, unsigned int vbo, unsigned int x,
                      unsigned int y, float scale, glm::vec3 color)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (char c : str) {
        Character ch = _face.get_char(c);

        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.size.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        float vertices[6][4] = {
            {xpos, ypos + h, 0.0f, 0.0f},    {xpos, ypos, 0.0f, 1.0f},
            {xpos + w, ypos, 1.0f, 1.0f},

            {xpos, ypos + h, 0.0f, 0.0f},    {xpos + w, ypos, 1.0f, 1.0f},
            {xpos + w, ypos + h, 1.0f, 0.0f}};

        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.texture_id);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch.advance >> 6) * scale;
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

bool FontsCollection::add_font_with_ascii(const std::string& font_path,
                                          unsigned int       size)
{
    std::filesystem::path temp_path = std::filesystem::path(font_path);
    if (!std::filesystem::exists(temp_path) || temp_path.extension() != ".ttf")
    {
        throw WindowError("No file \"" + font_path
                          + "\" or it doesn't have .ttf extension");
    }
    std::string font_name = temp_path.stem().string();

    auto pair_it = _faces.emplace(
        font_name, FreeTypeFace(_ft_lib._native_ft_lib, font_path, size));
    if (!pair_it.second) {
        return false;
    }
    // FreeTypeFace& face = pair_it.first->second;
    // face.load_ascii();
    pair_it.first->second.load_ascii();
    return true;
}

Font FontsCollection::operator[](const std::string& font_name)
{
    auto it = _faces.find(font_name);
    if (it == _faces.end()) {
        throw WindowError("No font with name \"" + font_name + "\" found");
    }
    return Font(it->second);
}

}  // namespace vino