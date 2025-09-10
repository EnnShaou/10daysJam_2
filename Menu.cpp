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



	chooseSFX = Novice::LoadAudio("./Resources/Audio/sfx/choose.mp3");
	confirmSFX = Novice::LoadAudio("./Resources/Audio/sfx/confirm.mp3");

	backgroundHandle_ = Novice::LoadTexture("./Resources/Scene/stageSelectSceen.png");
	cursorTextureHandle_ = Novice::LoadTexture("./Resources/PlayerBullet/bullet.png");
	stageTextureHandle_[0] = Novice::LoadTexture("./Resources/StageSelect/stageSelection1.png");
	stageTextureHandle_[1] = Novice::LoadTexture("./Resources/StageSelect/stageSelection2.png");
	stageTextureHandle_[2] = Novice::LoadTexture("./Resources/StageSelect/stageSelection3.png");
	stageTextureHandle_[3] = Novice::LoadTexture("./Resources/StageSelect/stageSelection4.png");
	stageTextureHandle_[4] = Novice::LoadTexture("./Resources/StageSelect/stageSelection5.png");
}

void MenuScene::Update()
{
	bgm_->PlayBGM(0);
	Animation();
	switch (phase_) {
	case MenuScene::Phase::kFadeIn:
		phase_ = MenuScene::Phase::kMaui;

		break;
	case MenuScene::Phase::kMaui:
		fade_->Update(); // フェードの更新
		if (Keys::IsTrigger(DIK_W) || Keys::IsTrigger(DIK_UP))
		{
			Novice::PlayAudio(chooseSFX, 0, 0.3f);
			if (static_cast<Stage>(static_cast<int> (StageNow) - 1) != Stage::KUNKUWN)
			{
				StageNow = static_cast<Stage>(static_cast<int> (StageNow) - 1);
			}

		}
		if (Keys::IsTrigger(DIK_S) || Keys::IsTrigger(DIK_DOWN))
		{
			Novice::PlayAudio(chooseSFX, 0, 0.3f);
			if (static_cast<Stage>(static_cast<int> (StageNow) + 1) != Stage::KNUMBER)
			{
				StageNow = static_cast<Stage>(static_cast<int> (StageNow) + 1);
			}
		}


		if (Keys::IsPress(DIK_SPACE)) {
			Novice::PlayAudio(confirmSFX, 0, 0.5f);
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
			bgm_->Stop();
			switch (StageNow)
			{
			case MenuScene::Stage::STAGE1:
				SceneNo = Scene::kStage1;
				break;
			case MenuScene::Stage::STAGE2:
				SceneNo = Scene::kStage2;
			break; 
			case MenuScene::Stage::STAGE3:
				SceneNo = Scene::kStage3;
			break;
			case MenuScene::Stage::STAGE4:
				SceneNo = Scene::kStage4;
			break; 
			case MenuScene::Stage::STAGE5:
				SceneNo = Scene::kStage5;
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
	// 背景描画
	Novice::DrawSpriteRect(0, 0, bgAnimePosX, 0, 1280, 720,
		backgroundHandle_, 1.0f / 4.0f, 1.0f, 0.0f, WHITE);
	int posX = 512;
	int posY = 100;
	int offset = 96;
	for (int i = 0; i < static_cast<int>(Stage::KNUMBER); i++)
	{
		Novice::DrawSprite(posX, (posY + (offset * (i + 1))),
			stageTextureHandle_[i], 1.0f, 1.0f, 0.F, 0xffffffff);
	}
	for (int i = 0; i < 2; i++)
	{
		/*Novice::DrawSprite(posX + (i == 0 ? -80 : 256 + 80 - 32),
			(posY + 24) * (static_cast<int>(StageNow) + 1) + 16,
0			cursorTextureHandle_, 1.0f, 1.0f, 0.F, 0xffffffff);*/
		Novice::DrawSpriteRect(posX + (i == 0 ? -80 : 256 + 80 - 32),
			(posY + (offset * (static_cast<int>(StageNow) + 1) + 16)),
			animePosX, 0, imageWidth, imageHeight, cursorTextureHandle_, 1.0f / 3.0f, 1.0f, 0.0f, WHITE);
	}
	fade_->Draw();
}

void MenuScene::Animation() {
	animationTimer++;
	animationMax = 3;
	if (animationTimer % 12 == 0) {
		animePosX += imageWidth;
		bgAnimePosX += 1280;
	}
	// icon
	if (animePosX >= imageWidth * animationMax) {
		animePosX = 0;
	}

	// background
	if (bgAnimePosX >= 1280 * 4) {
		bgAnimePosX = 0;
	}
	
}

void Reset::Update()
{
	SceneNo = PreSceneNO;
}

void Reset::Draw()
{
	Novice::DrawBox(0, 0, 1280, 720, 0.f, BLACK, kFillModeSolid);
}
