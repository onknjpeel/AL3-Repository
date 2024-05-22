#pragma once

#include <Model.h>
#include <WorldTransform.h>
#include <ViewProjection.h>

class Skydome {
	public:
		Skydome();

		~Skydome();

		void Initialize(Model* model,ViewProjection* viewProjection);

		void Update();

		void Draw();

	public:
		WorldTransform worldTransform_;
		Model* model_=nullptr;
		ViewProjection* viewProjection_=nullptr;
};