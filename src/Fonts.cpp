#include "Fonts.hpp"

#include <iostream>
#include <map>

#include <glad/glad.h>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace vino {

int load_libFreeType()
{
    FT_Library ft_lib;
    if (FT_Init_FreeType(&ft_lib)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library"
                  << std::endl;
        return -1;
    }

    FT_Face face;
    if (FT_New_Face(ft_lib, "../fonts/ARIAL.ttf", 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return -1;
    }


    return 0;
}

}  // namespace vino