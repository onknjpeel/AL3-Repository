#include "ViewProjection.h"

class Player;

struct Rect {
	float left = 0.0f;
	float right = 1.0f;
	float bottom = 0.0f;
	float top = 1.0f;
};

class CameraController {
	public:
		CameraController();

		~CameraController();

		void Initialize();

		void Update();

		void SetTarget(Player* target){target_ = target;}

		void Reset();

		const ViewProjection&  GetViewProjection() const {return viewProjection_;}
		
		void SetMovableArea(Rect area){movableArea_ = area;}

		Vector3 Lerp(const Vector3& startPos, const Vector3& endPos, float t);

		private:
			ViewProjection viewProjection_;

			Player* target_ = nullptr;

			Vector3 targetOffset_ = {0,0,-15.0f};

			Rect movableArea_ = {0,100,100};

			Vector3 targetPos_;

			static inline const float kInterpolationRate = 0.8f;

			static inline const float kVelocityBias = 15.0f;

			static inline const Rect mergin = {-12.0f,100-12.0f,-6.0f,6.0f};
};