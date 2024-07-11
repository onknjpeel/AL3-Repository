#include <Model.h>
#include <ViewProjection.h>
#include <WorldTransform.h>
#include <Vector3.h>
#include <array>
#include <numbers>

class DeathParticles {
	public:
		DeathParticles();

		~DeathParticles();

		void Initialize(Model* model,ViewProjection* viewProjection,const Vector3& position,uint32_t textureHandle);

		void Update();

		void Draw();

	private:
		Model* model_ = nullptr;

		ViewProjection* viewProjection_ = nullptr;

		uint32_t textureHandle_ = 0u;

		static inline const uint32_t kNumParticles = 8;

		std::array<WorldTransform, kNumParticles> worldTransforms_;

		static inline const float kDuration = 1.0f;

		static inline const float kSpeed = 0.1f;

		static inline const float kAngleUnit = float((2 * std::numbers::pi) / kNumParticles);

		bool isFinished_ = false;

		float counter_ = 0.0f;

		ObjectColor objectColor_;

		Vector4 color_;
};