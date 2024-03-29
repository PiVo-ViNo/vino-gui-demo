#pragma once

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
class LowBox;
template <typename _Ch>
class Button;
class ForegroundFigure;

class IBox {
public:
    [[nodiscard]] virtual bool is_cursor_in() const;
    [[nodiscard]] virtual bool is_clicked() const;

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
    }

    virtual void render() = 0;

protected:
    ITextureColorBox(glm::ivec2 low_left_pos, unsigned int width,
                     unsigned int height, Window& parent_window,
                     glm::vec4 color) :
        IBox(low_left_pos, width, height, parent_window),
        _box_shader("../shaders/boxVertex.glsl", "../shaders/colorFrag.glsl"),
        _color(color)
    {
        glGenBuffers(1, &_box_vertex_buffer);
        glGenVertexArrays(1, &_box_vertex_array);
        glGenBuffers(1, &_box_element_buffer);
    }

    Shader       _box_shader;
    glm::vec4    _color;
    unsigned int _box_vertex_buffer{};
    unsigned int _box_vertex_array{};
    unsigned int _box_element_buffer{};
};

class IStaticBox : public ITextureColorBox {
public:
    void render() override;

protected:
    IStaticBox(glm::ivec2 low_left_pos, unsigned int width, unsigned int height,
               Window& parent_window, glm::vec4 color);
};

class IDynamicBox : public ITextureColorBox,
                    public IMovable,
                    public IResizable {
public:
    void render() override;

    glm::ivec2 move_no_clip(glm::ivec2 direction) override;

    glm::uvec2 move_with_clip(glm::ivec2 direction) override;

    glm::uvec2 resize_no_clip(glm::uvec2 new_dimension) override;

    glm::uvec2 resize_with_clip(glm::uvec2 new_dimension) override;

protected:
    IDynamicBox(glm::ivec2 low_left_pos, unsigned int width,
                unsigned int height, Window& parent_window, glm::vec4 color);

private:
    void recalculate_corners();

    const std::size_t                 k_size_of_corners = 12 * sizeof(int);
    std::array<std::array<int, 3>, 4> _corners;
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
    FullscreenTexture(Window& window, glm::vec4 color) :
        IStaticBox({0, 0}, window.get_width(), window.get_height(), window,
                   color)
    {
    }
};

template <typename _Ch>
class LowBox : public IStaticBox {
public:
    using char_type = _Ch;

    LowBox(glm::ivec2 low_left_pos, unsigned int width, unsigned int height,
           Window& parent_window, glm::vec4 color) :
        IStaticBox(low_left_pos, width, height, parent_window, color)
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
           Window& parent_window, glm::vec4 box_color, glm::vec4 title_color,
           std::basic_string<char_type> title, const Font<char_type>& font) :
        IStaticBox(low_left_pos, width, height, parent_window, box_color),
        _title(std::move(title)),
        _font(font),
        _title_color(title_color)
    {
        _text = std::make_unique<TextRenderer<char_type>>();
    }

    void render() override
    {
        IStaticBox::render();
        _text->render_text(_title, _font, _title_color,
                           {_ll_pos.x + 10, _ll_pos.y + _height / 2}, _win);
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
                     glm::vec4 color) :
        IDynamicBox(low_left_pos, width, height, parent_window, color)
    {
    }
};

}  // namespace vino