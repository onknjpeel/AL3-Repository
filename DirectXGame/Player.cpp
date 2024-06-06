#define NOMINMAX
#include "Player.h"
#include "cassert"
#include "numbers"
#include <Input.h>
#include <algorithm>

Player::Player() {}

Player::~Player() {}

void Player::Initalize(Model* model,ViewProjection* viewProjection,const Vector3& position){
	assert(model);

	model_ = model;
	viewProjection_ = viewProjection;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

void Player::Update(){
	bool landing = false;

	if (onGround_) {
		if (Input::GetInstance()->PushKey(DIK_RIGHT) ||
			Input::GetInstance()->PushKey(DIK_LEFT)){

			Vector3 acceleration = {};
		
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;

					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}
				if (velocity_.x < 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}

				acceleration.x += kAcceleration;
			}
			else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
						
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}
				if (velocity_.x > 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}

				acceleration.x -= kAcceleration;
			}

			velocity_.x += acceleration.x;
			velocity_.x = std::clamp(velocity_.x,-kLimitRunSpeed,kLimitRunSpeed);
		
			if(turnTimer_ > 0.0f){

				turnTimer_ -= kTimeTurn / 60;

				float destinationRotationYTable[] = {
					std::numbers::pi_v<float> / 2.0f,
					std::numbers::pi_v<float> * 3.0f / 2.0f
				};

				float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
				worldTransform_.rotation_.y = destinationRotationY + (1 * EaseOutSine(turnTimer_));
			}
		}
		else {
			velocity_.x *= (1.0f - kAttenuation);
		}
		
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		}
		
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			velocity_.x += 0;
			velocity_.y += kGravityAcceleration;
			velocity_.z += 0;
		}
	}
	else {
		velocity_.x += 0;
		velocity_.y += -kGravityAcceleration;
		velocity_.z += 0;
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
		
		if (velocity_.y < 0) {
			if (worldTransform_.translation_.y <= 1.0f) {
				landing = true;
			}
		
		}
		if (landing) {
			worldTransform_.translation_.y = 1.5f;
			velocity_.x *= (1.0f - kAttenuation);
			velocity_.y = 0.0f;
			onGround_ = true;
		}
	}

	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;
	
	worldTransform_.UpdateMatrix();
}

void Player::Draw(){
	model_->Draw(worldTransform_,*viewProjection_,textureHandle_);
}