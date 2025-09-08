#include "Menu.h"
#include"keys.h"
MenuScene::MenuScene()
{
}

MenuScene::~MenuScene()
{

	if (fade_) {
		delete fade_;
		fade_ = nullptr;
	}
}

void MenuScene::Initialize()
{

	fade_ = new Fade();                       // フェードイン・フェードアウト用のオブジェクトを生成
	fade_->Initialize();                      // フェードの初期化
	fade_->Start(Fade::Status::FadeIn, 1.0f); // フェードインを開始


}

void MenuScene::Update()
{
	switch (phase_) {
	case MenuScene::Phase::kFadeIn:
		phase_ = MenuScene::Phase::kMaui;

		break;
	case MenuScene::Phase::kMaui:
		fade_->Update(); // フェードの更新
		if (Keys::IsTrigger(DIK_W) || Keys::IsTrigger(DIK_UP))
		{
			if (static_cast<Stage>(static_cast<int> (StageNow) - 1) != Stage::KUNKUWN)
			{
				StageNow = static_cast<Stage>(static_cast<int> (StageNow) - 1);
			}

		}
		if (Keys::IsTrigger(DIK_S) || Keys::IsTrigger(DIK_DOWN))
		{
			if (static_cast<Stage>(static_cast<int> (StageNow) + 1) != Stage::KNUMBER)
			{
				StageNow = static_cast<Stage>(static_cast<int> (StageNow) + 1);
			}
		}


		if (Keys::IsPress(DIK_SPACE)) {
			phase_ = MenuScene::Phase::kFadeOut;      // フェードアウトフェーズに移行
			fade_->Start(Fade::Status::FadeOut, 1.0f); // フェードアウトを開始
		}

#ifdef _DEBUG
		// ImGuiの表示
		Novice::ScreenPrintf(0, 0, "NOW STAGE %d", static_cast<int>(StageNow));
#endif                                // _DEBUG

		break;

	case MenuScene::Phase::kFadeOut:
		if (fade_->IsFinished()) { // フェードアウトが完了したらシーンを終了
			switch (StageNow)
			{
			case MenuScene::Stage::STAGE1:
				SceneNo = Scene::kStage1;
				break;
			case MenuScene::Stage::STAGE2:
				break;
			}

		}
		fade_->Update();        // フェードの更新
		break;
	default:
		break;
	}
}

void MenuScene::Draw()
{

	int posX = 512;
	int posY = 64;
	for (int i = 0; i < static_cast<int>(Stage::KNUMBER); i++)
	{
		Novice::DrawSprite(posX, (posY + 24) * (i + 1),
			stageTextureHandle_[i], 256, 64, 0.F, 0xffffffff);
	}
	for (int i = 0; i < 2; i++)
	{
		Novice::DrawSprite(posX + (i == 0 ? -80 : 256 + 80 - 32),
			(posY + 24) * (static_cast<int>(StageNow) + 1) + 16,
			cursorTextureHandle_, 32, 32, 0.F, 0xffffffff);
	}
	fade_->Draw();
}

void Reset::Update()
{
	SceneNo = PreSceneNO;
}

void Reset::Draw()
{
	Novice::DrawBox(0, 0, 1280, 720, 0.f, BLACK, kFillModeSolid);
}
