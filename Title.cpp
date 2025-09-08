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
}

void TitleScene::Update() {

	switch (phase_) {
	case TitleScene::Phase::kFadeIn:
		phase_ = TitleScene::Phase::kMaui;

		break;
	case TitleScene::Phase::kMaui:
		fade_->Update(); // フェードの更新
		if (Keys::IsPress(DIK_SPACE)) {
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
			SceneNo =Scene::kGame ;
		}
		fade_->Update();        // フェードの更新
		break;
	default:
		break;
	}
}

void TitleScene::Draw() {

	fade_->Draw();
}
