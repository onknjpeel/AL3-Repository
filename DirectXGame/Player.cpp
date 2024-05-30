#include "Player.h"
#include "cassert"

Player::Player() {}

Player::~Player() {}

void Player::Initalize(Model* model, uint32_t textureHandle,ViewProjection* viewProjection){
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
	viewProjection_ = viewProjection;

	worldTransform_.Initialize();
}

void Player::Update(){
	worldTransform_.UpdateMatrix();
}

void Player::Draw(){
	model_->Draw(worldTransform_,*viewProjection_,textureHandle_);
}