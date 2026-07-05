#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

constexpr unsigned WIDTH = 800;
constexpr unsigned HEIGHT = 600;

constexpr unsigned OPENGL_MAJOR = 4;
constexpr unsigned OPENGL_MINOR = 6;

constexpr auto PROJECT_NAME = "OpenGL Learn";

GLFWwindow *createWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, PROJECT_NAME, nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    glfwSetWindowPos(window, 100, 100);
    glfwMakeContextCurrent(window);

    const int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        exit(-1);
    }

    std::cout << std::format(
        "Loaded OpenGL {}.{}\n",
        GLAD_VERSION_MAJOR(version),
        GLAD_VERSION_MINOR(version)
    );

    return window;
}

int main() {
    const auto window = createWindow();

    glViewport(0, 0, WIDTH, HEIGHT);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
