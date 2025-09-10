#include "SceneManager.h"
#include "Title.h"
#include"Menu.h"
#include"Game.h"
Scene IScene::SceneNo = Scene::kTitle; // 現在のシーン番号を初期化
Scene IScene::PreSceneNO = Scene::kTitle;
sceneManage::sceneManage() {
	// シーンの配列を初期化
	currentSceneNo_ = Scene::kTitle;
	preSceneNo_ = Scene::kTitle;
	SceneArr_[static_cast<int>(currentSceneNo_)] = std::make_unique<TitleScene>();
}

sceneManage::~sceneManage() {}

void sceneManage::Initialize() {
	// シーンの初期化
	SceneArr_[static_cast<int>(currentSceneNo_)]->Initialize();
}

void sceneManage::Update() {
	preSceneNo_ = currentSceneNo_; // 前のシーン番号を保存
	currentSceneNo_ = SceneArr_[static_cast<int>(preSceneNo_)]->GetSceneNo(); // 現在のシーン番号を取得
	// シーンが変更された場合、前のシーンをリセットし、新しいシーンを初期化
	if (preSceneNo_ != currentSceneNo_) {
		SceneArr_[static_cast<int>(preSceneNo_)].reset();

		switch (currentSceneNo_) {
		case Scene::kTitle:
			SceneArr_[static_cast<int>(Scene::kTitle)] = std::make_unique<TitleScene>();
			break;
		case Scene::kMenu:
			SceneArr_[static_cast<int>(Scene::kMenu)] = std::make_unique<MenuScene>();
			break;
		case Scene::kGame:
			SceneArr_[static_cast<int>(Scene::kGame)] = std::make_unique<Game>();
			break;
		case Scene::kStage1:
			SceneArr_[static_cast<int>(Scene::kStage1)] = std::make_unique<Stage1>();
			break;
		case Scene::kStage2:
			SceneArr_[static_cast<int>(Scene::kStage2)] = std::make_unique<Stage2>();
			break;
		case Scene::kStage3:
			SceneArr_[static_cast<int>(Scene::kStage3)] = std::make_unique<Stage3>();
			break;
		case Scene::kStage4:
			SceneArr_[static_cast<int>(Scene::kStage4)] = std::make_unique<Stage4>();
			break;
		case Scene::kStage5:
			SceneArr_[static_cast<int>(Scene::kStage5)] = std::make_unique<Stage5>();
			break;

		case Scene::kReset:
			SceneArr_[static_cast<int>(Scene::kReset)] = std::make_unique<Reset>();
			SceneArr_[static_cast<int>(Scene::kReset)]->SetPreSceneNo(preSceneNo_);
			break;

		}

		SceneArr_[static_cast<int>(currentSceneNo_)]->Initialize();
	}
	// 現在のシーンの更新
	SceneArr_[static_cast<int>(currentSceneNo_)]->Update();
}

void sceneManage::Draw() {
	// 現在のシーンの描画
	SceneArr_[static_cast<int>(currentSceneNo_)]->Draw();
}
