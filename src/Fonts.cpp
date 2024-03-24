#include "Fonts.hpp"

#include <glad/glad.h>
#include <filesystem>
#include <iostream>

namespace vino {

// FreeTypeFace ---------------------------------------------------------------
// ----------------------------------------------------------------------------

template <typename _Ch>
void FreeTypeFace<_Ch>::set_pixel_size(unsigned int pixel_width,
                                       unsigned int pixel_height)
{
    if (FT_Error err =
            FT_Set_Pixel_Sizes(_native_ft_face, pixel_width, pixel_height))
    {
        throw WindowError("ERROR::FREETYPE " + std::to_string(err)
                          + "::Couldn't set pixel size");
    }
}

template <typename _Ch>
Character& FreeTypeFace<_Ch>::load_symbol(_Ch ch, bool in_cycle)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // Load character glyph in render mode
    if (FT_Error err = FT_Load_Char(_native_ft_face, ch, FT_LOAD_RENDER)) {
        throw WindowError("ERROR::FREETYPE " + std::to_string(err)
                          + "::Failed to load Glyph");
    }
    // std::cout << ch << " ";
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
    auto it = _chars_map.insert(std::pair<_Ch, Character>(ch, character)).first;
    if (!in_cycle) {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    return it->second;
}

template <typename _Ch>
void FreeTypeFace<_Ch>::load_ascii()
{
    for (_Ch ch = 0; ch < 127; ch++) {
        load_symbol(ch, true);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

template <typename _Ch>
Character& FreeTypeFace<_Ch>::get_char(const _Ch& ch)
{
    auto it = _chars_map.find(ch);
    if (it == _chars_map.end()) {
        return load_symbol(ch);
    }
    return it->second;
}

// Font -----------------------------------------------------------------------
// ----------------------------------------------------------------------------

template <typename _Ch>
void Font<_Ch>::render_str(const std::basic_string<_Ch>& str, unsigned int vbo,
                           glm::uvec2 ll_pos, float scale) const
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (const _Ch& c : str) {
        Character ch = _face.get_char(c);

        float xpos = ll_pos.x + ch.bearing.x * scale;
        float ypos = ll_pos.y - (ch.size.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        std::array<std::array<float, 4>, 6> vertices = {
            {{xpos, ypos + h, 0.0f, 0.0f},
             {xpos, ypos, 0.0f, 1.0f},
             {xpos + w, ypos, 1.0f, 1.0f},

             {xpos, ypos + h, 0.0f, 0.0f},
             {xpos + w, ypos, 1.0f, 1.0f},
             {xpos + w, ypos + h, 1.0f, 0.0f}}};

        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.texture_id);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(
            GL_ARRAY_BUFFER, 0,
            sizeof(float) * vertices.size() * vertices.data()->size(),
            vertices.data());

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);

        ll_pos.x += (ch.advance >> 6) * scale;
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

template <typename _Ch>
std::size_t Font<_Ch>::render_str_inbound(const std::basic_string<_Ch>& str,
                                          unsigned int vbo, glm::uvec2 ll_pos,
                                          float        scale,
                                          unsigned int x_bound) const
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    std::size_t count_chars = 0;

    for (const _Ch& c : str) {
        Character ch = _face.get_char(c);

        float xpos = ll_pos.x + ch.bearing.x * scale;
        float ypos = ll_pos.y - (ch.size.y - ch.bearing.y) * scale;
        if (c == '\n' || ll_pos.x + (ch.advance >> 6) * scale >= x_bound) {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
            return ++count_chars;
        }

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        std::array<std::array<float, 4>, 6> vertices = {
            {{xpos, ypos + h, 0.0f, 0.0f},
             {xpos, ypos, 0.0f, 1.0f},
             {xpos + w, ypos, 1.0f, 1.0f},

             {xpos, ypos + h, 0.0f, 0.0f},
             {xpos + w, ypos, 1.0f, 1.0f},
             {xpos + w, ypos + h, 1.0f, 0.0f}}};

        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.texture_id);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(
            GL_ARRAY_BUFFER, 0,
            sizeof(float) * vertices.size() * vertices.data()->size(),
            vertices.data());

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);

        ll_pos.x += (ch.advance >> 6) * scale;
        count_chars++;
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    return count_chars;
}

template <typename _Ch>
glm::uvec2 Font<_Ch>::get_dimensions_of(const std::string& str,
                                        float              scale) const
{
    glm::uvec2 dimensions{};

    for (const char& c : str) {
        Character ch = _face.get_char(c);

        dimensions.x += (ch.advance >> 6) * scale;
        if (ch.bearing.y > static_cast<int>(dimensions.y)) {
            dimensions.y = ch.bearing.y;
        }
    }

    return dimensions;
}

// FontCollection -------------------------------------------------------------
// ----------------------------------------------------------------------------

template <typename _Ch>
bool FontsCollection<_Ch>::add_font_with_ascii(const std::string& font_path,
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
        font_name, FreeTypeFace<_Ch>(_ft_lib._native_ft_lib, font_path, size));
    if (!pair_it.second) {
        return false;
    }
    pair_it.first->second.load_ascii();
    return true;
}

template <typename _Ch>
Font<_Ch> FontsCollection<_Ch>::operator[](const std::string& font_name)
{
    auto it = _faces.find(font_name);
    if (it == _faces.end()) {
        throw WindowError("No font with name \"" + font_name + "\" found");
    }
    return Font<_Ch>(it->second);
}

// explicit instantations
template class FreeTypeLib<char>;
template class FreeTypeLib<char16_t>;
template class FreeTypeLib<char32_t>;

template class FreeTypeFace<char>;
template class FreeTypeFace<char16_t>;
template class FreeTypeFace<char32_t>;

template class Font<char>;
template class Font<char16_t>;
template class Font<char32_t>;

template class FontsCollection<char>;
template class FontsCollection<char16_t>;
template class FontsCollection<char32_t>;
}  // namespace vino