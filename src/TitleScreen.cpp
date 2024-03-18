#include "TitleScreen.hpp"

#include <GLFW/glfw3.h>

#include "ImgData.hpp"
#include "ScreenTexture.hpp"
#include "Shader.hpp"

namespace vino {

constexpr double k_pi = 3.14159265358979323846;

void titleScreen(vino::Window& window)
{
    ScreenTexture title_scr_tex;

    Shader title_shader("../shaders/basicVertex.glsl",
                        "../shaders/basicFrag.glsl");

    // load .png, generate it
    ImgData img("../title_screen.png");

    unsigned int texture0 = configureTexture(img, 0);
    title_shader.use();
    title_shader.setInt("uTexture", 0);

    glfwSetTime(0.0);

    while (!window.should_close()) {
        double time = glfwGetTime();
        // if (time > 6.0) {
            // return;
        // }
        float  alpha = -((time - 2.5) / 2.5) * ((time - 2.5) / 2.5) + 1;
        title_shader.setFloat("ufAlpha", alpha);

        title_scr_tex.render(title_shader, texture0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse
        // moved etc.)
        window.swap_buffers();
        glfwPollEvents();
    }
}

}  // namespace vino