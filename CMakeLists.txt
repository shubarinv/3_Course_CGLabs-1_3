cmake_minimum_required(VERSION 3.17)
project(cgLabs)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/ ${CMAKE_MODULE_PATH})

set(CMAKE_CXX_STANDARD 20)


if (WIN32)
    include_directories(Libs/glfw/include)
    set(glm_DIR Libs/glm/cmake/glm) # if necessary
endif ()

include_directories(Libs/glad/include)

find_package(OpenGL REQUIRED)

if (WIN32)
    find_package(glm REQUIRED)
    find_package(GLFW3 3.3 REQUIRED)
endif ()
if (APPLE)
    find_package(GLM REQUIRED)
    find_package(GLFW3 REQUIRED)
endif ()

include_directories(${OPENGL_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} ${SPDLOG_INCLUDE_DIRS})

add_executable(cgLabs Libs/glad/src/glad.c renderer.hpp Buffers/vertex_buffer.hpp Buffers/index_buffer.hpp
        Buffers/vertex_array.hpp
        Buffers/vertex_buffer_layout.hpp shader.hpp vertex.hpp
        lib.hpp window.hpp Buffers/color_buffer.hpp Buffers/buffer.hpp Lab4.cpp object.hpp Shapes/cone.hpp Shapes/cylinder.hpp
        Libs/lodepng.hpp Libs/lodepng.cpp texture.hpp)

if (WIN32)
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glfw3 ${GLFW_LIBRARIES} ${GLM_LIBRARIES} )
endif ()
if (APPLE)
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glfw ${GLFW3_LIBRARIES} ${GLM_LIBRARIES} spdlog::spdlog)
endif ()