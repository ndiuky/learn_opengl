include(FetchContent)
FetchContent_Declare(
        glad
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git
        GIT_TAG v2.0.8
)

FetchContent_MakeAvailable(glad)

add_subdirectory("${glad_SOURCE_DIR}/cmake" "glad_cmake")

glad_add_library(glad_gl_core_46 STATIC API gl:core=4.6)
