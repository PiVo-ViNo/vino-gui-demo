// #define STB_IMAGE_IMPLEMENTATION
#include <iostream>

#include "MainMenu.hpp"
#include "TitleScreen.hpp"
#include "Window.hpp"
#include "Fonts.hpp"

namespace vino {

struct Box {
    const float positions_coord;
    const float texture_coord;
};

} // namespace vino

int main()
{
    vino::NonResizableWindow main_window(800, 600, "ViNo");
    main_window.make_current();

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    int ft_err = vino::load_libFreeType();
    // show title ViNo for 7s
    vino::titleScreen(main_window);

    // show main menu
    vino::mainMenu(main_window);

    return 0;
}
