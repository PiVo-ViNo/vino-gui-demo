#include "MainMenu.hpp"

#include <GLFW/glfw3.h>
#include <cstdint>

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
    Box main_box(10, 10, window.get_width() - 20, 300, window); 
    FontsCollection fonts;
    fonts.add_font_with_ascii("../fonts/ARIAL.ttf", 48);
    Font arial = fonts["ARIAL"];

    while (!window.should_close()) {
        if (window.is_pressed(GLFW_KEY_ESCAPE)) {
            window.close();
        }

        glClearColor(0.2f, 0.7f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        main_box.render_box();
        main_box.render_str("Hello, my dear friend Olegus!", arial, {1.0f, 1.0f, 1.0f});

        window.swap_buffers();
        glfwPollEvents();
    }
}

}  // namespace vino