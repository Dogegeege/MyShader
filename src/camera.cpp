#include "camera.h"

Camera::Camera(const glm::vec3& position, const glm::vec3& up)
    : front(glm::vec3(0.0f, 0.0f, -1.0f)),
      movementSpeed(SPEED),
      mouseSensitivity(SENSITIVITY),
      zoom(ZOOM),
      yaw(YAW),
      pitch(PITCH),
      farPlame(FAR_PLANE),
      position(position),
      worldUp(up),
      aspectRatio(static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT)) {
    UpdateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ)
    : Camera(glm::vec3(posX, posY, posZ), glm::vec3(upX, upY, upZ)) {};

/**
 * @brief 处理键盘输入
 * @param Camera_Movement 移动参数: `FORWARD`=>前, `BACKWARD`=>后, `LEFT`=>左, `RIGHT`=>右
 * @param deltaTime 前后帧间隔时间
 */
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
    float velocity = movementSpeed * deltaTime;

    // 注意Front y方向的分量不能对移动产生影响
    if (direction == FORWARD || direction == BACKWARD) {
        glm::vec3 front_xz = glm::normalize(glm::vec3(front.x, 0.f, front.z));

        if (direction == FORWARD) position += front_xz * velocity;   // <-- 除了上下移动，其他都限制在y=0平面内
        if (direction == BACKWARD) position -= front_xz * velocity;  // <-- 保证摄像机在y=0平面移动
    }
    if (direction == LEFT) position -= right * velocity;
    if (direction == RIGHT) position += right * velocity;
    if (direction == UP) position += worldUp * velocity;
    if (direction == DOWN) position -= worldUp * velocity;

    // if (direction != UP && direction != DOWN) Position.y = lastPos.y;
}

/**
 * @brief 处理鼠标移动输入
 * @param xoffset 鼠标x偏移量
 * @param yoffset 鼠标y偏移量
 * @param constrainPitch 是否限制`pitch`角度(防止翻转)
 */
void Camera::ProcessMouseMovement(double xoffset, double yoffset, bool constrainPitch) {
    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    UpdateCameraVectors();
}

/**
 * @brief 处理鼠标滚轮输入
 * @param yoffset 滚轮滚动值
 */
void Camera::ProcessMouseScroll(float yoffset) {
    zoom -= (float)yoffset;
    if (zoom < 1.0f) zoom = 1.0f;
    if (zoom > 45.0f) zoom = 45.0f;
}

void Camera::UpdateCameraVectors() {
    // calculate the new Front vector
    glm::vec3 resultFront;
    resultFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    resultFront.y = sin(glm::radians(pitch));
    resultFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    front = glm::normalize(resultFront);
    // also re-calculate the Right and Up vector
    right = glm::normalize(glm::cross(front, worldUp));
    // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    up    = glm::normalize(glm::cross(right, front));
}