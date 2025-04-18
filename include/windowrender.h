#ifndef WINDOWRENDER_H
#define WINDOWRENDER_H

#include <iostream>
#include <map>
#include <optional>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "camera.h"

#define MY_GLFW_CONTEXT_VERSION_MAJOR 3
#define MY_GLFW_CONTEXT_VERSION_MINOR 3

void APIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

std::optional<GLFWwindow*> glInit(const int screenWidth, const int screenHeight, const char* title);

class WindowRender {
   public:
    WindowRender(Camera& camera, std::string name);

    void processInput(float deltaTime);

    inline GLFWwindow* getWindow() { return window; }

    virtual ~WindowRender() {
        glfwDestroyWindow(window);
        glfwTerminate();
    };

    float       screenWidth;
    float       screenHeight;
    std::string windowName;

    static std::map<GLFWwindow*, Camera*> windowCameraMap;  // 静态映射表,用于回调函数

   private:
    GLFWwindow* window;
    Camera*     camera;

    GLFWwindow* InitWindow();
};

#endif