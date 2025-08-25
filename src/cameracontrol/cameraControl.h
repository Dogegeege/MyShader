#pragma once
#include "camera.h"

namespace engine {

/// 任务：根据键盘鼠标，更改camera的矩阵
/// 种类：gameCameraControl wasd 鼠标右键 trackBallControl cad/cae软件当中
class CameraControl {
   public:
    using ptr = std::shared_ptr<CameraControl>;

    CameraControl(const Camera::ptr& camera) noexcept;

    ~CameraControl() noexcept;

    /**
     * @brief 键盘按下事件 ，一共108键
     * @param action 键盘事件用bitset表示
     */
    virtual void onKeyboard(const KeyBoardState& action) noexcept;

    /**
     * @brief 鼠标事件
     * @param action 分为左右中键，触发`UP`和 `DOWN`
     */
    virtual void onMouseAction(const MouseAction& action) noexcept;

    /**
     * @brief 鼠标移动事件，(0,0) 为左上角代表原点
     * @param xpos x轴正方向向右
     * @param ypos y轴正方向向下
     */
    virtual void onMouseMove(double xpos, double ypos) noexcept;

   protected:
    Camera::ptr mCamera{nullptr};  // 正交相机  透视相机
};
}  // namespace engine