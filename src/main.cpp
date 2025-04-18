#include <math.h>
#include <filesystem>
#include <iostream>
#include <memory>
#include <vector>

#include "Application/application.hpp"

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
    // 初始化 ImGui
    IMGUI_CHECKVERSION();    // 检查 ImGui 版本
    ImGui::CreateContext();  // 创建 ImGui 上下文
    ImGuiIO& io = ImGui::GetIO();
    (void)io;  // 获取 IO 对象

    // 这里是一些ImGui的拓展的设置
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    // 中文的设置，记得将main.cpp的文件编码类型改为UTF-8
    io.Fonts->AddFontFromFileTTF("C:\\\\Windows\\\\Fonts\\\\msyh.ttc", 30.f, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());

    ImGui::StyleColorsDark();                                      // 设置暗色主题
    ImGui_ImplGlfw_InitForOpenGL(windowRender.getWindow(), true);  // 初始化 GLFW 后端
    ImGui_ImplOpenGL3_Init("#version 330");                        // 初始化 OpenGL3 后端

    //!---------------------------------顶点数据----------------------------------

    // std::shared_ptr<Shader> cubeShader    = std::make_shared<Shader>("../../assets/shader/cube.vsh", "../../assets/shader/cube.fsh");
    // std::shared_ptr<Shader> lightingShade = std::make_shared<Shader>("../../assets/shader/light.vsh", "../../assets/shader/light.fsh");
    // std::shared_ptr<Shader> modelShader = std::make_shared<Shader>("../../assets/shader/model.vsh", "../../assets/shader/model.fsh");

    Shader modelShader("../../assets/shader/model.vsh", "../../assets/shader/model.fsh");

    // Model ourModel = Model("E:/Project/Learn_OpenGL/assets/model/backpack/backpack.obj");
    // Model ourModel = Model("E:/Project/Learn_OpenGL/assets/model/vtuber-neuro-sama-v3/Nurtwinsprevobj.obj");

    Model ourModel = Model("E:/Project/Learn_OpenGL/assets/model/vtuber-neuro-sama-v3/textures/Neuro-v3model-Releaseready4.2.obj");

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

    app::application appinfo = app::application();  // 创建应用程序对象

    // model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));  // translate it down so it's at the center of the scene

    while (glfwWindowShouldClose(windowRender.getWindow()) == false) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime          = currentFrame - lastFrame;
        lastFrame          = currentFrame;

        windowRender.processInput(deltaTime);  // IO响应

        auto& bck = appinfo.backgroundColor;
        glClearColor(bck.x, bck.y, bck.z, bck.w);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        view       = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.zoom), camera.aspectRatio, 0.1f, 100.0f);

        //!--------------------------Model--------------------------------

        modelShader.use();

        modelShader.setMat4("model", model);
        modelShader.setMat4("view", view);
        modelShader.setMat4("projection", projection);

        //! 智能指针（需要改造）
        ourModel.Draw(modelShader);
        // //!--------------------------Cube--------------------------------

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

        // //!---------------------------Light--------------------------------

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

        // //!-------------------------------imgui-----------------------------------------

        // 开始新的一帧
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        appinfo.Render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());  // 渲染 ImGui 数据

        // 只需要渲染单页面内部ui不用以下设置，不过最好可以加上
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

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