cmake_minimum_required(VERSION 3.17)
project(cgLabs)

if (WIN32)
    set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/ ${CMAKE_MODULE_PATH})
endif ()

set(CMAKE_CXX_STANDARD 20)


if (WIN32)
    include_directories(Libs/glfw/include)
    set(glm_DIR Libs/glm/cmake/glm) # if necessary
endif ()

include_directories(Libs/glad/include)
include_directories(Libs/plog/include)
find_package(OpenGL REQUIRED)

if (WIN32)
    find_package(glm REQUIRED)
    find_package(GLFW3 3.3 REQUIRED)
endif ()
if (APPLE)
    find_package(GLM REQUIRED)
    find_package(spdlog REQUIRED)
    find_package(GLFW3 REQUIRED)
endif ()

include_directories(${OPENGL_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} ${SPDLOG_INCLUDE_DIRS})
set(EXEC_LIBS Libs/glad/src/glad.c Libs/lodepng.hpp Libs/lodepng.cpp) #Add paths to libs that are used in project
set(EXEC_BASE renderer.hpp Buffers/vertex_buffer.hpp Buffers/index_buffer.hpp #Add paths to all files that are required by project
        Buffers/vertex_array.hpp
        Buffers/vertex_buffer_layout.hpp shader.hpp vertex.hpp
        functions.hpp window.hpp Buffers/color_buffer.hpp Buffers/buffer.hpp object.hpp Shapes/cone.hpp Shapes/cylinder.hpp
        texture.hpp c_map_texture.hpp)
add_executable(${PROJECT_NAME} ${EXEC_LIBS} ${EXEC_BASE} main.cpp)

if (WIN32)
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glfw3 ${GLFW_LIBRARIES} ${GLM_LIBRARIES})
endif ()
if (APPLE)
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glfw ${GLFW3_LIBRARIES} ${GLM_LIBRARIES} spdlog::spdlog)
endif ()