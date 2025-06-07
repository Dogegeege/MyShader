#ifndef INPUT_MANAGEMENT_H
#define INPUT_MANAGEMENT_H

#include "camera.h"
#include "singleton_util.h"

class InputInfo : public Singleton<InputInfo> {
   public:
    InputInfo() {
        mouse_pos_x          = 0;
        mouse_pos_y          = 0;
        mouse_first_movement = true;
        mouse_sensitivity    = SENSITIVITY;
        mouse_lastpos_x      = SCR_WIDTH / 2.0;
        mouse_lastpos_y      = SCR_HEIGHT / 2.0;
    }
    void Update() {
        if (mouse_first_movement == true) {
            mouse_lastpos_x      = mouse_pos_x;
            mouse_lastpos_y      = mouse_pos_y;
            mouse_first_movement = false;  // 重置鼠标状态
        }
        mouse_offset_x = mouse_pos_x - mouse_lastpos_x;
        mouse_offset_y = mouse_lastpos_y - mouse_pos_y;

        mouse_lastpos_x = mouse_pos_x;
        mouse_lastpos_y = mouse_pos_y;
    }
    void TimeUpdate(const float& currentTime = 0.0f) {
        float currentFrame = currentTime;
        deltaTime          = currentFrame - lastFrame;
        lastFrame          = currentFrame;
    }
    float mouse_pos_x;
    float mouse_pos_y;

    float mouse_offset_x;
    float mouse_offset_y;
    float mouse_wheel_x;
    float mouse_wheel_y;  // 鼠标滚轮滚动偏移量

    float mouse_sensitivity;

    bool mouse_first_movement;

    bool mouse_button_left;
    bool mouse_button_middle;
    bool mouse_button_right;

    // timing
    float deltaTime = 0.0f;  // time between current frame and last frame

   private:
    static InputInfo* inputInfo_instance;
    float             mouse_lastpos_x;
    float             mouse_lastpos_y;

    float lastFrame = 0.0f;
};
namespace Input {
static void ProcessInputKeyBorard(GLFWwindow* window, Camera& camera, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);  // 按ESC退出
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) camera.ProcessKeyboard(DOWN, deltaTime);
}

static void ProcessInputMouseWheel(GLFWwindow* window, Camera& camera) {
    if (InputInfo::GetInstance()->mouse_button_middle == true) camera.ProcessMouseScroll(InputInfo::GetInstance()->mouse_wheel_y);
}

static void ProcessMouseMovement(GLFWwindow* window, Camera& camera) {
    if (InputInfo::GetInstance()->mouse_button_right == true) {
        InputInfo::GetInstance()->Update();  // 更新鼠标状态
        camera.ProcessMouseMovement(InputInfo::GetInstance()->mouse_offset_x * InputInfo::GetInstance()->mouse_sensitivity,
                                    InputInfo::GetInstance()->mouse_offset_y * InputInfo::GetInstance()->mouse_sensitivity);
    }
}
}  // namespace Input

#endif  // INPUT_MANAGEMENT_H