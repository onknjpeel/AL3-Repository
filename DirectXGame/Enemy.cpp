#include "Enemy.h"
#include <assert.h>

Enemy::Enemy() {}

Enemy::~Enemy() {}

void Enemy::Initialize(Model* model,ViewProjection* viewProjection,Vector3 position,uint32_t textureHandle) {
	assert(model);

	model_ = model;
	viewProjection_ = viewProjection;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = -std::numbers::pi_v<float> / 2.0f;

	textureHandle_ = textureHandle;

	velocity_ = {-kWalkSpeed,0,0};
	walkTimer_ = 0.0f;
}

void Enemy::Update() {
	worldTransform_.translation_ += velocity_;

	walkTimer_ += 1.0f/60.0f;

	float param = std::sin((2.0f*std::numbers::pi_v<float>)*(walkTimer_/kWalkMotionTime));
	float radian = kWalkMotionAngleStart+kWalkMotionAngleEnd*(param+1.0f)/2.0f;
	xTurnAngle = EaseInSine(radian);

	worldTransform_.rotation_.x = xTurnAngle;

	worldTransform_.UpdateMatrix();
}

void Enemy::Draw() {
	model_->Draw(worldTransform_,*viewProjection_,textureHandle_);
}

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

AABB Enemy::GetAABB() {
	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f,worldPos.y - kHeight / 2.0f,worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f,worldPos.y + kHeight / 2.0f,worldPos.z + kWidth / 2.0f};

	return aabb;
}

void Enemy::OnCollision(const Player* player) {
	(void)player;
}