#ifndef UIRENDER_H
#define UIRENDER_H

#include <memory>

#include <glad/glad.h>

#include <ImGuizmo.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>
#include <imgui_internal.h>

#include <GLFW/glfw3.h>

#include "model.h"
#include "windowrender.h"

class UIRender {
   public:
    UIRender(WindowRender& windowRender, Camera& camera, FrameBuffer* pFrameBuffer)
        : windowRender(windowRender), camera(camera), pFrameBuffer(pFrameBuffer) {
        model      = new glm::mat4(glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f)));
        view       = new glm::mat4(camera.GetViewMatrix());
        projection = new glm::mat4(glm::perspective(glm::radians(camera.zoom), camera.aspectRatio, 0.1f, 100.0f));
        UIInit();
    }
    UIRender(WindowRender* windowRender, Camera& camera, FrameBuffer* pFrameBuffer) : UIRender(*windowRender, camera, pFrameBuffer) {}
    ~UIRender() {
        if (ImGui::GetCurrentContext() == true) {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }
        delete model;
        delete view;
        delete projection;
    }

    void HideTabBar(bool isHideTarbar = false);  // 隐藏窗口的TabBar
    void MainRender();
    void ViewPortRender();
    void ShowTreeView();  // 导航页面
    void ShowMainView();  // 内容页面
    void ShowPageView0();
    void ShowPageView1();
    void ShowPageView2() { ImGui::Text("功能%d -> 按钮%d -> 页面2", FirstIdx, SecondIdx); }
    void ShowPageView3() { ImGui::Text("功能%d -> 按钮%d -> 页面3", FirstIdx, SecondIdx); }
    void ShowPageView4() { ImGui::Text("功能%d -> 按钮%d -> 页面4", FirstIdx, SecondIdx); }
    void ShowModelView();

    void Render();

    ImVec2       mousePos;
    ImVec2       viewPortSize;
    ImVec2       imageScreenPos;
    ImVec2       relativePos;
    unsigned int nowID = 0;

    glm::mat4* model;
    glm::mat4* view;
    glm::mat4* projection;
    glm::vec3  translate = glm::vec3(0.0f);
    glm::vec3  rotate    = glm::vec3(0.0f);  // 角度制
    float      scale     = 1.0f;

    bool isZBufferPreview = false;
    bool isSkyboxPreview  = true;
    bool showHightLight   = true;

    int FirstIdx  = 0;  // 一级索引
    int SecondIdx = 0;  // 二级索引

    bool                p_open              = true;                     // *使用窗口关闭功能
    bool                opt_fullscreen      = true;                     // 控制窗口是否全屏
    bool                opt_padding         = false;                    // 控制窗口内边距是否启用(与边框保持一定空白区域)
    ImGuiDockNodeFlags  dockspace_flags     = ImGuiDockNodeFlags_None;  // 停靠窗口接收参数标志位
    ImGuiWindowFlags    window_flags        = ImGuiWindowFlags_None;    // 主窗口接收参数标志位
    ImGuiTreeNodeFlags_ node_flags_inner    = ImGuiTreeNodeFlags_None;
    ImGuiTreeNodeFlags_ node_flags_outer    = ImGuiTreeNodeFlags_None;
    ImGuiTreeNodeFlags_ node_flags_selected = ImGuiTreeNodeFlags_None;

   private:
    // 全局初始化 ImGui
    void UIInit() {
        IMGUI_CHECKVERSION();    // 检查 ImGui 版本
        ImGui::CreateContext();  // 创建 ImGui 上下文
        io = &ImGui::GetIO();
        (void)io;  // 获取 IO 对象

        io->FontGlobalScale = 0.8f;                                   // 设置字体缩放比例
        io->ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;      // 启用 DPI 缩放字体
        io->ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports;  // 启用 DPI 缩放视口

        // 这里是一些ImGui的拓展的设置
        io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
        io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
        io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // 启用窗口停靠
        io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;    // 多窗口渲染
        io->ConfigViewportsNoAutoMerge   = true;                //?禁用窗口合并
        io->ConfigViewportsNoTaskBarIcon = false;               // 启用后，所有子视口窗口将不显示独立的任务栏图标
        // 中文的设置，记得将main.cpp的文件编码类型改为UTF-8
        ImFont* font = io->Fonts->AddFontFromFileTTF("C:\\\\Windows\\\\Fonts\\\\msyh.ttc", 30.f, nullptr, io->Fonts->GetGlyphRangesChineseFull());

        // 判断字体加载成功
        IM_ASSERT(font != nullptr);

        ImGuiStyle& style = ImGui::GetStyle();
        if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            // 直角边框
            style.WindowRounding              = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        ImGui::StyleColorsClassic();                                         // 设置暗色主题
        ImGui_ImplGlfw_InitForOpenGL(this->windowRender.getWindow(), true);  // 初始化 GLFW 后端
        ImGui_ImplOpenGL3_Init("#version 330");                              // 初始化 OpenGL3 后端
    }

    WindowRender& windowRender;
    Camera&       camera;
    FrameBuffer*  pFrameBuffer;
    ImGuiIO*      io;
};

#endif