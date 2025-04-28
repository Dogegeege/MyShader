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
    //!---------------------------------顶点数据----------------------------------

    // std::shared_ptr<Shader> cubeShader    = std::make_shared<Shader>("../../assets/shader/cube.vsh", "../../assets/shader/cube.fsh");
    // std::shared_ptr<Shader> lightingShade = std::make_shared<Shader>("../../assets/shader/light.vsh", "../../assets/shader/light.fsh");
    // std::shared_ptr<Shader> modelShader = std::make_shared<Shader>("../../assets/shader/model.vsh", "../../assets/shader/model.fsh");

    Shader modelShader("../../assets/shader/model.vsh", "../../assets/shader/model.fsh");

    // Model ourModel = Model("E:/Project/Learn_OpenGL/assets/model/backpack/backpack.obj");
    // Model ourModel = Model("E:/Project/Learn_OpenGL/assets/model/vtuber-neuro-sama-v3/Nurtwinsprevobj.obj");

    Model ourModel = Model("../../assets/model/vtuber-neuro-sama-v3/textures/Neuro-v3model-Releaseready4.2.obj");

    //!------------------------------变换---------------------------------------
    const glm::vec3 cubePositions[] = {glm::vec3(0.0f, 0.0f, 0.0f),     glm::vec3(2.0f, 5.0f, -15.0f), glm::vec3(-1.5f, -2.2f, -2.5f),
                                       glm::vec3(-3.8f, -2.0f, -12.3f), glm::vec3(2.4f, -0.4f, -3.5f), glm::vec3(-1.7f, 3.0f, -7.5f),
                                       glm::vec3(1.3f, -2.0f, -2.5f),   glm::vec3(1.5f, 2.0f, -2.5f),  glm::vec3(1.5f, 0.2f, -1.5f),
                                       glm::vec3(-1.3f, 1.0f, -1.5f)};

    glm::vec3 lightPositions(1.2f, 0.0f, 2.0f);

    glEnable(GL_DEPTH_TEST);  // 启用深度测试

    // timing
    float deltaTime = 0.0f;  // time between current frame and last frame
    float lastFrame = 0.0f;

    // model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));  // translate it down so it's at the center of the scene

    while (glfwWindowShouldClose(windowRender.getWindow()) == false) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime          = currentFrame - lastFrame;
        lastFrame          = currentFrame;

        windowRender.processInput(deltaTime);  // IO响应

        auto& bck = ui.backgroundColor;
        glClearColor(bck.x, bck.y, bck.z, bck.w);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        model = glm::translate(glm::mat4(1.0f), ui.translate);
        model = glm::scale(model, glm::vec3(ui.scale, ui.scale, ui.scale));

        glm::quat quatX = glm::angleAxis(ui.rotate.x, glm::vec3(1.0f, 0.0f, 0.0f));  // X 轴旋转四元数
        glm::quat quatY = glm::angleAxis(ui.rotate.y, glm::vec3(0.0f, 1.0f, 0.0f));  // Y 轴旋转四元数
        glm::quat quatZ = glm::angleAxis(ui.rotate.z, glm::vec3(0.0f, 0.0f, 1.0f));  // Z 轴旋转四元数

        model *= glm::mat4_cast(quatZ * quatY * quatX);
        view       = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.zoom), camera.aspectRatio, 0.1f, 100.0f);

        //!--------------------------Model--------------------------------

        modelShader.use();

        modelShader.setMat4("model", model);
        modelShader.setMat4("view", view);
        modelShader.setMat4("projection", projection);

        modelShader.setBool("isZBufferPreview", ui.isZBufferPreview);

        ourModel.ModelDraw(modelShader);
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

        //!-------------------------------imgui-----------------------------------------

        ui.RenderUI();

        //!---------------------------------------------------------------------
        // 处理事件
        glfwPollEvents();
        // if (ui.GetMainShouldClose()) { glfwSetWindowShouldClose(windowRender.getWindow(), GLFW_TRUE); }

        // 交换缓冲
        glfwSwapBuffers(windowRender.getWindow());
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    windowRender.~WindowRender();  // 显式写出
    return 0;
}