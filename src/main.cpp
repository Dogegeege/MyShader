#include <math.h>
#include <filesystem>
#include <iostream>
#include <memory>
#include <vector>

#include "Application/application.hpp"

#include "camera.h"
#include "glInit.h"
#include "model.h"
#include "shader.h"
#include "texure.h"
#include "vertexShaderLoader.h"

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool   rightMouseButtonPressed = false;  // 鼠标左键按下状态

glm::mat4 model      = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));
glm::mat4 view       = camera.GetViewMatrix();
glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

//!---------------------------------Callbaclk----------------------------------

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // glViewport(0, 0, width, height);

    // 固定宽高比
    float aspectRatio = 16.0f / 9.0f;  // 假设宽高比为 16:9
    int   viewportWidth, viewportHeight;

    if (width / (float)height > aspectRatio) {
        viewportWidth  = static_cast<int>(height * aspectRatio);
        viewportHeight = height;
    } else {
        viewportWidth  = width;
        viewportHeight = static_cast<int>(width / aspectRatio);
    }

    int viewportX = (width - viewportWidth) / 2;
    int viewportY = (height - viewportHeight) / 2;

    glViewport(viewportX, viewportY, viewportWidth, viewportHeight);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (rightMouseButtonPressed == true) { camera.ProcessMouseMovement(xpos, ypos); }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            rightMouseButtonPressed = true;  // 鼠标左键按下
        } else if (action == GLFW_RELEASE) {
            rightMouseButtonPressed = false;  // 鼠标左键释放
            camera.firstMouse       = true;   // 重置鼠标状态
        }
    }
}

//!-----------------------------------------------------------------

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
    glfwSetScrollCallback(window, scroll_callback);                     // 鼠标滚轮参数
    glfwSetMouseButtonCallback(window, mouse_button_callback);          // 注册鼠标按键回调
    glfwSetCursorPosCallback(window, mouse_callback);                   // 注册鼠标移动回调
    // 启用鼠标监听
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  // 隐藏鼠标

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

    ImGui::StyleColorsDark();                    // 设置暗色主题
    ImGui_ImplGlfw_InitForOpenGL(window, true);  // 初始化 GLFW 后端
    ImGui_ImplOpenGL3_Init("#version 330");      // 初始化 OpenGL3 后端

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

    while (glfwWindowShouldClose(window) == false) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime          = currentFrame - lastFrame;
        lastFrame          = currentFrame;

        processInput(window, deltaTime);  // IO响应

        auto& bck = appinfo.backgroundColor;
        glClearColor(bck.x, bck.y, bck.z, bck.w);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        view = camera.GetViewMatrix();

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
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}