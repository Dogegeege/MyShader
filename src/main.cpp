#include <math.h>
#include <iostream>
#include <memory>
#include <vector>

#include "camera.h"
#include "glInit.h"
#include "model.h"
#include "shader.h"
#include "texure.h"
#include "trans.h"
#include "vertex.h"

// 调试回调函数
void APIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    std::cerr << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "") << " type = " << type << ", severity = " << severity
              << ", message = " << message << std::endl;
}

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    camera.ProcessMouseMovement(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void processInput(GLFWwindow* window, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);  // 按ESC退出

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) camera.ProcessKeyboard(DOWN, deltaTime);
}

int main() {
    auto windowopt = glInit(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL");  // 初始化GLFW窗口
    if (windowopt.has_value() == false) { return -1; }

    auto window = windowopt.value();  // 获取窗口

    // 注册回调参数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  // 调整窗口
    glfwSetCursorPosCallback(window, mouse_callback);                   // 鼠标移动参数
    glfwSetScrollCallback(window, scroll_callback);                     // 鼠标滚轮参数

    // 启用鼠标监听
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  // 隐藏鼠标

#ifdef DEBUG
    #if MY_GLFW_CONTEXT_VERSION_MAJOR >= 4 && MY_GLFW_CONTEXT_VERSION_MINOR >= 3
    // 启用调试输出
    std::cerr << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    glDebugMessageCallback(MessageCallback, nullptr);
    #endif
#endif

    //---------------------------------顶点数据----------------------------------

    std::shared_ptr<Shader> cubeShader     = std::make_shared<Shader>("../src/shader/cube.vs", "../src/shader/cube.fs");
    std::shared_ptr<Shader> lightingShader = std::make_shared<Shader>("../src/shader/light.vs", "../src/shader/light.fs");

    // 立方体的八个顶点(实际每个面都有2个独立的三角形)

    modelInit();

    vertex cubeVertex(vertices, indices, prop);  // 立方体

    vertex ligtingVertex(vertices, indices, prop);  // 光源

#ifdef USETEXURE
    //------------------------- 纹理-----------------------------

    texure container("../texure/container.jpg", GL_TEXTURE0);  // 纹理单元0

    texure awesomef ace("../texure/awesomeface.png", GL_TEXTURE1, true);  // 纹理单元1

    // myShader->use();                                                 // 不要忘记在设置uniform变量之前激活着色器程序！
    // glUniform1i(glGetUniformLocation(myShader->ID, "texture1"), 0);  // 手动设置
    // myShader->setInt("texture2", 1);                                 // 或者使用着色器类设置

#endif

    //------------------------------变换---------------------------------------
    const glm::vec3 cubePositions[] = {glm::vec3(0.0f, 0.0f, 0.0f),     glm::vec3(2.0f, 5.0f, -15.0f), glm::vec3(-1.5f, -2.2f, -2.5f),
                                       glm::vec3(-3.8f, -2.0f, -12.3f), glm::vec3(2.4f, -0.4f, -3.5f), glm::vec3(-1.7f, 3.0f, -7.5f),
                                       glm::vec3(1.3f, -2.0f, -2.5f),   glm::vec3(1.5f, 2.0f, -2.5f),  glm::vec3(1.5f, 0.2f, -1.5f),
                                       glm::vec3(-1.3f, 1.0f, -1.5f)};

    const glm::vec3 lightPositions[] = {
        glm::vec3(1.2f, 0.0f, 2.0f),
    };

    glEnable(GL_DEPTH_TEST);  // 启用深度测试

    // timing
    float deltaTime = 0.0f;  // time between current frame and last frame
    float lastFrame = 0.0f;

    while (glfwWindowShouldClose(window) == false) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime          = currentFrame - lastFrame;
        lastFrame          = currentFrame;

        processInput(window, deltaTime);  // IO响应

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cubeShader->use();  // 使用着色器程序

        glm::mat4 model      = glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 view       = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        cubeVertex.bindVertexArray();

        cubeShader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);  // 物体颜色
        cubeShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);    // 发光颜色
        cubeShader->setVec3("lightPos", lightPositions[0]);     // 光源位置
        cubeShader->setVec3("viewPos", camera.Position);        // 摄像机位置

        for (int i = 0; i < 10; i++) {
            // 混合值
            float timeValue = glfwGetTime();
            float mixValue  = 2.f * static_cast<float>(sin(timeValue) / 2.0f);

            // cubeShader->setFloat("mixValue", mixValue);

            cubeShader->setMat4("model", glm::translate(model, cubePositions[i] + mixValue));
            cubeShader->setMat4("view", view);
            cubeShader->setMat4("projection", projection);

            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);  // 绘制的模式(三角形),(绘制顶点的个数)
        }
        glBindVertexArray(0);

        // 构造光源
        lightingShader->use();

        model      = glm::mat4(1.0f);
        view       = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        lightingShader->setMat4("model", glm::translate(model, lightPositions[0]));  // 随便给个位置
        lightingShader->setMat4("view", view);
        lightingShader->setMat4("projection", projection);

        ligtingVertex.bindVertexArray();
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // 交换缓冲
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    vertex::deleteBuffer();
    glfwTerminate();
    return 0;
}