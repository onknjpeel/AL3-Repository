#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <vector>
#include "Player.h"
#include "DebugCamera.h"
#include "Skydome.h"
#include "MapChipField.h"
#include "CameraController.h"
#include "Enemy.h"
#include "DeathParticles.h"

enum class Phase {
	kPlay,
	kDeath,
};


/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void GenerateBlocks();

	void CheckAllCollisions();

	void ChangePhase();

	bool IsFinished() const {return finished_;}

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	Model* model_ = nullptr;
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;
	ViewProjection viewProjection_;

	uint32_t textureHandle_= 0;
	uint32_t playerTextureHandle_ = 0;
	uint32_t enemyTextureHandle_ = 0;
	uint32_t particlesTextureHandle_ = 0;

	bool isDebugCameraActive_ = false;
	DebugCamera* debugCamera_ = nullptr;
	Skydome* skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;
	Model* modelPlayer_ = nullptr;
	Model* modelEnemy_ = nullptr;
	Model* modelParticles_ = nullptr;

	MapChipField* mapChipField_;

	Player* player_ = nullptr;

	CameraController* cameraController_ = nullptr;

	std::list<Enemy*> enemies_;
	int32_t enemyNum_ = 3;

	DeathParticles* deathParticles_ = nullptr;

	Phase phase_;

	bool finished_ = false;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
