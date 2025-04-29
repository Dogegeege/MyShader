#include <math.h>
#include <filesystem>
#include <iostream>
#include <memory>
#include <vector>

#include "UIrender.h"

#include "camera.h"
#include "model.h"
#include "shader.h"
#include "texture.h"
#include "windowrender.h"

Camera       camera(glm::vec3(0.0f, 0.0f, 3.0f));
WindowRender windowRender(camera, "LearnOpenGL");

glm::mat4 model      = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));
glm::mat4 view       = camera.GetViewMatrix();
glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), camera.aspectRatio, 0.1f, 100.0f);

//!-----------------------------------------------------------------

int main() {
    UIRender ui(windowRender);

    Shader modelShader("../../assets/shader/model.vsh", "../../assets/shader/model.fsh");
    Shader highLightContour("../../assets/shader/highlightcontour.vsh", "../../assets/shader/highlightcontour.fsh");

    Model ourModel = Model("../../assets/model/vtuber-neuro-sama-v3/textures/Neuro-v3model-Releaseready4.2.obj");

    //!------------------------------变换---------------------------------------
    // const glm::vec3 cubePositions[] = {glm::vec3(0.0f, 0.0f, 0.0f),     glm::vec3(2.0f, 5.0f, -15.0f), glm::vec3(-1.5f, -2.2f, -2.5f),
    //                                    glm::vec3(-3.8f, -2.0f, -12.3f), glm::vec3(2.4f, -0.4f, -3.5f), glm::vec3(-1.7f, 3.0f, -7.5f),
    //                                    glm::vec3(1.3f, -2.0f, -2.5f),   glm::vec3(1.5f, 2.0f, -2.5f),  glm::vec3(1.5f, 0.2f, -1.5f),
    //                                    glm::vec3(-1.3f, 1.0f, -1.5f)};

    // glm::vec3 lightPositions(1.2f, 0.0f, 2.0f);

    glEnable(GL_DEPTH_TEST);  // 启用深度测试
    glDepthFunc(GL_LESS);

    glEnable(GL_STENCIL_TEST);  // 启用模板测试
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);

    // timing
    float deltaTime = 0.0f;  // time between current frame and last frame
    float lastFrame = 0.0f;

    while (glfwWindowShouldClose(windowRender.getWindow()) == false) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime          = currentFrame - lastFrame;
        lastFrame          = currentFrame;

        windowRender.processInput(deltaTime);  // IO响应

        glClearColor(63.0f / 255.0f, 63.0f / 255.0f, 63.0f / 255.0f, 1);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        //!--------------------------Shader--------------------------------

        model = glm::translate(glm::mat4(1.0f), ui.translate);
        model = glm::scale(model, glm::vec3(ui.scale, ui.scale, ui.scale));

        glm::quat quatX = glm::angleAxis(ui.rotate.x, glm::vec3(1.0f, 0.0f, 0.0f));  // X 轴旋转四元数
        glm::quat quatY = glm::angleAxis(ui.rotate.y, glm::vec3(0.0f, 1.0f, 0.0f));  // Y 轴旋转四元数
        glm::quat quatZ = glm::angleAxis(ui.rotate.z, glm::vec3(0.0f, 0.0f, 1.0f));  // Z 轴旋转四元数

        model *= glm::mat4_cast(quatZ * quatY * quatX);
        view       = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.zoom), camera.aspectRatio, 0.1f, 100.0f);

        modelShader.use();
        modelShader.setMat4("model", model);
        modelShader.setMat4("view", view);
        modelShader.setMat4("projection", projection);

        modelShader.setBool("isZBufferPreview", ui.isZBufferPreview);

        highLightContour.use();
        highLightContour.setMat4("model", model);
        highLightContour.setMat4("view", view);
        highLightContour.setMat4("projection", projection);

        //!--------------------------Model--------------------------------

        // 内层模型
        modelShader.use();

        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);  // 每一位都可以被修改，即启用模板缓冲写入

        ourModel.ModelDraw(modelShader);

        // 外层轮廓
        //?如果不禁用深度测试,则外层轮廓呈现透视状态
        glDisable(GL_DEPTH_TEST);
        highLightContour.use();
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);  // 禁止修改

        ourModel.ModelDraw(highLightContour);

        // *写回保证下一轮glClear可以清除模板缓存
        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);

        glEnable(GL_DEPTH_TEST);

#ifdef CUBESHADE

        //!--------------------------Cube--------------------------------

        // cubeShader->use();  // 使用着色器程序

        // // 摄像机属性
        // cubeShader->setMat4("model", model);
        // cubeShader->setMat4("view", view);
        // cubeShader->setMat4("projection", projection);

        // // 设置材质属性
        // cubeShader->setVec3("material.specular", 0.5f, 0.5f, 0.5f);
        // cubeShader->setFloat("material.shininess", 32.0f);

        // // 设置光源（对物体）属性
        // cubeShader->setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        // cubeShader->setVec3("light.diffuse", appinfo.lightColor);  // 将光照调暗了一些以搭配场景
        // cubeShader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        // cubeShader->setVec3("light.position", lightPositions);           // 光源位置
        // cubeShader->setFloat("light.intensity", appinfo.lightIntesity);  // 光源强度

        // cubeShader->setVec3("viewPos", camera.Position);  // 摄像机位置

        // diffuseMap.acitVeTexure();
        // specularMap.acitVeTexure();

        // for (int i = 0; i < 10; i++) {
        //     // glm::vec3 lightColor;
        //     // lightColor.x = sin(glfwGetTime() * 2.0f);
        //     // lightColor.y = sin(glfwGetTime() * 0.7f);
        //     // lightColor.z = sin(glfwGetTime() * 1.3f);

        //     // glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);    // 降低影响
        //     // glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);  // 很低的影响

        //     // cubeShader->setVec3("light.ambient", ambientColor);
        //     // cubeShader->setVec3("light.diffuse", diffuseColor);

        //     cubeShader->setMat4("model", glm::translate(model, cubePositions[i]));

        //     cubeVertex.bindAndDrawElements();
        // }
        // cubeVertex.UnbindVertexArray();

        //!---------------------------Light--------------------------------

        // // 构造光源
        // lightingShader->use();

        // float radius = 2.0f;

        // lightPositions.y = cos(appinfo.lightPose * camera.MovementSpeed) * radius;
        // lightPositions.z = sin(appinfo.lightPose * camera.MovementSpeed) * radius;

        // model = glm::mat4(1.0f);
        // model = glm::translate(model, lightPositions);
        // model = glm::scale(model, glm::vec3(0.2f));  // a smaller cube

        // lightingShader->setMat4("model", glm::translate(model, lightPositions));  // 随便给个位置
        // lightingShader->setMat4("view", view);
        // lightingShader->setMat4("projection", projection);

        // lightingShader->setVec4("lightColor", appinfo.lightColor);

        // ligtingVertex.bindAndDrawElements();
#endif

        //!-------------------------------imgui-----------------------------------------

        ui.RenderUI();

        //!---------------------------------------------------------------------
        // 处理事件
        glfwPollEvents();

        // 交换缓冲
        glfwSwapBuffers(windowRender.getWindow());
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    windowRender.~WindowRender();  // 显式写出
    return 0;
}