#pragma once
#include "cameraControl.h"

namespace engine {

class GameCameraControl : public CameraControl {
   public:
    static constexpr int MoveStateNum = 6;
    static constexpr int MoveFront    = 0;
    static constexpr int MoveBack     = 1;
    static constexpr int MoveLeft     = 2;
    static constexpr int MoveRight    = 3;
    static constexpr int MoveUP       = 4;
    static constexpr int MoveDown     = 5;

    using ptr = std::shared_ptr<GameCameraControl>;
    static ptr create(const Camera::ptr& camera) { return std::make_shared<GameCameraControl>(camera); }

    GameCameraControl(const Camera::ptr& camera) noexcept;

    ~GameCameraControl() noexcept;

    /**
     * @brief 设置鼠标控制摄像机的速度
     */
    void setSpeed(float speed) noexcept;

    /**
     * @brief 设置鼠标控制摄像机的灵敏度
     */
    void setSensitivity(float s) noexcept;

    /**
     * @brief 键盘按下事件 ，一共108键，`gameCameraControl`只处理wasd
     * @param action 键盘事件用bitset表示
     */
    virtual void onKeyboard(const KeyBoardState& action) noexcept override;

    /**
     * @brief 鼠标事件,`gameCameraControl`
     * @param action 分为左右中键，触发`UP`和 `DOWN`
     */
    virtual void onMouseAction(const MouseAction& action) noexcept override;

    /**
     * @brief 鼠标移动事件，(0,0) 为左上角代表原点，更新摄像机朝向
     * @param xpos x轴正方向向右
     * @param ypos y轴正方向向下
     */
    virtual void onMouseMove(double xpos, double ypos) noexcept override;

    // TODO: 增加移动模式，固定高度模式
    inline void setFPSMode(bool isFPS) noexcept { mIsUsingFPSMode = isFPS; }
    inline bool isFPSMode() const noexcept { return mIsUsingFPSMode; }

    /**
     * @brief 更新摄像机位置
     */
    void update() noexcept;

   private:
    float mSpeed{0.1f};        // 移动速度
    float mSensitivity{0.1f};  // 鼠标旋转灵敏度

    glm::vec3 mFront = glm::vec3(0.0f);  // 当前相机的`front`
    float     mPitchAngle{0.0f};         // 绕x轴旋转（上下俯仰角）
    float     mYawAngle{-90.0f};         // 绕y轴旋转（左右偏航角）看向-z方向

    /// control parameters
    glm::vec2                 mCurrentMousePosition = glm::vec2(0.0f);  // 记录最后一次调用`onMouseMove`时候的位置
    std::bitset<MoveStateNum> mMoveState;                               // 记录按键状态

    bool mRotationState   = false;  /// 右键按下，才进行旋转
    bool mMouseStateReset = true;   // 是否为首次聚焦窗口（第一次获取位置）
    bool mIsUsingFPSMode  = true;   // 是否为FPS模式（固定世界坐标系的xy轴）
};

}  // namespace engine