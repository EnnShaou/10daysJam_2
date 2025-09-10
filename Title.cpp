#include "Title.h"

#include"keys.h"
#include"Novice.h"

TitleScene::TitleScene() {}

TitleScene::~TitleScene() {


	if (fade_) {
		delete fade_;
		fade_ = nullptr;
	}

}

void TitleScene::Initialize() {

	fade_ = new Fade();                       // フェードイン・フェードアウト用のオブジェクトを生成
	fade_->Initialize();                      // フェードの初期化
	fade_->Start(Fade::Status::FadeIn, 1.0f); // フェードインを開始
	bgm_ = new BGM;
	bgm_->Initialize();


	confirmSFX = Novice::LoadAudio("./Resources/Audio/sfx/confirm.mp3");
	backgroundHandle_ = Novice::LoadTexture("./Resources/Scene/titleSceen.png");
	spaceHandle_ = Novice::LoadTexture("./Resources/Scene/space.png");
}

void TitleScene::Update() {

	
	bgm_->SetVolume(0.15f);
	Animation();
	switch (phase_) {
	case TitleScene::Phase::kFadeIn:
		phase_ = TitleScene::Phase::kMaui;
		bgm_->PlayBGM(0);
		break;
	case TitleScene::Phase::kMaui:
		fade_->Update(); // フェードの更新
		if (Keys::IsPress(DIK_SPACE)) {
			Novice::PlayAudio(confirmSFX, 0, 0.5f);
			phase_ = TitleScene::Phase::kFadeOut;      // フェードアウトフェーズに移行
			fade_->Start(Fade::Status::FadeOut, 1.0f); // フェードアウトを開始
		}

#ifdef _DEBUG
		// ImGuiの表示
		Novice::ScreenPrintf(0, 0, "TITLE");
#endif                                // _DEBUG

		break;

	case TitleScene::Phase::kFadeOut:
		if (fade_->IsFinished()) { // フェードアウトが完了したらシーンを終了
			//bgm_->Stop();
			SceneNo =Scene::kMenu;
		}
		fade_->Update();        // フェードの更新
		break;
	default:
		break;
	}
}

void TitleScene::Draw() {
	Novice::DrawSpriteRect(0, 0, animePosX, 0, imageWidth, imageHeight, 
		backgroundHandle_, 1.0f / 4.0f, 1.0f, 0.0f, WHITE);

	int cycleTime = 60; // 1秒ループ
	int timeInCycle = animationTimer % cycleTime;

	if (timeInCycle < 30) { 
		Novice::DrawSprite(0, 0, spaceHandle_, 1.0f, 1.0f, 0.0f, WHITE);
	}
	fade_->Draw();
}

void TitleScene::Animation() {
	animationTimer++;
	animationMax = 4;
	if (animationTimer % 12 == 0) {
		animePosX += imageWidth;

	}
	if (animePosX >= imageWidth * animationMax) {
		animePosX = 0;
	}
}
