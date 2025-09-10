#pragma once
#include <memory>

enum class Scene { kUnknown = -1, kTitle, kGame, kMenu, kStage1, kStage2, kStage3, kStage4, kStage5, kClear, kReset, kOver, SceneNem };

class IScene { // インターフェースクラス
public:
	IScene() {}
	virtual ~IScene() = default;
	static Scene SceneNo; // 現在のシーン番号
	static Scene PreSceneNO;
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	Scene GetSceneNo() const { return SceneNo; } // 現在のシーン番号を取得
	void SetPreSceneNo(Scene scene) { PreSceneNO = scene; }

private:
};

class sceneManage { // シーン管理クラス
public:
	sceneManage();
	~sceneManage();
	void Initialize();
	void Update();
	void Draw();

private:
	std::unique_ptr<IScene> SceneArr_[static_cast<int>(Scene::SceneNem)]; // シーンの配列
	Scene currentSceneNo_;                                                // 現在のシーン番号
	Scene preSceneNo_;                                                    // 前のシーン番号

};
