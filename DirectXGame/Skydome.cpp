#include "Skydome.h"
#include <cassert>

Skydome::Skydome() {

}

Skydome::~Skydome() {

}

void Skydome::Initialize(Model* model,ViewProjection* viewProjection) {
	worldTransform_.Initialize();
	assert(model);
	model_=model;
	viewProjection_=viewProjection;
}

void Skydome::Update() {

}

void Skydome::Draw() {
	model_->Draw(worldTransform_,*viewProjection_);
}