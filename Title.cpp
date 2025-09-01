#include "Title.h"
#include "ImGui.h"


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
		if (/*Input::GetInstance()->PushKey(DIK_SPACE)*/true) {
			phase_ = TitleScene::Phase::kFadeOut;      // フェードアウトフェーズに移行
			fade_->Start(Fade::Status::FadeOut, 1.0f); // フェードアウトを開始
		}

#ifdef _DEBUG
		// ImGuiの表示
		ImGui::Text("Title Scene");
		ImGui::Text("Press Enter to start the game.");
#endif                                // _DEBUG

		break;

	case TitleScene::Phase::kFadeOut:
		if (fade_->IsFinished()) { // フェードアウトが完了したらシーンを終了
			//SceneNo = ;
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
