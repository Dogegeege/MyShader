#ifndef GLINIT_H
#define GLINIT_H

#include <iostream>
#include <optional>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#define MY_GLFW_CONTEXT_VERSION_MAJOR 3
#define MY_GLFW_CONTEXT_VERSION_MINOR 3

std::optional<GLFWwindow*> glInit(const int screenWidth, const int screenHeight, const char* title) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MY_GLFW_CONTEXT_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MY_GLFW_CONTEXT_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Error: Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return std::nullopt;
    }
    glfwMakeContextCurrent(window);  // 创建窗口

    // 启用GLAD加载OpenGL函数,在glfwMakeContextCurrent()之后确保创建上下文
    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == false) {
        std::cerr << "Error: Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return std::nullopt;
    }

    return window;
}

#endif