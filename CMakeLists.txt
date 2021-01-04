cmake_minimum_required(VERSION 3.17)
project(cgLabs)


set(CMAKE_CXX_STANDARD 20)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/ ${CMAKE_MODULE_PATH})
set(CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Libs/magnum/modules ${CMAKE_CURRENT_SOURCE_DIR}/Libs/corrade/modules ${CMAKE_MODULE_PATH})
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
find_package(Corrade REQUIRED)
add_subdirectory(Libs/magnum EXCLUDE_FROM_ALL)
find_package(Magnum REQUIRED GL Shaders GlfwApplication Primitives Trade)

add_executable(${PROJECT_NAME} Labs/lab4.cpp vertex.hpp mesh.hpp lights.hpp camera.hpp texture.hpp) # change lab here

target_link_libraries(${PROJECT_NAME} PRIVATE
        Magnum::Application
        Magnum::GL
        Magnum::Shaders
        Magnum::Primitives
        Magnum::Trade
        Magnum::Magnum)