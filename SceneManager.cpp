#include "SceneManager.h"
#include "Title.h"
#include"Game.h"
Scene IScene::SceneNo = Scene::kTitle; // 現在のシーン番号を初期化
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

void sceneManage::UpDate() {
	preSceneNo_ = currentSceneNo_; // 前のシーン番号を保存

	currentSceneNo_ = SceneArr_[static_cast<int>(preSceneNo_)]->GetSceneNo(); // 現在のシーン番号を取得
	// シーンが変更された場合、前のシーンをリセットし、新しいシーンを初期化
	if (preSceneNo_ != currentSceneNo_) {
		SceneArr_[static_cast<int>(preSceneNo_)].reset();

		switch (currentSceneNo_) {
		case Scene::kTitle:
			SceneArr_[static_cast<int>(Scene::kTitle)] = std::make_unique<TitleScene>();
			break;
		case Scene::kGame:
			SceneArr_[static_cast<int>(Scene::kGame)] = std::make_unique<Game>();
			break;
		case Scene::kClear:
			//SceneArr_[static_cast<int>(Scene::kClear)] = std::make_unique<ClearScene>();
			break;
		case Scene::kOver:
			//SceneArr_[static_cast<int>(Scene::kOver)] = std::make_unique<OverScene>();
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
