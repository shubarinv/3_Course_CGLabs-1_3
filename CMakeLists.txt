cmake_minimum_required(VERSION 3.17)
project(cgLabs)

if (WIN32)
    set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/ ${CMAKE_MODULE_PATH})
endif ()

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(Libs/corrade)
add_subdirectory(Libs/magnum)


include_directories(${MAGNUM_INCLUDE_DIRS} )
set(EXEC_LIBS ) #Add paths to libs that are used in project
set(EXEC_BASE )
add_executable(${PROJECT_NAME} ${EXEC_LIBS} ${EXEC_BASE} ) # change lab here

if (WIN32)
    target_link_libraries(${PROJECT_NAME} ${MAGNUM_LIBRARIES} )
endif ()
if (APPLE)
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
endif ()