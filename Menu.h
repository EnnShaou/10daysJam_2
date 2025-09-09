#pragma once
#include"SceneManager.h"
#include"Fade.h"
#include "BGM.h"
class MenuScene : public IScene {

public:
	MenuScene();
	~MenuScene() override;
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
	enum  class Stage
	{
		KUNKUWN = -1,
		STAGE1,
		STAGE2,
		STAGE3,
		STAGE4,
		STAGE5,

		KNUMBER,

	};

private:
	Fade* fade_ = nullptr; // フェードイン・フェードアウト用のオブジェクトb                    // シーンが終了したかどうか
	Phase phase_ = Phase::kFadeIn; // 現在のフェーズ
	Stage StageNow = Stage::STAGE1;
	int stageTextureHandle_[static_cast<int>(Stage::KNUMBER)] = { 0 };
	int cursorTextureHandle_ = 0;
	BGM* bgm_ = nullptr;

	int chooseSFX;		// 上下ボタン
	int confirmSFX;		// 選択を確定
};
class Reset : public IScene
{
public:
	Reset() {}
	~Reset() {}
	void Initialize() override{}
	void Update() override;
	void Draw() override;
};