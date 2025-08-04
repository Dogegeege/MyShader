#include "UIrender.h"

void UIRender::HideTabBar(bool isHideTarbar) {
    ImGuiWindowClass window_class;
    window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton;
    if (isHideTarbar == true) window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
    ImGui::SetNextWindowClass(&window_class);
}

void UIRender::MainRender() {
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

            if (ImGui::MenuItem("标志:自动隐藏选项卡栏(AutoHideTabBar)", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) {
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
             *p_open = false;
             */

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

    // Begin()后,实时更新的变量
    InputInfo::GetInstance()->UnFocusOnRenderWindow();

    /**添加自己的窗口**/
    ViewPortRender();
    ShowTreeView();
    ShowObectView();
    ShowMainView();
    ShowModelView();

    ImGui::End();
}

void UIRender::ViewPortRender() {
    HideTabBar();
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
    ImGui::Begin("ViewPort");
    //! 注意 获取位置 ImGui::Begin("ViewPort") 之后，ImGui::Image 之前
    viewPortSize   = ImGui::GetContentRegionAvail();  // 获取ViewPort窗口的大小
    imageScreenPos = ImGui::GetCursorScreenPos();     // Image区域左上角屏幕坐标
    mousePos       = ImGui::GetMousePos();
    relativePos    = ImVec2(mousePos.x - imageScreenPos.x, mousePos.y - imageScreenPos.y);

    InitViewPort();

    if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) == true) { InputInfo::GetInstance()->FocusOnRenderWindow(); }

    unsigned int textureID = pFrameBuffer->GetColorAttachment();  // 获取颜色纹理的ID
    ImGui::Image(textureID, viewPortSize, {0, 1}, {1, 0});

    ProcessPicking();  // 处理拾取逻辑

    ImGui::End();
}

void UIRender::ShowObectView() {
    if (selectedObject == nullptr) { return; }
    HideTabBar();
    std::string node_name = "物体 " + selectedObject->GetName();
    ImGui::Begin(node_name.c_str(), nullptr, ImGuiWindowFlags_NoFocusOnAppearing);
    if (ImGui::TreeNodeEx("变换", node_flags_outer)) {
        ImGui::SeparatorText("位置 ");

        ImGui::Text("平移");
        ImGui::SameLine();
        ImGui::DragFloat3("##TRANSLATE", &selectedObject->translate.x, 0.1f, -999.0f, 999.0f, "%.3f");

        ImGui::Text("旋转");
        ImGui::SameLine();
        ImGui::DragFloat3("##ROTATE", &selectedObject->rotate.x, 0.1f, -999.0f, 999.0f, "%.3f");

        ImGui::Text("缩放");
        ImGui::SameLine();
        ImGui::DragFloat("##SCALE", &selectedObject->scale, 0.01f, 0.001f, 100.0f, "%.3f");

        ImGui::TreePop();
    }
    if (ImGui::TreeNodeEx("Other", node_flags_inner)) {
        ImGui::Checkbox("Z-buffer预览", &isZBufferPreview);
        ImGui::Checkbox("天空盒预览", &isSkyboxPreview);
        ImGui::Checkbox("高亮边框", &showHightLight);

        ImGui::TreePop();
    }

    ImGui::End();
}

void UIRender::ShowTreeView() {
    HideTabBar();
    ImGui::Begin("导航窗口");
    if (ImGui::TreeNodeEx("功能选项", node_flags_outer)) {
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
    ImGui::End();
}

void UIRender::ShowMainView() {
    HideTabBar(true);

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

void UIRender::ShowPageView0() {
    ImGui::Text("调试参数:%d , %d", FirstIdx, SecondIdx);

    ImGui::Text("渲染窗口大小 %.3f %.3f ", viewPortSize.x, viewPortSize.y);
    ImGui::Text("渲染窗口左上角坐标 %.3f %.3f ", imageScreenPos.x, imageScreenPos.y);

    ImGui::Text("鼠标坐标 %.3f %.3f ", mousePos.x, mousePos.y);
    ImGui::Text("相对坐标 %.3f %.3f ", relativePos.x, relativePos.y);
    ImGui::Text("鼠标当前ID %d , %d", clickedID, selectedID);

    // // 一个表格示例

    // static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable
    // |
    //                                ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

    // if (ImGui::BeginTable("table0", 3, flags)) {
    //     ImGui::TableSetupColumn("AAA", ImGuiTableColumnFlags_WidthFixed);
    //     ImGui::TableSetupColumn("BBB", ImGuiTableColumnFlags_WidthFixed);
    //     ImGui::TableSetupColumn("CCC", ImGuiTableColumnFlags_WidthStretch);
    //     ImGui::TableHeadersRow();

    //     for (int row = 0; row < 5; row++) {
    //         ImGui::TableNextRow();

    //         for (int column = 0; column < 3; column++) {
    //             ImGui::TableSetColumnIndex(column);
    //             ImGui::Text("%s %d,%d", (column == 2) ? "Stretch" : "Fixed", column, row);
    //         }
    //     }
    //     ImGui::EndTable();
    // }
}

void UIRender::ShowPageView1() {
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

void UIRender::ShowModelView() {
    // HideTabBar();
    ImGui::Begin("场景集合");

    if (ImGui::TreeNodeEx("模型", node_flags_outer)) {
        for (auto [i, j] : Object3D::loadedObject3D) {
            ImGuiTreeNodeFlags node_flags = node_flags_selected;
            if (i == selectedID) node_flags |= ImGuiTreeNodeFlags_Selected;
            bool node_open = ImGui::TreeNodeEx(j->GetName().c_str(), node_flags);

            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) { selectedID = i; }
            if (node_open == true) { ImGui::TreePop(); }
        }
        ImGui::TreePop();
    }

    ImGui::End();
}

/**
 * @brief `Imgui`循环体内渲染
 */
void UIRender::Render() {
    // 开始新的一帧
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();

    // 主窗口接收参数标志位
    window_flags |= ImGuiWindowFlags_MenuBar;
    window_flags |= ImGuiWindowFlags_NoDocking;

    // 停靠窗口接收参数标志位
    dockspace_flags |= ImGuiDockNodeFlags_None;
    dockspace_flags |= ImGuiDockNodeFlags_PassthruCentralNode;
    // dockspace_flags |= ImGuiDockNodeFlags_AutoHideTabBar;

    node_flags_inner    = static_cast<ImGuiTreeNodeFlags>(ImGuiTreeNodeFlags_DrawLinesFull);
    node_flags_outer    = static_cast<ImGuiTreeNodeFlags>(ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DrawLinesFull);
    node_flags_selected = static_cast<ImGuiTreeNodeFlags>(ImGuiTreeNodeFlags_SpanAvailWidth);
    this->MainRender();
    //   ImGui::ShowAboutWindow();
    ImGui::ShowDebugLogWindow();
    ImGui::ShowDemoWindow();

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

void UIRender::UIInit() {
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
    ImFont* font = io->Fonts->AddFontFromFileTTF("C:\\\\Windows\\\\Fonts\\\\STZHONGS.TTF", 30.f, nullptr, io->Fonts->GetGlyphRangesChineseFull());

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

void UIRender::InitViewPort() {
    // ViewPort窗口大小如果有改变，需要重置帧缓冲区
    if (viewPortSize.x * viewPortSize.y > 0 && (viewPortSize.x != pFrameBuffer->GetWidth() || viewPortSize.y != pFrameBuffer->GetHeight())) {
        pFrameBuffer->Resize(viewPortSize.x, viewPortSize.y);
        framebuffer_size_callback(windowRender.getWindow(), pFrameBuffer->GetWidth(), pFrameBuffer->GetHeight());
    }
}

void UIRender::ProcessPicking() {
    // OpenGL坐标Y轴翻转
    unsigned int px                                   = static_cast<unsigned int>(relativePos.x);
    unsigned int py                                   = pFrameBuffer->GetHeight() - static_cast<unsigned int>(relativePos.y) - 1;
    glm::uvec3   id                                   = pFrameBuffer->ReadPixel(px, py);
    clickedID                                         = id.x;
    // 操作类型（平移/旋转/缩放）
    static ImGuizmo::OPERATION mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
    static ImGuizmo::MODE      mCurrentGizmoMode      = ImGuizmo::WORLD;

    *view       = camera.GetViewMatrix();
    *projection = camera.GetProjectionMatrix();

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGuizmo::IsOver() == false &&
        ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) == true &&
        ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows) == true) {
        selectedID = clickedID;
    }
    selectedObject = Object3D::GetObject3D(selectedID);

    if (selectedObject == nullptr || selectedID == 0) { return; }
    glm::mat4& model     = selectedObject->modelMatrix;
    glm::vec3& translate = selectedObject->translate;
    glm::vec3& rotate    = selectedObject->rotate;  // 角度制
    float&     scale     = selectedObject->scale;
    ImGuizmo::SetDrawlist();
    ImGuizmo::SetRect(imageScreenPos.x, imageScreenPos.y, viewPortSize.x, viewPortSize.y);
    ImGuizmo::Manipulate(glm::value_ptr(*this->view), glm::value_ptr(*this->projection), mCurrentGizmoOperation, mCurrentGizmoMode,
                         glm::value_ptr(model));

    // 只在 ImGuizmo 没有被操作时，用 UI 控件重建 model
    if (ImGuizmo::IsUsing() == true) {
        glm::vec3 scaleX;
        ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(model), &translate.x, &rotate.x, &scaleX.x);

        scale = scaleX.x;  // 假设均匀缩放
    } else {
        model           = glm::translate(glm::mat4(1.0f), translate);
        model           = glm::scale(model, glm::vec3(scale, scale, scale));
        glm::quat quatX = glm::angleAxis(glm::radians(rotate.x), glm::vec3(1, 0, 0));
        glm::quat quatY = glm::angleAxis(glm::radians(rotate.y), glm::vec3(0, 1, 0));
        glm::quat quatZ = glm::angleAxis(glm::radians(rotate.z), glm::vec3(0, 0, 1));
        model *= glm::mat4_cast(quatZ * quatY * quatX);
    }
}
