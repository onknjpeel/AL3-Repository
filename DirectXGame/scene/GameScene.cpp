#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;

	for(std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_){
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

	delete debugCamera_;

	delete skydome_;

	delete modelSkydome_;

	delete mapChipField_;
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

	viewProjection_.Initialize();

	textureHandle_ = TextureManager::Load("block.jpg");

	debugCamera_ = new DebugCamera(1280,720);

	skydome_ = new Skydome();

	modelSkydome_ = Model::CreateFromOBJ("sphere",true);

	skydome_->Initialize(modelSkydome_,&viewProjection_);

	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/map.csv");

	GenerateBlocks();
}

void GameScene::Update() {
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
		viewProjection_.matView=debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection=debugCamera_->GetViewProjection().matProjection;

		viewProjection_.TransferMatrix();
	}
	else {
		viewProjection_.UpdateMatrix();
	}
#endif
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
