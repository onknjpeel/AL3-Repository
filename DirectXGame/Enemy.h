#include "WorldTransform.h"
#include "Model.h"
#include "ViewProjection.h"
#include "numbers"

class Enemy {
	public:
		Enemy();

		~Enemy();

		void Initialize(Model* model,ViewProjection* viewProjection,Vector3 position,uint32_t textureHandle);

		void Update();

		void Draw();

	private:

		WorldTransform worldTransform_;

		Model* model_ = nullptr;

		uint32_t textureHandle_ = 0u;

		ViewProjection* viewProjection_ = nullptr;

		static inline const float kWalkSpeed = 0.01f;

		Vector3 velocity_ = {};

		static inline const float kWalkMotionAngleStart = (-std::numbers::pi_v<float> / 4.0f);
		static inline const float kWalkMotionAngleEnd = (std::numbers::pi_v<float> / 4.0f);
		static inline const float kWalkMotionTime = 0.5f;

		float walkTimer_ = 0.0f;

		float xTurnAngle;
};