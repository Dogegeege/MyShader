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
        view       = new glm::mat4(camera.GetViewMatrix());
        projection = new glm::mat4(glm::perspective(glm::radians(camera.zoom), camera.aspectRatio, 0.1f, 100.0f));
        UIInit();
    }
    UIRender(WindowRender* windowRender, Camera& camera, FrameBuffer* pFrameBuffer) : UIRender(*windowRender, camera, pFrameBuffer) {}
    ~UIRender() {
        if (ImGui::GetCurrentContext()) {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }
        // delete model;
        // delete view;
        // delete projection;
    }

    void HideTabBar(bool isHideTarbar = false);  // 隐藏窗口的TabBar
    void MainRender();
    void ViewPortRender();
    void ShowTreeView();  // 导航页面
    void ShowObectView();
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
    unsigned int clickedID  = 0;
    unsigned int selectedID = 0;  // 0 表示未选中

    glm::mat4*                view;
    glm::mat4*                projection;
    std::shared_ptr<Object3D> selectedObject = nullptr;  // 当前选中的物体

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
    void UIInit();
    void InitViewPort();
    void ProcessPicking();

    WindowRender& windowRender;
    Camera&       camera;
    FrameBuffer*  pFrameBuffer;
    ImGuiIO*      io;
};

#endif