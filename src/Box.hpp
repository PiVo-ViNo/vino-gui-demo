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

class TextRenderer;

class FullscreenTexture;
class LowBox;
class Button;
// class ForegroundFigure;

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

class TextRenderer {
public:
    TextRenderer();

    ~TextRenderer()
    {
        glDeleteBuffers(1, &_text_vertex_buffer);
        glDeleteVertexArrays(1, &_text_vertex_array);
    }

    void render_str(const std::string& str, Font& font, glm::vec3 color,
                    glm::ivec2 ll_pos, const Window& window);

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

class LowBox : public IStaticBox {
public:
    LowBox(glm::ivec2 low_left_pos, unsigned int width, unsigned int height,
           Window& parent_window, glm::vec4 color) :
        IStaticBox(low_left_pos, width, height, parent_window, color)
    {
        _text = std::make_unique<TextRenderer>();
    }

    void render_text(std::string text, Font& font, glm::vec4 color)
    {
        _text->render_str(text, font, color,
                          {_ll_pos.x + 10, _ll_pos.y + _height / 2}, _win);
    }

private:
    std::unique_ptr<TextRenderer> _text{};
};

class Button : public IStaticBox {
public:
    Button(glm::ivec2 low_left_pos, unsigned int width, unsigned int height,
           Window& parent_window, glm::vec4 box_color, glm::vec4 title_color,
           std::string title, Font& font) :
        IStaticBox(low_left_pos, width, height, parent_window, box_color),
        _title(std::move(title)),
        _font(font),
        _title_color(title_color)
    {
        _text = std::make_unique<TextRenderer>();
    }

    void render() override
    {
        IStaticBox::render();
        _text->render_str(_title, _font, _title_color,
                          {_ll_pos.x + 10, _ll_pos.y + _height / 2}, _win);
    }

private:
    std::string                   _title{};
    Font                          _font;
    std::unique_ptr<TextRenderer> _text{};
    glm::vec4                     _title_color;
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