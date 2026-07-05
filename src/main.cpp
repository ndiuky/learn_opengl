#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

constexpr int OPENGL_MAJOR = 4;
constexpr int OPENGL_MINOR = 6;

constexpr auto PROJECT_NAME = "OpenGL Learn";

constexpr float vertices[] = {
    // positions          // colors
   -0.5f,  0.5f, 0.0f,    1.0f, 1.0f, 0.0f,
   -0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,

   -0.5f,  0.5f, 0.0f,    1.0f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,
    0.5f,  0.5f, 0.0f,    1.0f, 0.0f, 0.0f,
};

std::string readResourceFile(const std::filesystem::path &relativePath) {
    const auto fullPath = RESOURCE_DIR / relativePath;

    std::ifstream file(fullPath);
    if (!file.is_open()) {
        throw std::runtime_error(
            std::string("Failed to open resource file: ") + fullPath.string()
        );
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

GLFWwindow *createWindow() {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(
        WIDTH,
        HEIGHT,
        PROJECT_NAME,
        nullptr,
        nullptr
    );

    if (window == nullptr) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwSetWindowPos(window, 100, 100);
    glfwMakeContextCurrent(window);

    const int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        glfwDestroyWindow(window);
        glfwTerminate();
        throw std::runtime_error("Failed to initialize OpenGL context");
    }

    std::cout << std::format(
        "Loaded OpenGL {}.{}\n",
        GLAD_VERSION_MAJOR(version),
        GLAD_VERSION_MINOR(version)
    );

    return window;
}

GLuint compileShader(GLenum type, const std::string &source) {
    const GLuint shader = glCreateShader(type);

    const char *sourcePtr = source.c_str();
    glShaderSource(shader, 1, &sourcePtr, nullptr);
    glCompileShader(shader);

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);

        glDeleteShader(shader);

        throw std::runtime_error(
            std::string("Shader compilation failed:\n") + infoLog
        );
    }

    return shader;
}

GLuint createShaderProgram(
    const std::filesystem::path &vertexPath,
    const std::filesystem::path &fragmentPath
) {
    const std::string vertexCode = readResourceFile(vertexPath);
    const std::string fragmentCode = readResourceFile(fragmentPath);

    const GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexCode);
    const GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentCode);

    const GLuint program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);

        glDeleteProgram(program);

        throw std::runtime_error(
            std::string("Shader program linking failed:\n{}") + infoLog
        );
    }

    return program;
}

GLuint createVAO() {
    GLuint vao = 0;
    GLuint vbo = 0;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    constexpr GLsizei stride = 6 * sizeof(float);

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        stride,
        nullptr
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        stride,
        reinterpret_cast<void *>(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    return vao;
}

int main() {
    try {
        GLFWwindow *window = createWindow();

        glViewport(0, 0, WIDTH, HEIGHT);

        const GLuint vao = createVAO();

        const GLuint shaderProgram = createShaderProgram(
            std::filesystem::path("shaders") / "shader.vert",
            std::filesystem::path("shaders") / "shader.frag"
        );

        while (!glfwWindowShouldClose(window)) {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glUseProgram(shaderProgram);
            glBindVertexArray(vao);

            const int uni_loc = glGetUniformLocation(shaderProgram, "scr_aspect");
            glUniform1f(uni_loc, (static_cast<float>(HEIGHT) / static_cast<float>(WIDTH)));

            const int uni_angle_loc = glGetUniformLocation(shaderProgram, "angle");
            glUniform1f(uni_angle_loc, static_cast<float>(glfwGetTime()));

            glDrawArrays(GL_TRIANGLES, 0, 6);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glDeleteVertexArrays(1, &vao);
        glDeleteProgram(shaderProgram);

        glfwDestroyWindow(window);
        glfwTerminate();

        return 0;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        glfwTerminate();
        return -1;
    }
}
