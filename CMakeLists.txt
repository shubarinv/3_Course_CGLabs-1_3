cmake_minimum_required(VERSION 3.17)
project(cgLabs)

set(CMAKE_CXX_STANDARD 20)
if (NOT TARGET spdlog)
    # Stand-alone build
    find_package(spdlog REQUIRED)
endif ()
find_package(OpenGL REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS}
        ${SPDLOG_INCLUDE_DIRS})

add_executable(cgLabs Application.cpp renderer.hpp vertex_buffer.hpp index_buffer.hpp vertex_array.hpp vertex_buffer_layout.hpp shader.hpp vertex.hpp lib.hpp)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glfw ${GLFW_LIBRARIES}GLEW::GLEW ${GLM_LIBRARIES}
        spdlog::spdlog)