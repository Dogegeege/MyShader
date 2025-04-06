#include <math.h>
#include <iostream>
#include <memory>
#include <vector>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

#include "camera.h"
#include "glInit.h"
#include "model.h"
#include "shader.h"
#include "texure.h"
#include "vertexShaderLoader.h"

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

    //---------------------------------顶点数据----------------------------------

    std::shared_ptr<Shader> cubeShader     = std::make_shared<Shader>("../src/shader/cube.vsh", "../src/shader/cube.fsh");
    std::shared_ptr<Shader> lightingShader = std::make_shared<Shader>("../src/shader/light.vsh", "../src/shader/light.fsh");
    // std::shared_ptr<Shader> modelShader    = std::make_shared<Shader>("../src/shader/model.vsh", "../src/shader/model.fsh");

    // Model neuro = Model("../model/vtuber-neuro-sama-v3/source/_unique_vertices.csv",
    //                     "../model/vtuber-neuro-sama-v3/source/_indices.csv",  // 导入体模型
    //                     "../model/vtuber-neuro-sama-v3/source/_prop.csv", 4);
    Model cube = Model();

    // VertexShaderLoader modelVertex(neuro);  // nero

    VertexShaderLoader cubeVertex(cube);  // 立方体

    VertexShaderLoader ligtingVertex(cube);  // 光源

#ifndef USETEXURE
    //------------------------- 纹理-----------------------------

    Texure diffuseMap("../texure/container2.png", 0);            // 纹理单元0
    Texure specularMap("../texure/container2_specular.png", 1);  // 纹理单元1

    cubeShader->use();  // 不要忘记在设置uniform变量之前激活着色器程序！

    // 静态设置纹理
    cubeShader->setInt("material.diffuse", 0);
    cubeShader->setInt("material.specular", 1);
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

        glm::mat4 model      = glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 view       = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

#ifdef MODEL
        //--------------------------Model--------------------------------
        model = glm::mat4(0.01f);

        modelShader->use();

        modelShader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);  // 物体颜色
        modelShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);    // 发光颜色

        modelShader->setMat4("model", glm::translate(model, cubePositions[0]));
        modelShader->setMat4("view", view);
        modelShader->setMat4("projection", projection);

        modelVertex.bindAndDrawElements();
        modelVertex.UnbindVertexArray();
#endif
        //--------------------------Cube--------------------------------

        cubeShader->use();  // 使用着色器程序

        // 摄像机属性
        cubeShader->setMat4("model", model);
        cubeShader->setMat4("view", view);
        cubeShader->setMat4("projection", projection);

        // 设置材质属性
        cubeShader->setVec3("material.specular", 0.5f, 0.5f, 0.5f);
        cubeShader->setFloat("material.shininess", 32.0f);

        // 设置光源（对物体）属性
        cubeShader->setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        cubeShader->setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);  // 将光照调暗了一些以搭配场景
        cubeShader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        cubeShader->setVec3("light.position", lightPositions[0]);  // 光源位置

        cubeShader->setVec3("viewPos", camera.Position);  // 摄像机位置

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap.getTexureObject());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap.getTexureObject());

        for (int i = 0; i < 10; i++) {
            // glm::vec3 lightColor;
            // lightColor.x = sin(glfwGetTime() * 2.0f);
            // lightColor.y = sin(glfwGetTime() * 0.7f);
            // lightColor.z = sin(glfwGetTime() * 1.3f);

            // glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);    // 降低影响
            // glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);  // 很低的影响

            // cubeShader->setVec3("light.ambient", ambientColor);
            // cubeShader->setVec3("light.diffuse", diffuseColor);

            cubeShader->setMat4("model", glm::translate(model, cubePositions[i]));

            cubeVertex.bindAndDrawElements();
        }
        cubeVertex.UnbindVertexArray();

        //---------------------------Light--------------------------------

        // 构造光源
        lightingShader->use();

        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPositions[0]);
        model = glm::scale(model, glm::vec3(0.2f));  // a smaller cube

        lightingShader->setMat4("model", glm::translate(model, lightPositions[0]));  // 随便给个位置
        lightingShader->setMat4("view", view);
        lightingShader->setMat4("projection", projection);

        ligtingVertex.bindVertexArray();
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // 交换缓冲
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    VertexShaderLoader::unBindBuffer();
    glfwTerminate();
    return 0;
}