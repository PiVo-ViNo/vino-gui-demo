#pragma once

#include <glad/glad.h>
#include <string>

namespace vino {

struct ImgData {
    int            width{};
    int            height{};
    int            numColorChannels{};
    unsigned char* data{};

    explicit ImgData(const std::string& path_to_png);

    ~ImgData();
};

unsigned int configureTexture(const ImgData& img, unsigned int texNum,
                              int texParam = GL_REPEAT);

}  // namespace vino