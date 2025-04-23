#ifndef UIRENDER_H
#define UIRENDER_H

#include <glad/glad.h>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>
#include <imgui_internal.h>

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
        if (opt_fullscreen == true) {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();

            // 全屏时，窗口尺寸和位置设为视口的工作区域，隐藏标题栏和边框，并禁用交互焦点。
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

            window_flags |= ImGuiWindowFlags_NoTitleBar;             // 移除窗口标题栏（包含关闭/折叠按钮）
            window_flags |= ImGuiWindowFlags_NoCollapse;             // 禁用窗口折叠功能（标题栏左侧的折叠按钮消失）
            window_flags |= ImGuiWindowFlags_NoResize;               // 禁止用户调整窗口大小（禁用右下角拖拽手柄）
            window_flags |= ImGuiWindowFlags_NoMove;                 // 锁定窗口位置（无法通过拖拽标题栏移动窗口）
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;  // 窗口获得焦点时不会自动置顶
            window_flags |= ImGuiWindowFlags_NoNavFocus;             // 禁止键盘导航焦点（Tab键切换控件时跳过此窗口）

        } else {
            // 非全屏时，取消中心停靠标志。
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) {  // 中心不停靠时显示背景
            window_flags |= ImGuiWindowFlags_NoBackground;
        }
        if (opt_padding == false) ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        //----------------------------------------------------------------------------------
        ImGui::Begin("主窗口", &p_open, window_flags);
        //----------------------------------------------------------------------------------

        if (opt_padding == false) ImGui::PopStyleVar();
        if (opt_fullscreen == true) ImGui::PopStyleVar(2);  // Submit the DockSpace

        if (io->ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");

            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        } else {
            // 不关闭Docking
            // ShowDockingDisabledMessage();
        }
        // ----------------------------------功能选项----------------------------------------
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("选项(Options)")) {
                ImGui::MenuItem("全屏(Fullscreen)", NULL, &opt_fullscreen);
                ImGui::MenuItem("填充(Padding)", NULL, &opt_padding);

                ImGui::Separator();
                if (ImGui::MenuItem("标志:不分割(NoSplit)", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) {
                    // 分割时允许停靠
                    dockspace_flags ^= ImGuiDockNodeFlags_NoSplit;
                }

                if (ImGui::MenuItem("标志:不调整大小(NoResize)", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) {
                    // 窗口停靠时是否可以调整大小
                    dockspace_flags ^= ImGuiDockNodeFlags_NoResize;
                }

                if (ImGui::MenuItem("标志:不停靠在中心节点(NoDockingInCentralNode)", "",
                                    (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) {
                    dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
                }

                if (ImGui::MenuItem("标志:自动隐藏选项卡栏(AutoHideTabBar)", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0, false)) {
                    // 当停靠节点内仅有一个选项卡时，自动隐藏选项卡栏；当存在多个选项卡时，自动显示选项卡栏。
                    dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
                }

                if (ImGui::MenuItem("标志:中心节点筛选器(PassthruCentralNode)", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0,
                                    opt_fullscreen)) {
                    // 禁止中心停靠
                    dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode;
                }

                // 不关闭菜单

                /* if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
                 *p_open = false;*/

                ImGui::EndMenu();
            }

            // ----------------------------------主题切换----------------------------------------
            if (ImGui::BeginMenu("主题(Other)")) {
                if (ImGui::MenuItem("暗黑(Dark)")) { ImGui::StyleColorsDark(); }
                if (ImGui::MenuItem("明亮(Light)")) { ImGui::StyleColorsLight(); }
                if (ImGui::MenuItem("经典(Classic)")) { ImGui::StyleColorsClassic(); }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // // 显示一段简单的文本
        // ImGui::Text("This is a minimal ImGui application.");
        // ImGui::Checkbox("DrawLigt", &drawLight);  // 显示一个复选框
        // ImGui::SliderFloat("LightPosition", &lightPose, 0.0f, 5.0f);
        // ImGui::ColorEdit3("LightColor", (float*)&backgroundColor);  // 显示一个颜色编辑器

        // ImGui::SliderFloat("LightIntesity", &lightIntesity, 1.0f, 5.0f);

        // if (ImGui::Button("TestButton")) cnt++;
        // ImGui::SameLine();  // 显示在同一行

        /**添加自己的窗口**/

        ShowTreeView();
        ShowMainView();

        ImGui::End();
    }

    // 隐藏窗口的TabBar
    void HideTabBar() {
        ImGuiWindowClass window_class;
        window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton;
        // window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
        ImGui::SetNextWindowClass(&window_class);
    }

    int       FirstIdx     = 0;  // 一级索引
    int       SecondIdx    = 0;  // 二级索引
    char      text[3][64]  = {"0", "0", "0"};
    char      text1[3][64] = {"0", "0", "0"};
    glm::vec3 translate    = glm::vec3(0.0f);
    glm::vec3 rotate       = glm::vec3(0.0f);  // 角度制

    // 导航页面
    void ShowTreeView() {
        HideTabBar();
        ImGui::Begin("导航窗口");
        if (ImGui::TreeNode("功能选项")) {
            for (int i = 0; i < 5; i++) {
                if (i == 0) ImGui::SetNextItemOpen(true, ImGuiCond_Once);
                if (ImGui::TreeNode((void*)(intptr_t)i, "功能 %d", i)) {
                    ImGui::Text("图标");
                    ImGui::SameLine();
                    if (ImGui::SmallButton("按钮1")) {
                        FirstIdx  = i;
                        SecondIdx = 0;
                    }
                    ImGui::Text("图标");
                    ImGui::SameLine();
                    if (ImGui::SmallButton("按钮2")) {
                        FirstIdx  = i;
                        SecondIdx = 1;
                    }
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("物体")) {
            if (ImGui::TreeNode("变换")) {
                ImGui::Text("位置 ");

                ImGui::InputText("X", text[0], IM_ARRAYSIZE(text[0]), ImGuiInputTextFlags_CharsDecimal);
                GetNumeric(text[0], translate.x);
                ImGui::InputText("Y", text[1], IM_ARRAYSIZE(text[1]), ImGuiInputTextFlags_CharsDecimal);
                GetNumeric(text[1], translate.y);
                ImGui::InputText("Z", text[2], IM_ARRAYSIZE(text[2]), ImGuiInputTextFlags_CharsDecimal);
                GetNumeric(text[2], translate.z);
                std::cout << translate.x << " " << translate.y << " " << translate.z << std::endl;

                ImGui::Text("旋转");
                ImGui::AlignTextToFramePadding();
                ImGui::InputText("X1", text1[0], IM_ARRAYSIZE(text1[0]), ImGuiInputTextFlags_CharsDecimal);
                GetNumeric(text1[0], rotate.x);
                ImGui::InputText("Y1", text1[1], IM_ARRAYSIZE(text1[1]), ImGuiInputTextFlags_CharsDecimal);
                GetNumeric(text1[1], rotate.y);
                ImGui::InputText("Z1", text1[2], IM_ARRAYSIZE(text1[2]), ImGuiInputTextFlags_CharsDecimal);
                GetNumeric(text1[2], rotate.z);
                ImGui::TreePop();
                std::cout << rotate.x << " " << rotate.y << " " << rotate.z << std::endl;
            }
            if (ImGui::TreeNode("旋转")) { ImGui::TreePop(); }
            ImGui::TreePop();
        }
        ImGui::End();
    }

    // 内容页面
    void ShowMainView() {
        HideTabBar();

        // 清除之前的内容
        ImGui::Begin("页面窗口");

        switch (FirstIdx) {
            case 0: ShowPageView0(); break;
            case 1: ShowPageView1(); break;
            case 2: ShowPageView2(); break;
            case 3: ShowPageView3(); break;
            case 4: ShowPageView4(); break;
            default: break;
        }

        ImGui::End();
    }

    void ShowPageView0() {
        ImGui::Text("功能%d -> 按钮%d -> 页面0", FirstIdx, SecondIdx);

        // 一个表格示例

        static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable |
                                       ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

        if (ImGui::BeginTable("table0", 3, flags)) {
            ImGui::TableSetupColumn("AAA", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("BBB", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("CCC", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableHeadersRow();

            for (int row = 0; row < 5; row++) {
                ImGui::TableNextRow();

                for (int column = 0; column < 3; column++) {
                    ImGui::TableSetColumnIndex(column);
                    ImGui::Text("%s %d,%d", (column == 2) ? "Stretch" : "Fixed", column, row);
                }
            }
            ImGui::EndTable();
        }
    }

    void ShowPageView1() {
        ImGui::Text("功能%d -> 按钮%d -> 页面1", FirstIdx, SecondIdx);

        // 选项卡示例
        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;

        if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags)) {
            if (ImGui::BeginTabItem("Avocado")) {
                ImGui::Text("This is the Avocado tab!\nblah blah blah blah blah");
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Broccoli")) {
                ImGui::Text("This is the Broccoli tab!\nblah blah blah blah blah");
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Cucumber")) {
                ImGui::Text("This is the Cucumber tab!\nblah blah blah blah blah");
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
    }

    void ShowPageView2() { ImGui::Text("功能%d -> 按钮%d -> 页面2", FirstIdx, SecondIdx); }
    void ShowPageView3() { ImGui::Text("功能%d -> 按钮%d -> 页面3", FirstIdx, SecondIdx); }
    void ShowPageView4() { ImGui::Text("功能%d -> 按钮%d -> 页面4", FirstIdx, SecondIdx); }

    /**
     * @brief `Imgui`循环体内渲染
     */
    void RenderUI() {
        // 开始新的一帧
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 主窗口接收参数标志位
        window_flags |= ImGuiWindowFlags_MenuBar;
        window_flags |= ImGuiWindowFlags_NoDocking;

        // 停靠窗口接收参数标志位
        dockspace_flags |= ImGuiDockNodeFlags_None;
        dockspace_flags |= ImGuiDockNodeFlags_PassthruCentralNode;
        dockspace_flags |= ImGuiDockNodeFlags_AutoHideTabBar;

        this->MainRender();
        // ImGui::ShowAboutWindow();
        // ImGui::ShowDebugLogWindow();
        // ImGui::ShowDemoWindow();

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

    bool               p_open          = true;                     // *使用窗口关闭功能
    bool               opt_fullscreen  = true;                     // 控制窗口是否全屏
    bool               opt_padding     = false;                    // 控制窗口内边距是否启用(与边框保持一定空白区域)
    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;  // 停靠窗口接收参数标志位
    ImGuiWindowFlags   window_flags    = ImGuiWindowFlags_None;    // 主窗口接收参数标志位

   private:
    // 全局初始化 ImGui
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

        ImGui::StyleColorsLight();                                           // 设置暗色主题
        ImGui_ImplGlfw_InitForOpenGL(this->windowRender.getWindow(), true);  // 初始化 GLFW 后端
        ImGui_ImplOpenGL3_Init("#version 330");                              // 初始化 OpenGL3 后端
    }

    WindowRender& windowRender;
    ImGuiIO*      io;

    static inline bool GetNumeric(const std::string& str, float& num) {
        try {
            num = std::stof(str);
            return true;  // 转换成功，返回true
        } catch (const std::invalid_argument& e) {
            // 转换失败时原参数保留
            return false;  // 转换失败，返回false
        }
    }
};

#endif