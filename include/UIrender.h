#ifndef UIRENDER_H
#define UIRENDER_H

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "windowrender.h"

class UIRender {
   public:
    UIRender(WindowRender& windowRender) : windowRender(windowRender) { UIInit(); }

    bool   drawLight       = false;
    int    cnt             = 0;
    float  lightPose       = 0.0f;
    float  lightIntesity   = 0.0f;
    ImVec4 backgroundColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    void MainRender() {
        // 开始一个新的 ImGui 窗口
        ImGui::Begin("中文显示UTF-8");
        // 获取当前窗口的大小
        // ImVec2 windowSize = ImGui::GetIO().DisplaySize;

        // 设置窗口的大小和位置
        // ImGui::SetWindowSize(windowSize);
        // ImGui::SetWindowPos(ImVec2(0, 0));

        // 显示一段简单的文本
        ImGui::Text("This is a minimal ImGui application.");
        ImGui::Checkbox("DrawLigt", &drawLight);  // 显示一个复选框
        ImGui::SliderFloat("LightPosition", &lightPose, 0.0f, 5.0f);
        ImGui::ColorEdit3("LightColor", (float*)&backgroundColor);  // 显示一个颜色编辑器

        ImGui::SliderFloat("LightIntesity", &lightIntesity, 1.0f, 5.0f);

        if (ImGui::Button("TestButton")) cnt++;
        ImGui::SameLine();  // 显示在同一行

        // 结束当前窗口
        ImGui::End();
    }

    /**
     * @brief `Imgui`循环体内渲染
     */
    void RenderUI() {
        // 开始新的一帧
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        MainRender();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());  // 渲染 ImGui 数据

        // 只需要渲染单页面内部ui不用以下设置，不过最好可以加上
        if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();  // 备份OpenGL上下文
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

   private:
    // 初始化 ImGui
    void UIInit() {
        IMGUI_CHECKVERSION();    // 检查 ImGui 版本
        ImGui::CreateContext();  // 创建 ImGui 上下文
        io = &ImGui::GetIO();
        (void)io;  // 获取 IO 对象

        // 这里是一些ImGui的拓展的设置
        io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
        io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
        io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // 启用窗口停靠
        io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;    // 多窗口渲染
        io->ConfigViewportsNoAutoMerge = false;                 //?禁用窗口合并

        // 中文的设置，记得将main.cpp的文件编码类型改为UTF-8
        ImFont* font =
            io->Fonts->AddFontFromFileTTF("C:\\\\Windows\\\\Fonts\\\\msyh.ttc", 30.f, nullptr, io->Fonts->GetGlyphRangesChineseSimplifiedCommon());

        // 必须判断一下字体有没有加载成功
        IM_ASSERT(font != nullptr);

        // ImGuiStyle& style = ImGui::GetStyle();
        // if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        //     style.WindowRounding              = 0.0f;
        //     style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        // }

        ImGui::StyleColorsLight();                                           // 设置暗色主题
        ImGui_ImplGlfw_InitForOpenGL(this->windowRender.getWindow(), true);  // 初始化 GLFW 后端
        ImGui_ImplOpenGL3_Init("#version 330");                              // 初始化 OpenGL3 后端
    }

    WindowRender& windowRender;
    ImGuiIO*      io;
};

#endif