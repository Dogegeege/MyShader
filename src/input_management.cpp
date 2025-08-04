#include "input_management.h"

void Input::ProcessInputKeyBorard(GLFWwindow* window, Camera* camera, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);  // 按ESC退出

    if (InputInfo::GetInstance()->IsFocusOnRenderWindow() == false) { return; }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera->ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) camera->ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) camera->ProcessKeyboard(DOWN, deltaTime);
}

void Input::ProcessInputMouseWheel(GLFWwindow* window, Camera* camera, float yoffset) {
    if (InputInfo::GetInstance()->IsFocusOnRenderWindow() == false) { return; }

    InputInfo::GetInstance()->mouse_wheel_y = static_cast<float>(yoffset);
    if (InputInfo::GetInstance()->mouse_button_middle == true) camera->ProcessMouseScroll(InputInfo::GetInstance()->mouse_wheel_y);
}

void Input::ProcessInputMouseMovement(GLFWwindow* window, Camera* camera, double xpos, double ypos) {
    if (InputInfo::GetInstance()->IsFocusOnRenderWindow() == false) { return; }

    InputInfo::GetInstance()->mouse_pos_x = static_cast<float>(xpos);
    InputInfo::GetInstance()->mouse_pos_y = static_cast<float>(ypos);
    if (InputInfo::GetInstance()->mouse_button_right == true) {
        InputInfo::GetInstance()->Update();  // 更新鼠标状态
        camera->ProcessMouseMovement(InputInfo::GetInstance()->mouse_offset_x * InputInfo::GetInstance()->mouse_sensitivity,
                                     InputInfo::GetInstance()->mouse_offset_y * InputInfo::GetInstance()->mouse_sensitivity);
    }
}

void Input::ProcessInputMouseButton(GLFWwindow* window, int button, int action, int mods) {
    if (InputInfo::GetInstance()->IsFocusOnRenderWindow() == false) { return; }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        InputInfo::GetInstance()->mouse_button_right = true;
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        InputInfo::GetInstance()->mouse_button_right   = false;
        InputInfo::GetInstance()->mouse_first_movement = true;  // 鼠标第一次移动
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        InputInfo::GetInstance()->mouse_button_left = true;
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        InputInfo::GetInstance()->mouse_button_left = false;
    }
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
        InputInfo::GetInstance()->mouse_button_middle = true;
    } else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE) {
        InputInfo::GetInstance()->mouse_button_middle = false;
    }
}