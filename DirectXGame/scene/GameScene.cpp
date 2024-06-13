#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;

	delete player_;

	for(std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_){
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

	delete debugCamera_;

	delete skydome_;

	delete modelPlayer_;

	delete modelSkydome_;

	delete mapChipField_;

	delete cameraController_;
}

void GameScene::GenerateBlocks() {
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	worldTransformBlocks_.resize(numBlockVirtical);

	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if(mapChipField_->GetMapChipTypeByIndex(j,i) == MapChipType::kBlock){
				WorldTransform* worldTransform = new WorldTransform();

				worldTransform->Initialize();

				worldTransformBlocks_[i][j] = worldTransform;

				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j,i);
			}
		}
	}
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	model_ = Model::Create();
	textureHandle_ = TextureManager::Load("block.jpg");
	viewProjection_.Initialize();

	debugCamera_ = new DebugCamera(1280,720);

	skydome_ = new Skydome();
	modelSkydome_ = Model::CreateFromOBJ("sphere",true);
	skydome_->Initialize(modelSkydome_,&viewProjection_);

	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/map.csv");

	player_ = new Player();
	modelPlayer_ = Model::CreateFromOBJ("player",true);
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(2,17);
	player_->Initalize(modelPlayer_,&viewProjection_,playerPosition);
	player_->SetMapChipField(mapChipField_);

	GenerateBlocks();

	cameraController_ = new  CameraController;
	cameraController_->Initialize();
	cameraController_->SetTarget(player_);
	cameraController_->Reset();
	cameraController_->SetMovableArea({12.0f,100-12.0f,6.0f,6.0f});
}

void GameScene::Update() {
	player_->Update();

	for(std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_){
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if(!worldTransformBlock){
				continue;
			}
			worldTransformBlock->UpdateMatrix();

		}
	}

	debugCamera_->Update();

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
		if(!isDebugCameraActive_){
			isDebugCameraActive_ = true;
		}
		else {
			isDebugCameraActive_ = false;
		}
	}
	
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = cameraController_->GetViewProjection().matView;
		viewProjection_.matProjection = cameraController_->GetViewProjection().matProjection;

		viewProjection_.TransferMatrix();
	}
	else {
		viewProjection_.UpdateMatrix();
	}
#endif

	cameraController_->Update();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	player_->Draw();
	skydome_->Draw();
	for(std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_){
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if(!worldTransformBlock){
				continue;
			}
			model_->Draw(*worldTransformBlock,viewProjection_,textureHandle_);
		}
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
