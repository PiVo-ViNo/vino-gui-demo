cmake_minimum_required(VERSION 3.25)
project(ViNo_gui VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_LINKER lld-link)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/runtime")

if(CMAKE_BUILD_TYPE EQUAL Release)
    set(BUILD_SHARED_LIBS ON)
endif()

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
if(CMAKE_BUILD_TYPE EQUAL Release)
    add_compile_options(-O2)
else ()
    add_compile_options(-g)
endif()

# ---- Additional cmake function to add target definitions ----
include(cmake/add_default_target_compile_definitions.cmake)

# Object Library
add_library(vinogui SHARED 
    src/glad.c 
    src/Window.cpp
    src/ImgData.cpp
    src/Fonts.cpp
    src/Box.cpp
)

target_include_directories(vinogui
    PUBLIC "${CMAKE_SOURCE_DIR}/include"
    PUBLIC "${DEPS_SRC}/ft2/include"
    PUBLIC "${DEPS_SRC}/glm"
    PUBLIC "${DEPS_SRC}/glfw3/include"
)
add_default_target_compile_definitions(vinogui)

target_link_libraries(vinogui
    PUBLIC glfw
    PUBLIC freetype
)
# Executable
if(WIN32)
    add_executable(ViNo_gui_exe WIN32)
else()
    add_executable(ViNo_gui_exe)
endif()

target_sources(ViNo_gui_exe
    PUBLIC src/main.cpp
    PUBLIC src/TitleScreen.cpp
    PUBLIC src/MainMenu.cpp
)
add_executable(ViNo_gui::exe ALIAS ViNo_gui_exe)
add_default_target_compile_definitions(ViNo_gui_exe)

set_property(TARGET ViNo_gui_exe PROPERTY OUTPUT_NAME vino_gui)

add_link_options(-Wl,"/entry:mainCRTStartup /subsystem:windows")
target_link_libraries(ViNo_gui_exe PRIVATE vinogui)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
