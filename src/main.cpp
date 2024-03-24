// #define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <stdexcept>

#include "Window.hpp"
#include "MainMenu.hpp"
#include "TitleScreen.hpp"
#include "Box.hpp"
#include "GLFW/glfw3.h"

int main()
{
    vino::NonResizableWindow main_window(800, 600, "ViNo");
    main_window.make_current();

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    try {
        // vino::FontsCollection fonts_collection;
        // fonts_collection.add_font("../fonts/ARIAL.ttf");
        // fonts_collection["arial"]; // -> Font

        // vino::Box main_box();
        // main_box.render_str(fonts_collection["arial"], {0.0f, 0.0f, 0.5f});

        // show title ViNo for 7s
        vino::titleScreen(main_window);

        // show main menu
        vino::mainMenu(main_window);

        throw std::runtime_error("ОЛЕГУС BOM BOM BOM!");
    } catch (std::exception& ex) {
        vino::NonResizableWindow err_window(500, 200, "ViNo Error");
        err_window.make_current();

        vino::LowBox<char> err_box({0, 0}, 500, 200, err_window,
                             {1.0f, 1.0f, 1.0f, 1.0f});

        vino::FontsCollection<char> fonts;
        /// TODO: make possible to choose different sizes
        fonts.add_font_with_ascii("../fonts/ARIAL.ttf", 22);
        while (!err_window.should_close()) {
            if (err_window.is_pressed(GLFW_KEY_ESCAPE)) {
                err_window.close();
            }

            err_box.render();
            err_box.render_text("Error: " + std::string(ex.what()), fonts["ARIAL"],
                                {0.0f, 0.0f, 0.0f, 1.0f});
            err_window.swap_buffers();
            glfwPollEvents();
        }
    }

    return 0;
}
