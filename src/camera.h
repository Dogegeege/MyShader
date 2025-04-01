#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT };

// 默认相机参数
const float YAW         = -90.0f;
const float PITCH       = 0.0f;
const float SPEED       = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM        = 45.0f;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera {
   public:
    // 摄像机默认看向 -Z轴，即屏幕内
    // 不要忘记正z轴是从屏幕指向你的，如果我们希望摄像机向后移动，我们就沿着z轴的正方向移动。
    glm::vec3 Position;  // 摄像机位置
    glm::vec3 Front;     // 摄像机正前方轴
    glm::vec3 Up;        // 摄像机正上轴
    glm::vec3 Right;     // 右轴
    glm::vec3 WorldUp;   // 世界坐标系中的上轴(辅助轴，仅与摄像机前轴外积得到右轴，并非摄像机正交坐标空间的y轴)

    // 欧拉角
    float Yaw;
    float Pitch;

    // camera options
    float MovementSpeed;     // 键盘移动灵敏度
    float MouseSensitivity;  // 鼠标灵敏度
    float Zoom;              // FOV视野,度数表示

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    Camera(float posX, float posY, float posZ, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f, float yaw = YAW, float pitch = PITCH);

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    inline glm::mat4 GetViewMatrix() { return glm::lookAt(Position, Position + Front, Up); }

    void ProcessKeyboard(Camera_Movement direction, float deltaTime);

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset);

   private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    inline void updateCameraVectors() {
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
};
#endif