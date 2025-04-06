#pragma once

#include <imgui.h>

namespace app {

class application {
   public:
    application(bool drawLight, int cnt, float lightPose, ImVec4 lightColor)
        : drawLight(drawLight), cnt(cnt), lightPose(lightPose), lightColor(lightColor) {}

    // Render 函数：负责绘制 ImGui 界面
    inline void Render() {
        // 开始一个新的 ImGui 窗口
        ImGui::Begin("Hello,world");
        // 获取当前窗口的大小
        // ImVec2 windowSize = ImGui::GetIO().DisplaySize;

        // 设置窗口的大小和位置
        // ImGui::SetWindowSize(windowSize);
        // ImGui::SetWindowPos(ImVec2(0, 0));

        // 显示一段简单的文本
        ImGui::Text("This is a minimal ImGui application.");
        ImGui::Checkbox("DrawLigt", &drawLight);  // 显示一个复选框
        ImGui::SliderFloat("LightPosition", &lightPose, 0.0f, 5.0f);
        ImGui::ColorEdit3("LightColor", (float*)&lightColor);  // 显示一个颜色编辑器

        if (ImGui::Button("TestButton")) cnt++;
        ImGui::SameLine();  // 显示在同一行

        // 结束当前窗口
        ImGui::End();
    }

    bool   drawLight;
    int    cnt;
    float  lightPose;
    ImVec4 lightColor;
};

inline void init() {}

}  // namespace app
