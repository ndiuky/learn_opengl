cmake_minimum_required(VERSION 4.2)
project(learn_opengl)

set(CMAKE_CXX_STANDARD 26)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(GLFW)
include(GLAD)
include(STB)

file(GLOB_RECURSE SRCS src/*.cpp)

add_executable(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME} PRIVATE glfw glad_gl_core_46 stb)
target_compile_definitions(${PROJECT_NAME} PRIVATE RESOURCE_DIR="${CMAKE_SOURCE_DIR}/resources")
