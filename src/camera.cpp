#include "camera.h"

Camera::Camera(const glm::vec3& position, const glm::vec3& up)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
      MovementSpeed(SPEED),
      MouseSensitivity(SENSITIVITY),
      Zoom(ZOOM),
      lastX(SCR_WIDTH / 2.0),
      lastY(SCR_HEIGHT / 2.0),
      Yaw(YAW),
      Pitch(PITCH),
      Position(position),
      WorldUp(up) {
    firstMouse = true;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ)
    : Camera(glm::vec3(posX, posY, posZ), glm::vec3(upX, upY, upZ)) {};

/**
 * @brief 处理键盘输入
 * @param Camera_Movement 移动参数: `FORWARD`=>前, `BACKWARD`=>后, `LEFT`=>左, `RIGHT`=>右
 * @param deltaTime 前后帧间隔时间
 */
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
    float     velocity = MovementSpeed * deltaTime;
    glm::vec3 lastPos  = Position;  // 记录上次位置

    // 注意Front y方向的分量不能对移动产生影响
    if (direction == FORWARD || direction == BACKWARD) {
        glm::vec3 Front_xz = glm::normalize(glm::vec3(Front.x, 0.f, Front.z));

        if (direction == FORWARD) Position += Front_xz * velocity;   // <-- 除了上下移动，其他都限制在y=0平面内
        if (direction == BACKWARD) Position -= Front_xz * velocity;  // <-- 保证摄像机在y=0平面移动
    }
    if (direction == LEFT) Position -= Right * velocity;
    if (direction == RIGHT) Position += Right * velocity;
    if (direction == UP) Position += WorldUp * velocity;
    if (direction == DOWN) Position -= WorldUp * velocity;

    // if (direction != UP && direction != DOWN) Position.y = lastPos.y;
}

/**
 * @brief 处理鼠标移动输入
 * @param xpos 鼠标x坐标
 * @param ypos 鼠标y坐标
 * @param constrainPitch 是否限制`pitch`角度(防止翻转)
 */
void Camera::ProcessMouseMovement(double xpos, double ypos, GLboolean constrainPitch) {
    if (firstMouse == true) {
        lastX      = xpos;
        lastY      = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX         = xpos;
    lastY         = ypos;

    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
        if (Pitch > 89.0f) Pitch = 89.0f;
        if (Pitch < -89.0f) Pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

/**
 * @brief 处理鼠标滚轮输入
 * @param yoffset 滚轮滚动值
 */
void Camera::ProcessMouseScroll(float yoffset) {
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f) Zoom = 1.0f;
    if (Zoom > 45.0f) Zoom = 45.0f;
}

void Camera::updateCameraVectors() {
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front   = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right   = glm::normalize(glm::cross(Front, WorldUp));
    // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up      = glm::normalize(glm::cross(Right, Front));
}