#include "cameraControl.h"

namespace engine {

CameraControl::CameraControl(const Camera::ptr& camera) noexcept {
    mCamera = camera;
}

CameraControl::~CameraControl() noexcept {}

void CameraControl::onKeyboard(const KeyBoardState& action) noexcept {}

void CameraControl::onMouseAction(const MouseAction& action) noexcept {}

void CameraControl::onMouseMove(double xpos, double ypos) noexcept {}
}  // namespace engine