#pragma once
#pragma once
#include "Fade.h"
#include "MapChipField.h"
#include "Player.h"
#include "SceneManager.h"
#include <vector>
#include"WtF.h"
#include"DrawFunc.h"
#include"Camera.h"
#include "EnemyManager.h"
#include"MapBlockManager.h"
#include "BGM.h"
class Game : public IScene {
public:
	Game();
	~Game() override;
	// 初期
	virtual void Initialize() override;
	// 更新
	void Update() override;
	// 描画
	void Draw() override;
	// ブロックの生成
	void GenerateBlocks();
	void CheckAllCollisions();
	enum class Phase { FadeIn, kPlay, kDeath, kFadeOut };
	void ChangePhase(); // フェーズを変更する

	struct BlockManager {
		WtF* wtf = nullptr;
		MapChipType mapChipType;
	};

protected:
	Camera* camera_;
	MapChipField* mapChipField_;
	MapChipField* backGroundMapChipField_;
	MapBlockManager* blockManger;
	MapBlockManager* BackGroundBlockManger;
	Player* player_ = nullptr;// プレイヤー
	EnemyManager enemyManager;
	Phase phase_ = Phase::FadeIn;                             // ゲームのフェーズ
	Fade* fade_ = nullptr;                       // フェードイン・フェードアウト用のオブジェクト
	BGM* bgm_ = nullptr;
};

class Stage1 :public Game
{
public:
	Stage1() {}
	~Stage1() {}

	void Initialize()override;

private:

};
