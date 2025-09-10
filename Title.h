#pragma once
#include"SceneManager.h"
#include"Fade.h"

#include "BGM.h"

class TitleScene : public IScene {

public:
	TitleScene();
	~TitleScene() override;
	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;
	// 描画
	void Draw() override;
	// フェーズを変更する
	void ChangePhase() {}
	// bool IsFinished() const { return finished_; } // シーンが終了したかどうかを取得
	enum class Phase { kFadeIn = 0, kMaui, kFadeOut };

private:
	Fade* fade_ = nullptr; // フェードイン・フェードアウト用のオブジェクトb
	// bool finished_ = false;                     // シーンが終了したかどうか
	float elapsedTime_ = 0.0f;
	Phase phase_ = Phase::kFadeIn; // 現在のフェーズ
	BGM* bgm_ = nullptr;

	int confirmSFX;

	int backgroundHandle_ = 0;
	int spaceHandle_ = 0;

	// animation
	int animationTimer = 0;
	int animationMax = 4;
	int animePosX = 0;
	int imageWidth = 1280;
	int imageHeight = 720;

	void Animation();
};
