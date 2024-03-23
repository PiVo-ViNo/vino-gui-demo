#pragma once

#include "custom_errors.hpp"
#include <glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>

namespace vino {

struct Character;
class FreeTypeLib;
class FreeTypeFace;
class Font;
class FontsCollection;

struct Character {
    unsigned int texture_id;  // ID handle of the glyph texture
    glm::ivec2   size;        // Size of glyph
    glm::ivec2   bearing;     // Offset from baseline to left/top of glyph
    unsigned int advance;     // Offset to advance to next glyph
};

class FreeTypeLib {
public:
    friend class FontsCollection;

    FreeTypeLib()
    {
        // return Error code, so 0 is success (well, i didn't this C code)
        if (FT_Init_FreeType(&_native_ft_lib)) {
            throw WindowError(
                "ERROR::FREETYPE::Couldn't init FreeType2 library");
        }
    }

    ~FreeTypeLib() { FT_Done_FreeType(_native_ft_lib); }

    /// Q: FT_Library isn't something global, there could be many of them,
    /// so we could move them?
    FreeTypeLib(const FreeTypeLib&) = delete;
    FreeTypeLib(FreeTypeLib&&) = delete;
    FreeTypeLib& operator=(const FreeTypeLib&) = delete;
    FreeTypeLib& operator=(FreeTypeLib&& other) = delete;

private:
    FT_Library _native_ft_lib{};
};

class FreeTypeFace {
public:
    Character& get_char(const char& ch);

    FreeTypeFace(FreeTypeFace&& other) :
        _font_path(std::move(other._font_path)),
        _chars_map(std::move(other._chars_map))
    {
        FT_Face ptrTemp = other._native_ft_face;
        other._native_ft_face = nullptr;
        _native_ft_face = ptrTemp;
    }

    FreeTypeFace& operator=(const FreeTypeFace&) = delete;

    ~FreeTypeFace() { FT_Done_Face(_native_ft_face); }

private:
    friend class FontsCollection;

    FreeTypeFace(FT_Library& ft_lib, std::string font_path,
                 unsigned int pxl_size) :
        _font_path(std::move(font_path))
    {
        if (_font_path.empty()) {
            throw WindowError("ERROR::FREETYPE::Empty path to font");
        }
        if (FT_Error err =
                FT_New_Face(ft_lib, _font_path.c_str(), 0, &_native_ft_face))
        {
            throw WindowError("ERROR::FREETYPE " + std::to_string(err)
                              + "::Couldn't init FreeTypeFace");
        }
        if (FT_Error err = FT_Set_Pixel_Sizes(_native_ft_face, 0, pxl_size)) {
            throw WindowError("ERROR::FREETYPE " + std::to_string(err)
                              + "::Couldn't set pixel size");
        }
    }

    /// @param `pixel_height` can be ommited, makes it equal to `pixel_width`
    void set_pixel_size(unsigned int pixel_width,
                        unsigned int pixel_height = 0);
    /// TODO: Make support for every UTF-8 symbol
    void load_symbol(unsigned char ch, bool in_cycle = false);
    void load_ascii();

    std::string               _font_path{};
    FT_Face                   _native_ft_face{};
    std::map<char, Character> _chars_map{};
};

class Font {
public:
    explicit Font(FreeTypeFace& face) : _face(face) {}

    void render_str(const std::string& str, unsigned int vbo, glm::uvec2 ll_pos,
                    float scale) const;

    /// @return how many chars from str was rendered
    std::size_t render_str_inbound(const std::string& str, unsigned int vbo,
                              glm::uvec2 ll_pos, float scale,
                              unsigned int x_bound) const;

    glm::uvec2 get_dimensions_of(const std::string& str, float scale) const;

private:
    FreeTypeFace& _face;
};

class FontsCollection {
public:
    // explicit FontCollection(FreeTypeLib& ft_lib) : _ft_lib(ft_lib) {}

    bool add_font(const std::string& font_path, unsigned int size);
    bool add_font_with_ascii(const std::string& font_path, unsigned int size);

    Font operator[](const std::string& font_name);

private:
    FreeTypeLib _ft_lib{};
    /// TODO: write hash(FreeTypeFace), so std::set can be used
    std::map<std::string, FreeTypeFace> _faces{};
};

}  // namespace vino