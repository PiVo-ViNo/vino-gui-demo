#include "MainMenu.hpp"

// #include "ImgData.hpp"
// #include "ScreenTexture.hpp"
#include "Fonts.hpp"
#include "Box.hpp"
#include "Shader.hpp"

#include <GLFW/glfw3.h>
#include <deque>
#include <string>

namespace vino {

void mainMenu(Window& window)
{
    Shader menu_shader("../shaders/basicVertex.glsl",
                       "../shaders/basicFrag.glsl");
    menu_shader.use();
    menu_shader.setInt("uTexture", 0);

    FullscreenTexture fs_texture(window, {1.0f, 1.0f, 1.0f, 1.0f});

    LowBox<char32_t> main_box({10, 10}, window.get_width() - 20, 300, window,
                    {1.0f, 0.1f, 1.0f, 1.0f});

    FontsCollection<char32_t> fonts;
    /// TODO: make possible to choose different sizes
    fonts.add_font_with_ascii("../fonts/ARIALBI.ttf", 22);
    // Font   arial = fonts["ARIAL"];
    Button<char32_t> main_button({10, 310}, 100, 50, window, {1.0f, 1.0f, 0.0f, 1.0f},
                       {0.0f, 0.0f, 0.0f, 1.0f}, U"Olegus", fonts["ARIALBI"]);

    ForegroundFigure olegus({100, 50}, 300, 500, window,
                            {0.1f, 0.0f, 1.0f, 1.0f});

    olegus.move_with_clip({300, 0});

    std::deque<const std::u32string> texts;

    texts.emplace_back(
        U"œ\n"
        "Where œmerge Aragva and her twin, \n"
        "Kura, and fast rush onward, in \n"
        "Times past, a lonely cloister stood; \n"
        "By fields, a dense and o'ergrown wood \n"
        "Encircled 'twas.... A wayfarer, \n"
        "Toiling uphill, will see what were \n"
        "A gate and gateposts once and, too,");
    texts.emplace_back(U"A church.... To-day, no incense to \n"
        "Its round dome coils, nor do a prayer \n"
        "The humble monks chant, hoarse-voiced, there. \n"
        "Alone, forgot by death and men, \n"
        "A bent old greybeard, denizen \n"
        "Of these remote and desolate hills, \n"
        "Over the ruins watches still ");
    texts.emplace_back(U"And daily wipes the dust that clings \n"
        "To tombs, of which the letterings \n"
        "Of glories past speak and of things \n"
        "Of like note. Of a tsar one such \n"
        "Tells who by his gold crown was much \n"
        "Weighed down, and did of Russia gain \n"
        "The patronage o'er his domain. \n"
        "Twas then God's love descended on \n"
        "The land, and Georgia bloomed, and gone \n"
        "Her old fears were and old suspense: \n"
        "Of friendly bayonets a fence \n"
        "Did, bristling, rise in her defence.");

    std::size_t cur_text = 0;

    glfwSetTime(0);
    std::cout << std::endl;
    while (!window.should_close()) {
        if (window.is_pressed(GLFW_KEY_ESCAPE)) {
            window.close();
        }
        if (glfwGetTime() >= 0.5 && window.is_pressed(GLFW_KEY_SPACE)) {
            cur_text = (cur_text + 1) % texts.size();
            glfwSetTime(0);
            olegus.move_with_clip({-50, 0});
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

        main_box.render_text(texts[cur_text], fonts["ARIALBI"], {1.0f, 1.0f, 1.0f, 1.0f});

        window.swap_buffers();
        glfwPollEvents();
    }
}

}  // namespace vino