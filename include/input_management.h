#ifndef INPUT_MANAGEMENT_H
#define INPUT_MANAGEMENT_H

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "_camera.h"
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
    inline void Update() {
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
    inline void TimeUpdate(const float& currentTime = 0.0f) {
        float currentFrame = currentTime;
        deltaTime          = currentFrame - lastFrame;
        lastFrame          = currentFrame;
    }
    inline bool IsFocusOnRenderWindow() { return isFocusOnRenderWindow; }
    inline void FocusOnRenderWindow() { isFocusOnRenderWindow = true; }
    inline void UnFocusOnRenderWindow() { isFocusOnRenderWindow = false; }

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

    bool isFocusOnRenderWindow = false;
};

namespace Input {
void ProcessInputKeyBorard(GLFWwindow* window, Camera* camera, float deltaTime);
void ProcessInputMouseWheel(GLFWwindow* window, Camera* camer, float yoffset);
void ProcessInputMouseMovement(GLFWwindow* window, Camera* camera, double xpos, double ypos);
void ProcessInputMouseButton(GLFWwindow* window, int button, int action, int mods);
}  // namespace Input
#endif  // INPUT_MANAGEMENT_H