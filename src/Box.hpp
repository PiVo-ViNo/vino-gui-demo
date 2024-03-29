#pragma once

#include "ImgData.hpp"
#include "Shader.hpp"
#include "Window.hpp"
#include "Fonts.hpp"
#include <glm.hpp>

#include <array>
#include <memory>

namespace vino {

class IBox;
class IMovable;
class IResizable;
class ITextureColorBox;
class IDynamicBox;
class IStaticBox;

template <typename _Ch>
class TextRenderer;

class FullscreenTexture;
template <typename _Ch>
class StaticTextBox;
template <typename _Ch>
class Button;
class ForegroundFigure;

class IBox {
public:
    [[nodiscard]] virtual bool         is_cursor_in() const;
    [[nodiscard]] virtual bool         is_clicked() const;
    [[nodiscard]] virtual glm::ivec2   get_low_left_pos() const;
    [[nodiscard]] virtual unsigned int get_width() const;
    [[nodiscard]] virtual unsigned int get_height() const;

protected:
    IBox(glm::ivec2 low_left_pos, unsigned int width, unsigned int height,
         Window& parent_window) :
        _win(parent_window),
        _ll_pos(low_left_pos),
        _width(width),
        _height(height)
    {
        std::cout << "Created Box: " << _width << "x" << _height << std::endl;
    }

    Window&      _win;
    glm::ivec2   _ll_pos;
    unsigned int _width;
    unsigned int _height;
};

class IMovable {
public:
    /// Move object with no restrictions
    /// @return the new low left corner position of the object
    virtual glm::ivec2 move_no_clip(glm::ivec2 direction) = 0;

    /// Move object within bounds of the parent window
    /// @return the new low left corner position of the object
    virtual glm::uvec2 move_with_clip(glm::ivec2 direction) = 0;
};

class IResizable {
public:
    virtual glm::uvec2 resize_no_clip(glm::uvec2 new_dimension) = 0;

    virtual glm::uvec2 resize_with_clip(glm::uvec2 new_dimension) = 0;
};

class ITextureColorBox : public IBox {
public:
    ~ITextureColorBox()
    {
        glDeleteVertexArrays(1, &_box_vertex_array);
        glDeleteBuffers(1, &_box_vertex_buffer);
        glDeleteBuffers(1, &_box_element_buffer);
        glDeleteTextures(1, &_box_texture);
    }

    /// Render without setting alpha uniform value in shader (== -1.0f)
    virtual void render();
    virtual void render(float uniform_alpha);

    [[nodiscard]] glm::vec4 get_color() const;

protected:
    ITextureColorBox(glm::ivec2 low_left_pos, unsigned int width,
                     unsigned int height, Window& parent_window,
                     const ImgData& img, glm::vec4 color, int GL_TYPE_DRAW);

    Shader       _box_shader;
    glm::vec4    _color;
    unsigned int _box_vertex_buffer{};
    unsigned int _box_vertex_array{};
    unsigned int _box_element_buffer{};
    unsigned int _box_texture{};
};

class IStaticBox : public ITextureColorBox {
public:
protected:
    // This way, because both ImgData and Color shouldn't be ommited at once
    IStaticBox(glm::ivec2 low_left_pos, unsigned int width, unsigned int height,
               Window& parent_window, glm::vec4 color, const ImgData& img = {});

    IStaticBox(glm::ivec2 low_left_pos, unsigned int width, unsigned int height,
               Window& parent_window, const ImgData& img,
               glm::vec4 color = {1.0, 1.0, 1.0, 1.0});
};

class IDynamicBox : public ITextureColorBox,
                    public IMovable,
                    public IResizable {
public:
    glm::ivec2 move_no_clip(glm::ivec2 direction) override;

    glm::uvec2 move_with_clip(glm::ivec2 direction) override;

    glm::uvec2 resize_no_clip(glm::uvec2 new_dimension) override;

    glm::uvec2 resize_with_clip(glm::uvec2 new_dimension) override;

protected:
    IDynamicBox(glm::ivec2 low_left_pos, unsigned int width,
                unsigned int height, Window& parent_window, glm::vec4 color,
                const ImgData& img);

    IDynamicBox(glm::ivec2 low_left_pos, unsigned int width,
                unsigned int height, Window& parent_window, const ImgData& img,
                glm::vec4 color = {1.0, 1.0, 1.0, 1.0});

private:
    void recalculate_corners();

    std::array<std::array<int, 5>, 4> _corners;
};

template <typename _Ch>
class TextRenderer {
public:
    using char_type = _Ch;

    TextRenderer();

    ~TextRenderer()
    {
        glDeleteBuffers(1, &_text_vertex_buffer);
        glDeleteVertexArrays(1, &_text_vertex_array);
    }

    void render_text(const std::basic_string<char_type>& str,
                     const Font<char_type>& font, glm::vec3 color,
                     glm::ivec2 ll_pos, const Window& window);

    std::size_t render_text_inbound(const std::basic_string<char_type>& str,
                                    const Font<char_type>&              font,
                                    glm::vec3 color, glm::ivec2 ll_pos,
                                    unsigned int x_bound, const Window& window);

private:
    Shader       _text_shader;
    unsigned int _text_vertex_buffer{};
    unsigned int _text_vertex_array{};
};

class FullscreenTexture : public IStaticBox {
public:
    FullscreenTexture(Window& window, glm::vec4 color,
                      const ImgData& img = {}) :
        IStaticBox({0, 0}, window.get_width(), window.get_height(), window, img,
                   color)
    {
    }

    FullscreenTexture(Window& window, const ImgData& img,
                      glm::vec4 color = {1.0, 1.0, 1.0, 1.0}) :
        IStaticBox({0, 0}, window.get_width(), window.get_height(), window, img,
                   color)
    {
    }
};

template <typename _Ch>
class StaticTextBox : public IStaticBox {
public:
    using char_type = _Ch;

    StaticTextBox(glm::ivec2 low_left_pos, unsigned int width,
                  unsigned int height, Window& parent_window, glm::vec4 color,
                  const ImgData& img = {}) :
        IStaticBox(low_left_pos, width, height, parent_window, img, color)
    {
        _text = std::make_unique<TextRenderer<char_type>>();
    }

    StaticTextBox(glm::ivec2 low_left_pos, unsigned int width,
                  unsigned int height, Window& parent_window,
                  const ImgData& img = {},
                  glm::vec4      color = {1.0, 1.0, 1.0, 1.0}) :
        IStaticBox(low_left_pos, width, height, parent_window, img, color)
    {
        _text = std::make_unique<TextRenderer<char_type>>();
    }

    /// Renders text after clearing the box from the previous one
    void render_text(std::basic_string<char_type> text,
                     const Font<char_type>& font, glm::vec4 color);

private:
    std::unique_ptr<TextRenderer<char_type>> _text{};
};

template <typename _Ch>
class Button : public IStaticBox {
public:
    using char_type = _Ch;

    Button(glm::ivec2 low_left_pos, unsigned int width, unsigned int height,
           Window& parent_window, glm::vec4 font_color,
           std::basic_string<char_type> title, const Font<char_type>& font,
           const ImgData& img = {},
           glm::vec4      box_color = {1.0, 1.0, 1.0, 1.0}) :
        IStaticBox(low_left_pos, width, height, parent_window, img, box_color),
        _title(std::move(title)),
        _font(font),
        _title_color(font_color)
    {
        _text = std::make_unique<TextRenderer<char_type>>();
    }

    void render() override
    {
        IStaticBox::render();
        _text->render_text(
            _title, _font, _title_color,
            {_ll_pos.x + 10,
             _ll_pos.y + (_height - _font.get_dimensions_of("A", 1.0).y) / 2},
            _win);
    }

private:
    std::basic_string<char_type>             _title{};
    Font<char_type>                          _font;
    std::unique_ptr<TextRenderer<char_type>> _text{};
    glm::vec4                                _title_color;
};

class ForegroundFigure : public IDynamicBox {
public:
    ForegroundFigure(glm::ivec2 low_left_pos, unsigned int width,
                     unsigned int height, Window& parent_window,
                     const ImgData& img,
                     glm::vec4      color = {1.0, 1.0, 1.0, 1.0}) :
        IDynamicBox(low_left_pos, width, height, parent_window, img, color)
    {
    }
};

template <typename _Ch>
class LowBox {
public:
    using char_type = _Ch;

    /// First in program construction of LowBox
    LowBox(Window& parent_window, glm::ivec2 box_ll_pos,
           glm::uvec2 box_dimensions, const Font<char_type>& font);

    LowBox(Window& parent_window, const Font<char_type>& font) :
        LowBox(parent_window, glob_box_ll_pos, glob_box_dimensions, font)
    {
        if (!globals_set) {
            throw WindowError(
                "First vino::LowBox construction must be done through full "
                "constructor to init global LowBox parameters (!= 0)");
        }
    }

    void render();
    void render(const std::basic_string<char_type>& name,
                const std::basic_string<char_type>& text);
    void update_text(const std::basic_string<char_type>& text);

private:
    inline static glm::ivec2 glob_box_ll_pos{};
    inline static glm::uvec2 glob_box_dimensions{};
    inline static glm::vec4  glob_box_color{1.0, 1.0, 1.0, 1.0};
    inline static glm::ivec2 glob_title_ll_pos{};
    inline static glm::uvec2 glob_title_dimensions{};
    inline static bool       globals_set = false;

    StaticTextBox<char_type>     _text_box;
    StaticTextBox<char_type>     _name_box;
    std::basic_string<char_type> _text{};
    Font<char_type>              _font;
};

}  // namespace vino