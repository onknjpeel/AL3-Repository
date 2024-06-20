#define NOMINMAX
#include "Player.h"
#include "cassert"
#include "numbers"
#include <Input.h>
#include <algorithm>
#include "MapChipField.h"
#include <DebugText.h>

Player::Player() {}

Player::~Player() {}

void Player::Initalize(Model* model,ViewProjection* viewProjection,const Vector3& position,uint32_t textureHandle){
	assert(model);

	model_ = model;
	viewProjection_ = viewProjection;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	textureHandle_ = textureHandle;
}

void Player::Update(){
	Move();

	CollisionMapInfo collisionMapInfo;
	collisionMapInfo.moveAmount = velocity_;

	MapHitCollision(collisionMapInfo);
}

void Player::Draw(){
	model_->Draw(worldTransform_,*viewProjection_,textureHandle_);
}

void Player::Move() {
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
			velocity_ = velocity_ + Vector3(0,kGravityAcceleration,0);
		}
	}
	else {
		velocity_ = velocity_ + Vector3(0,-kGravityAcceleration,0);
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
		
		if (velocity_.y < 0) {
			if (worldTransform_.translation_.y <= 1.0f) {
				landing = true;
			}
		
		}
		if (landing) {
			worldTransform_.translation_.y = 1.0f;
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

void Player::MapHitCollision(CollisionMapInfo& info) {
	MapHitUp(info);
	MapHitDown(info);
	MapHitRight(info);
	MapHitLeft(info);
}

void Player::MapHitUp(CollisionMapInfo& info) {
	std::array<Vector3,Corner::kNumCorner> positionsNew;

	if (info.moveAmount.y <= 0) {
		return;
	}

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.moveAmount,static_cast<Corner>(i));
	}

	MapChipType mapChipType;

	bool hit = false;

	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex,indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex,indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0,+kHeight / 2.0f, 0));

		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex,indexSet.yIndex);
		info.moveAmount.y = std::max(0.0f,rect.bottom - worldTransform_.translation_.y - (kHeight / 2.0f + kBlank));
	
		info.hitCelling = true;
	}

	JudgeAndMove(info);

	HitCellingProcess(info);
}

void Player::MapHitDown(CollisionMapInfo& info) {
	std::array<Vector3,Corner::kNumCorner> positionsNew;

	if (info.moveAmount.y >= 0) {
		return;
	}

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.moveAmount,static_cast<Corner>(i));
	}

	MapChipType mapChipType;

	bool hit = false;

	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex,indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex,indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0,-kHeight / 2.0f, 0));

		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex,indexSet.yIndex);
		info.moveAmount.y = std::min(0.0f,rect.bottom + worldTransform_.translation_.y - (kHeight / 2.0f + kBlank));

		info.onGround = true;
	}

	SwitchOnGround(info);
}
void Player::MapHitRight(CollisionMapInfo& info) {
	std::array<Vector3,Corner::kNumCorner> positionsNew;
	
	if (info.moveAmount.x >= 0) {
		return;
	}

	MapChipType mapChipType;

	bool hit = false;

	IndexSet indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex,indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex,indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		HitWallProcess(info);

		info.hitWall = true;
	}
}
void Player::MapHitLeft(CollisionMapInfo& info) {
	std::array<Vector3,Corner::kNumCorner> positionsNew;

	if (info.moveAmount.x <= 0) {
		return;
	}

	MapChipType mapChipType;

	bool hit = false;

	IndexSet indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex,indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex,indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		HitWallProcess(info);

		info.hitWall = true;
	}
}

Vector3 Player::CornerPosition(const Vector3& center,Corner corner) {
	Vector3 offsetTable[kNumCorner] = {
		{+kWidth / 2.0f,-kHeight / 2.0f,0},
		{-kWidth / 2.0f,-kHeight / 2.0f,0},
		{+kWidth / 2.0f,+kHeight / 2.0f,0},
		{-kWidth / 2.0f,+kHeight / 2.0f,0}
	};

	Vector3 result = {
		center.x + offsetTable[static_cast<uint32_t>(corner)].x,
		center.y + offsetTable[static_cast<uint32_t>(corner)].y,
		center.z + offsetTable[static_cast<uint32_t>(corner)].z
	};

	return result;
};

void Player::JudgeAndMove(const CollisionMapInfo info) {
	worldTransform_.translation_.x += info.moveAmount.x;
	worldTransform_.translation_.y += info.moveAmount.y;
	worldTransform_.translation_.z += info.moveAmount.z;
}

void Player::HitCellingProcess(const CollisionMapInfo& info) {
	if (info.hitCelling) {
		DebugText::GetInstance()->ConsolePrintf("hit celling\n");
		velocity_.y = 0;
	}
}

void Player::SwitchOnGround(const CollisionMapInfo& info) {
	if (info.onGround) {
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		}
		else {
			std::array<Vector3,Corner::kNumCorner> positionsNew;

			MapChipType mapChipType;
			bool hit = false;

			IndexSet indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex,indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}

			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex,indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}

			if (!hit) {
				onGround_ = false;
			}
		}
	}
	else {
		if (info.onGround) {
			onGround_ = true;

			velocity_.x += (1.0f - kAttenuationLanding);

			velocity_.y = 0.0f;
		}
	}
}

void Player::HitWallProcess(const CollisionMapInfo& info) {
	if (info.hitWall) {
		velocity_.x *= (1.0f - kAttenuationWall);
	}
}