#include "ViewProjection.h"

class Player;

class CameraController {
	public:
		CameraController();

		~CameraController();

		void Initialize();

		void Update();

		void SetTarget(Player* target){target_ = target;}

		void Reset();

		private:
			ViewProjection viewProjection_;

			Player* target_ = nullptr;

			Vector3 targetOffset_ = {0,0,-15.0f};
};