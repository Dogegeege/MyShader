#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN };

namespace {
// 默认相机参数
constexpr float YAW         = -90.0f;
constexpr float PITCH       = 0.0f;
constexpr float SPEED       = 2.5f;
constexpr float SENSITIVITY = 0.1f;
constexpr float ZOOM        = 45.0f;

constexpr int SCR_WIDTH  = 1280;
constexpr int SCR_HEIGHT = 720;

constexpr float FAR_PLANE = 1000.0f;
}  // namespace

/**
 *
 */
class Camera {
   public:
    Camera(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));
    Camera(float posX, float posY, float posZ, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f);

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    inline glm::mat4 GetViewMatrix() const { return glm::lookAt(position, position + front, up); }
    inline glm::mat4 GetProjectionMatrix() const { return glm::perspective(glm::radians(zoom), aspectRatio, 0.1f, farPlame); }
    inline glm::vec3 GetPosition() const { return position; }
    inline glm::vec3 GetFront() const { return front; }
    inline glm::vec3 GetUp() const { return up; }
    inline glm::vec3 GetRight() const { return right; }
    inline float     GetYaw() const { return yaw; }
    inline float     GetPitch() const { return pitch; }

    inline void SetFarPlane(float farPlane) { farPlame = farPlane; }

    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(double xpos, double ypos, bool constrainPitch = true);
    void ProcessMouseScroll(float yoffset);

    // camera options
    float movementSpeed;     // 键盘移动灵敏度
    float mouseSensitivity;  // 鼠标灵敏度
    float zoom;              // 缩放视野(Field of View)
    float aspectRatio;       // 宽高比

   private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void UpdateCameraVectors();

    // 摄像机默认看向 -Z轴，即屏幕内
    // 不要忘记正z轴是从屏幕指向你的，如果我们希望摄像机向后移动，我们就沿着z轴的正方向移动。
    glm::vec3 position;  // 摄像机位置
    glm::vec3 up;        // 摄像机正上轴
    glm::vec3 right;     // 右轴
    glm::vec3 worldUp;   // 世界坐标系中的上轴(辅助轴，仅与摄像机前轴外积得到右轴，并非摄像机正交坐标空间的y轴)
    glm::vec3 front;     // 摄像机正前方轴

    // 欧拉角
    float yaw;
    float pitch;

    float farPlame;
};
#endif