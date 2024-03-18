cmake_minimum_required(VERSION 3.25)
project(vino_gui VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

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
    glfw3
    URL https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip
    SOURCE_DIR ${DEPS_SRC}/glfw3
    FIND_PACKAGE_ARGS 3.3...<3.5
)
FetchContent_MakeAvailable(glm ft2 glfw3)

add_executable(vino_gui 
    src/main.cpp 
    src/glad.c 
    src/Window.cpp
    src/TitleScreen.cpp
    src/MainMenu.cpp
    src/ImgData.cpp
    src/Fonts.cpp
)

target_include_directories(vino_gui
    PUBLIC "${CMAKE_SOURCE_DIR}/include"
    PUBLIC "${DEPS_SRC}/ft2/include"
    PUBLIC "${DEPS_SRC}/glm"
)

target_link_libraries(vino_gui 
    PRIVATE glfw3
    PRIVATE freetype 
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
