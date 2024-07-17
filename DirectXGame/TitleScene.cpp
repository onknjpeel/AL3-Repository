#include "TitleScene.h"
#include <Input.h>

TitleScene::TitleScene() {

}

TitleScene::~TitleScene() {

}

void TitleScene::Initialize() {

}

void TitleScene::Update() {
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}
}

void TitleScene::Draw() {

}