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
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	model_ = Model::Create();

	const uint32_t kNumBlockVirtical = 10;
	const uint32_t kNumBlockHorizontal = 20;
	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;

	worldTransformBlocks_.resize(kNumBlockVirtical);

	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			if((i+j)%2==1){
				worldTransformBlocks_[i][j] = new WorldTransform();

				worldTransformBlocks_[i][j]->Initialize();

				worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
				worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;
			}
		}
	}

	viewProjection_.Initialize();

	textureHandle_ = TextureManager::Load("block.jpg");

	debugCamera_ = new DebugCamera(1280,720);

	skydome_ = new Skydome();

	modelSkydome_ = Model::CreateFromOBJ("sphere",true);

	skydome_->Initialize(modelSkydome_,&viewProjection_);
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
