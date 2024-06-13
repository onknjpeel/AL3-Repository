#include "CameraController.h"
#include "Player.h"

CameraController::CameraController() {

}

CameraController::~CameraController() {

}

void CameraController::Initialize() {
	viewProjection_.Initialize();
}

void CameraController::Update() {
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	const Vector3& targetVelocity = target_->GetVelocity();

	targetPos_.x = targetWorldTransform.translation_.x + targetOffset_.x + targetVelocity.x * kVelocityBias;
	targetPos_.y = targetWorldTransform.translation_.y + targetOffset_.y + targetVelocity.y* kVelocityBias;
	targetPos_.z = targetWorldTransform.translation_.z + targetOffset_.z + targetVelocity.z * kVelocityBias;

	viewProjection_.translation_ = Lerp(viewProjection_.translation_,targetPos_,kInterpolationRate);
	
	viewProjection_.translation_.x = std::max<float>(viewProjection_.translation_.x,targetPos_.x + mergin.left);
	viewProjection_.translation_.x = std::min<float>(viewProjection_.translation_.x,targetPos_.x + mergin.right);
	viewProjection_.translation_.y = std::max<float>(viewProjection_.translation_.y,targetPos_.y + mergin.bottom);
	viewProjection_.translation_.y = std::min<float>(viewProjection_.translation_.y,targetPos_.y + mergin.top);

	viewProjection_.translation_.x = std::max<float>(viewProjection_.translation_.x,movableArea_.left);
	viewProjection_.translation_.x = std::min<float>(viewProjection_.translation_.x,movableArea_.right);
	viewProjection_.translation_.y = std::max<float>(viewProjection_.translation_.y,movableArea_.bottom);
	viewProjection_.translation_.y = std::min<float>(viewProjection_.translation_.y,movableArea_.top);

	viewProjection_.UpdateMatrix();
}

void CameraController::Reset() {
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	viewProjection_.translation_.x = targetWorldTransform.translation_.x + targetOffset_.x;
	viewProjection_.translation_.y = targetWorldTransform.translation_.y + targetOffset_.y;
	viewProjection_.translation_.z = targetWorldTransform.translation_.z + targetOffset_.z;
}

Vector3 CameraController::Lerp(const Vector3& startPos, const Vector3& endPos, float t) {
	Vector3 position;
	position.x = t * startPos.x + (1.0f - t) * endPos.x;
	position.y = t * startPos.y + (1.0f - t) * endPos.y;
	position.z = t * startPos.z + (1.0f - t) * endPos.z;
	return position;
}