#ifndef WINDOWRENDER_H
#define WINDOWRENDER_H

#include <iostream>
#include <map>
#include <optional>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <GLFW/glfw3.h>

#include "camera.h"
#include "input_management.h"
#include "object.h"

#define MY_GLFW_CONTEXT_VERSION_MAJOR 3
#define MY_GLFW_CONTEXT_VERSION_MINOR 3

void APIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

class WindowRender {
   public:
    WindowRender(Camera& camera, std::string name);

    void processInput(float deltaTime);

    inline GLFWwindow* getWindow() const { return window; }
    inline void        GetScreenSize(int& width, int& height);

    virtual ~WindowRender() {
        windowCameraMap.erase(window);
        glfwDestroyWindow(window);
        glfwTerminate();
    };

    int         screenWidth;
    int         screenHeight;
    std::string windowName;
    bool        isLeftMouseButtonPressed = false;  // 是否按下左键

    static std::map<GLFWwindow*, Camera*> windowCameraMap;  // 静态映射表,用于回调函数

   private:
    GLFWwindow* window;
    Camera*     camera;

    GLFWwindow* InitWindow();
};

/**
 * 构造函数，会直接创建一个帧缓冲
 * @param width 帧缓冲的宽
 * @param height 帧缓存的高
 */
class FrameBuffer {
   public:
    /**
     * 构造函数，会直接创建一个帧缓冲
     * @param width 帧缓冲的宽
     * @param height 帧缓存的高
     */
    FrameBuffer(const uint32_t width, const uint32_t height);
    ~FrameBuffer();
    void            Resize(const uint32_t width, const uint32_t height);
    inline uint32_t GetWidth() const { return m_Width; }
    inline uint32_t GetHeight() const { return m_Height; }
    inline uint32_t GetID() const { return m_FrameBufferID; }
    inline uint32_t GetColorAttachment() const { return m_ColorAttachment; }
    inline uint32_t GetDepthAttachment() const { return m_DepthAttachment; }

    void      Bind();
    void      UnBind();
    glm::vec3 ReadPixel(const unsigned int x, const unsigned int y);

   private:
    uint32_t m_FrameBufferID, m_ColorAttachment, m_DepthAttachment;
    uint32_t p_FBO;
    uint32_t m_Width, m_Height;
};

#endif