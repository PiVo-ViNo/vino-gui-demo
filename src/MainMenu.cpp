#include "MainMenu.hpp"

#include <GLFW/glfw3.h>
#include <string>

// #include "ImgData.hpp"
// #include "ScreenTexture.hpp"
#include "Fonts.hpp"
#include "Box.hpp"
#include "Shader.hpp"

namespace vino {

void mainMenu(Window& window)
{
    Shader menu_shader("../shaders/basicVertex.glsl",
                       "../shaders/basicFrag.glsl");
    menu_shader.use();
    menu_shader.setInt("uTexture", 0);

    FullscreenTexture fs_texture(window, {1.0f, 1.0f, 1.0f, 1.0f});

    LowBox main_box({10, 10}, window.get_width() - 20, 300, window,
                    {1.0f, 0.1f, 1.0f, 1.0f});

    FontsCollection fonts;
    /// TODO: make possible to choose different sizes
    fonts.add_font_with_ascii("../fonts/ARIAL.ttf", 22);
    Font   arial = fonts["ARIAL"];
    Button main_button({10, 310}, 100, 50, window, {1.0f, 1.0f, 0.0f, 1.0f},
                       {0.0f, 0.0f, 0.0f, 1.0f}, "Button", arial);

    ForegroundFigure olegus({100, 50}, 300, 500, window,
                            {0.1f, 0.0f, 1.0f, 1.0f});

    olegus.move_with_clip({300, 0});

    std::string screen_str = "Olegus";

    std::cout << std::endl;
    while (!window.should_close()) {
        if (window.is_pressed(GLFW_KEY_ESCAPE)) {
            window.close();
        }

        glClearColor(0.2f, 0.7f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        fs_texture.render();
        olegus.render();
        if (main_box.is_clicked()) {
            main_box.render();
        } else {
            main_box.render();
        }
        main_button.render();

        main_box.render_text(screen_str, arial, {1.0f, 1.0f, 1.0f, 1.0f});

        window.swap_buffers();
        glfwPollEvents();
    }
}

}  // namespace vino