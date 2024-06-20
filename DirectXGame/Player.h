#include "WorldTransform.h"
#include "Model.h"
#include "ViewProjection.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include "MyMath.h"

enum class LRDirection {
	kRight,
	kLeft,
};

class MapChipField;

struct CollisionMapInfo {
	bool hitCelling = false;
	bool onGround = false;
	bool hitWall = false;
	Vector3 moveAmount;
};

enum Corner {
	kRightBottom,
	kLeftBottom,
	kRightTop,
	kLeftTop,

	kNumCorner
};

class Player{
	public:
		Player();
		~Player();

		void Initalize(Model* model,ViewProjection* viewProjection,const Vector3& position);

		void Update();

		void Draw();

		float EaseOutSine(float num) {
			return sinf((num * float(M_PI)) / 2.0f);
		}

		const WorldTransform& GetWorldTransform() const {return worldTransform_;}

		const Vector3& GetVelocity() const {return velocity_;}

		void SetMapChipField(MapChipField* mapChipField){mapChipField_ = mapChipField;}

		void Move();

		void MapHitCollision(CollisionMapInfo& info);

		void MapHitUp(CollisionMapInfo& info);
		/*void MapHitDown(CollisionMapInfo& info);
		void MapHitRight(CollisionMapInfo& info);
		void MapHitLeft(CollisionMapInfo& info);
*/
		Vector3 CornerPosition(const Vector3& center,Corner corner);

		void JudgeAndMove(const CollisionMapInfo info); 

		void HitCellingProcess(const CollisionMapInfo& info);

	private:
		WorldTransform worldTransform_;

		Model* model_ = nullptr;

		uint32_t textureHandle_ = 0u;

		ViewProjection* viewProjection_ = nullptr;

		Vector3 velocity_ = {};

		static inline const float kAcceleration = 0.1f;
		static inline const float kAttenuation = 0.5f;
		static inline const float kLimitRunSpeed = 0.5f;

		LRDirection lrDirection_ = LRDirection::kRight;

		float turnFirstRotationY_ = 0.0f;
		float turnTimer_ = 0.0f;

		static inline const float kTimeTurn = 0.3f;

		bool onGround_ = true;

		static inline const float kGravityAcceleration = 0.05f;
		static inline const float kLimitFallSpeed = 5.0f;
		static inline const float kJumpAcceleration = 5.0f;

		MapChipField* mapChipField_ = nullptr;

		static inline const float kWidth = 0.8f;
		static inline const float kHeight = 0.8f;

		static inline const float kBlank = 5.0f;
};