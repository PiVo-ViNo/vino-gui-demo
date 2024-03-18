#include "MainMenu.hpp"

#include <GLFW/glfw3.h>

// #include "ImgData.hpp"
// #include "ScreenTexture.hpp"
#include "Shader.hpp"
#include "Fonts.hpp"

namespace vino {

void mainMenu(Window& window)
{
    Shader menu_shader("../shaders/basicVertex.glsl",
                       "../shaders/basicFrag.glsl");

    menu_shader.use();
    menu_shader.setInt("uTexture", 0);

    while (!window.should_close()) {
        if (window.is_pressed(GLFW_KEY_ESCAPE)) {
            window.close();
        }

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        window.swap_buffers();
        glfwPollEvents();
    }
}

}  // namespace vino