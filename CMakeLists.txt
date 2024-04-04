cmake_minimum_required(VERSION 3.25)
project(vino_gui VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_LINKER lld-link)

set(DEPS_SRC "${CMAKE_SOURCE_DIR}/_deps_src")
include(FetchContent)
FetchContent_Declare(
    glm	
	URL https://github.com/g-truc/glm/releases/download/1.0.1/glm-1.0.1-light.7z 
	SOURCE_DIR ${DEPS_SRC}/glm
)
FetchContent_Declare(
    ft2
    URL https://deac-fra.dl.sourceforge.net/project/freetype/freetype2/2.13.2/freetype-2.13.2.tar.xz
    SOURCE_DIR ${DEPS_SRC}/ft2
)
FetchContent_Declare(
    glfw
    URL https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip
    SOURCE_DIR ${DEPS_SRC}/glfw3
)
FetchContent_MakeAvailable(glm ft2 glfw)
 
add_compile_options(-Wall -Wextra -gdwarf-4)

# ---- Additional cmake function to add target definitions ----
include(cmake/add_default_target_compile_definitions.cmake)

# Object Library
add_library(libvinogui OBJECT 
    src/glad.c 
    src/Window.cpp
    src/TitleScreen.cpp
    src/MainMenu.cpp
    src/ImgData.cpp
    src/Fonts.cpp
    src/Box.cpp
)

target_include_directories(libvinogui
    PUBLIC "${CMAKE_SOURCE_DIR}/include"
    PUBLIC "${DEPS_SRC}/ft2/include"
    PUBLIC "${DEPS_SRC}/glm"
    PUBLIC "${DEPS_SRC}/glfw3/include"
)
add_default_target_compile_definitions(libvinogui)

target_link_libraries(libvinogui
    PUBLIC glfw
    PUBLIC freetype
)
# Executable
add_executable(vino_gui_exe src/main.cpp)
add_executable(vino_gui::exe ALIAS vino_gui_exe)
add_default_target_compile_definitions(vino_gui_exe)

set_property(TARGET vino_gui_exe PROPERTY OUTPUT_NAME vino_gui)

add_link_options(-Wl,"/entry:mainCRTStartup /subsystem:windows")
target_link_libraries(vino_gui_exe PRIVATE libvinogui)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
