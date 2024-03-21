// #define STB_IMAGE_IMPLEMENTATION
#include <iostream>

#include "Window.hpp"
#include "MainMenu.hpp"
#include "TitleScreen.hpp"

int main()
{
    vino::NonResizableWindow main_window(800, 600, "ViNo");
    main_window.make_current();

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // glEnable(GL_CULL_FACE);

    // vino::FontsCollection fonts_collection; 
    // fonts_collection.add_font("../fonts/ARIAL.ttf");
    // fonts_collection["arial"]; // -> Font

    // vino::Box main_box();
    // main_box.render_str(fonts_collection["arial"], {0.0f, 0.0f, 0.5f});

    // show title ViNo for 7s
    vino::titleScreen(main_window);

    // show main menu
    vino::mainMenu(main_window);

    return 0;
}
