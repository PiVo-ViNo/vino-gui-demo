#include "ImgData.hpp"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace vino {

ImgData::ImgData(const std::string& path_to_png)
{
    stbi_set_flip_vertically_on_load(1);
    data = stbi_load(path_to_png.c_str(), &width, &height, &numColorChannels,
                     STBI_rgb_alpha);
    std::cout << "Reading image " << path_to_png << "\n";
    std::cout << "width: " << width << "\t\t"
              << "height: " << height << "\t\t"
              << "color channels: " << numColorChannels << std::endl;
}
ImgData::~ImgData()
{
    stbi_image_free(data);
}

unsigned int configureTexture(const ImgData& img, unsigned int texNum,
                              int texParam)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0 + texNum);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // set the texture wrapping/filtering options (on the currently bound
    // texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texParam);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texParam);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (img.data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width, img.height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, img.data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    // glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}
}  // namespace vino