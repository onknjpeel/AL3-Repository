#include <DeathParticles.h>
#include <MyMath.h>
#include <algorithm>

DeathParticles::DeathParticles() {

}

DeathParticles::~DeathParticles() {

}

void DeathParticles::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position, uint32_t textureHandle) {
	model_ = model;
	viewProjection_ = viewProjection;
	textureHandle_ = textureHandle;
	for (auto& worldTransform : worldTransforms_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}
	objectColor_.Initialize();
	color_ = {1,1,1,1};
}

void DeathParticles::Update() {
	if (isFinished_) {
		return;
	}

	for (uint32_t i = 0; i < kNumParticles; ++i) {
		Vector3 velocity = {kSpeed,0,0};

		float angle = kAngleUnit * i;

		Matrix4x4 matrixRotation = MakeRotateZMatrix(angle);

		velocity = Transform(velocity,matrixRotation);

		worldTransforms_[i].translation_ += velocity;
	}

	counter_ += 1.0f / 60.0f;

	color_.w = std::clamp(1.0f - counter_,0.0f,1.0f);
	objectColor_.SetColor(color_);
	objectColor_.TransferMatrix();

	if (counter_ >= kDuration) {
		counter_ = kDuration;

		isFinished_ = true;
	}

	for (auto& worldTransform : worldTransforms_) {
		MakeRotateMatrix(worldTransform.rotation_);
		MakeAffineMatrix(worldTransform.scale_,worldTransform.rotation_,worldTransform.translation_);
		worldTransform.UpdateMatrix();
	}
}

void DeathParticles::Draw() {
	if (isFinished_) {
		return;
	}
	for (auto& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform,*viewProjection_,textureHandle_,&objectColor_);
	}
}