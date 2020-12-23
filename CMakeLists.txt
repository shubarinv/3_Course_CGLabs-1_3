cmake_minimum_required(VERSION 3.17)
project(cgLabs)


set(CMAKE_CXX_STANDARD 20)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/ ${CMAKE_MODULE_PATH})
add_subdirectory(Libs/glfw)

include_directories(Libs/glfw/include)
include_directories(Libs/corrade/src)
include_directories(Libs/magnum/src)


# Add Corrade as a subproject
set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)
add_subdirectory(Libs/corrade EXCLUDE_FROM_ALL)
# Add Magnum as a subproject, enable GLFWApplication
set(WITH_GLFWAPPLICATION ON CACHE BOOL "" FORCE)
set(WITH_SDL2APPLICATION OFF CACHE BOOL "" FORCE)
add_subdirectory(Libs/magnum EXCLUDE_FROM_ALL)
find_package(Magnum REQUIRED GL Shaders GlfwApplication)

add_executable(${PROJECT_NAME} lab2.cpp vertex.hpp mesh.hpp) # change lab here

target_link_libraries(${PROJECT_NAME} PRIVATE
        Magnum::Application
        Magnum::GL
        Magnum::Shaders
        Magnum::Magnum)