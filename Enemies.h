#pragma once
#include"Vector2.h"
#include"DrawFunc.h"
#include"WtF.h"
#include"Camera.h"
#include"MapChipField.h"
#include"Player.h"

// ------------------ 敵の基底クラス ------------------
class Enemies
{
public:
	Enemies() = default;
	~Enemies();

	// 矩形の四隅を示す列挙型（当たり判定用）
	enum Corner
	{
		kRightBottom,   // 右下
		kLeftBottom,    // 左下
		kLeftTop,       // 左上
		kRightTop,      // 右上
		kNumCorner      // 隅の数
	};

	// マップ衝突情報
	struct CollisonMapInfo
	{
		bool Top = false;      // 上方向との衝突
		bool Bottom = false;   // 下方向との衝突（地面）
		bool LR = false;       // 左右方向との衝突
		Vector2 vel;           // 移動量（速度ベクトル）
	};

	// 初期化（カメラ、初期位置）
	virtual void Initialize(Camera* camera, Vector2& pos, MapChipField* mapChipField) = 0;

	// 更新処理
	virtual void Update() = 0;

	// 描画処理
	virtual void Draw() = 0;

	// --- 衝突判定関連 ---
	void MapCollision(CollisonMapInfo& info);   // マップ全体との衝突判定

	//void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }
	void SetPlayer(Player* player) { player_ = player; }

protected:
	MapChipField* mapChipField_; // マップフィールドポインタ
	DrawSprite* sprite = nullptr;   // 敵スプライト
	WtF wtf;                        // 座標など
	Camera* camera_ = nullptr;      // カメラ参照
	Player* player_ = nullptr;


	// 当たり判定のサイズ
	float kWidth = 60;              // 当たり判定の幅
	float kHeight = 60;             // 当たり判定の高さ
	float kBlank = 2.f;             // 当たり判定の余白（境界ずれ防止）

	Vector2 vel_;                                   //　速度
	const float kGravity = 0.98f;                   //　重力
	const float kLimitFallSpeed = kGravity * 20.f;  //　落下速度制限

	//void GroundStates(const CollisonMapInfo& info);      // 地面との接地状態更新
	// --- マップ ---
	// 以下は詳細な方向別の衝突判定処理（現状コメントアウト）
	void MapCollisionTop(CollisonMapInfo& info);         // 上方向の衝突判定
	void MapCollisionBottom(CollisonMapInfo& info);      // 下方向の衝突判定
	void MapCollisionLeft(CollisonMapInfo& info);        // 左方向の衝突判定
	void MapCollisionRight(CollisonMapInfo& info);       // 右方向の衝突判定
	void GroundStates(const CollisonMapInfo& info);      // 地面との接地状態の更新
	void MapWallCollision(CollisonMapInfo& info);        // 壁との衝突判定
	void MapAfterCollision(const CollisonMapInfo& info); // 衝突後の状態処理
	virtual void InputGravity(const CollisonMapInfo& info);
	// --- 当たり判定のコーナー位置取得 ---
	Vector2 CornerPos(const Vector2 center, Corner corner);
	bool onGround = false;  // 地面に接地しているか

	// ビヘイビア管理用の関数
	enum class Behavior {
		kStop,
		kAstral,
		kUnknown,
	};

	virtual void AstralBehavior();
	virtual void AstralBehaviorInitialize();
	virtual void StopBehavior();
	virtual void StopBehaviorInitialize();

	Vector2 kSpeed = { 2.0f, 2.0f };  //かぼちゃの速さ
	float kAtkRange = 240.0f;         //プレイヤーがこの範囲にいると動く

	Behavior behavior_ = Behavior::kStop;
	Behavior behaviorNext_ = Behavior::kUnknown;
};


// ------------------ かぼちゃ敵クラス（派生） ------------------
class EnemyPumpkin : public Enemies
{
public:
	EnemyPumpkin();
	~EnemyPumpkin();

	// 初期化（カメラ、初期位置）
	void Initialize(Camera* camera, Vector2& pos, MapChipField* mapChipField) override;

	// 更新処理
	void Update() override;

	// 描画処理
	void Draw() override;

	Vector2 GetPos() { return wtf.translation_; }


private:
	// ここにかぼちゃ敵固有の変数や処理を追加できる
	void InputGravity(const CollisonMapInfo& info)override;
};


// ------------------ 灯敵クラス（派生） ------------------
class EnemyLamp : public Enemies
{
public:
	EnemyLamp();
	~EnemyLamp();

	// 初期化（カメラ、初期位置）
	void Initialize(Camera* camera, Vector2& pos, MapChipField* mapChipField) override;

	// 更新処理
	void Update() override;

	// 描画処理
	void Draw() override;

	Vector2 GetPos() { return wtf.translation_; }

private:

	float lightRadius_ = 280.0f;
};

// ------------------ コウモリ（派生） ------------------
class EnemyBat : public Enemies {
public:
	EnemyBat();
	~EnemyBat();

	// 初期化（カメラ、初期位置）
	void Initialize(Camera* camera, Vector2& pos, MapChipField* mapChipField) override;

	// 更新処理
	void Update() override;

	// 描画処理
	void Draw() override;

private:
};