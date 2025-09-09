#pragma once
#include"Vector2.h"
#include"DrawFunc.h"
#include"WtF.h"
#include"Camera.h"
#include"MapChipField.h"
#include"Player.h"
#include"MapBlockManager.h"
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

	// ワールドトランスフォームのゲッター
	virtual Vector2 GetPos() const { Vector2 worldPos = wtf.translation_; return worldPos; }

	// サイズのゲッター
	virtual Vector2 GetSize() const { return { kWidth, kHeight }; }

	// 当たり判定
	virtual void OnCollision() = 0;

	// --- 衝突判定関連 ---
	void MapCollision(CollisonMapInfo& info);   // マップ全体との衝突判定

	//void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }
	void SetPlayer(Player* player) { player_ = player; }

	virtual bool isPushButton(BlockButton* button);
protected:
	MapChipField* mapChipField_;  // マップフィールドポインタ
	DrawSprite* sprite = nullptr; // 敵スプライト
	WtF wtf;                      // 座標など
	Camera* camera_ = nullptr;    // カメラ参照
	Player* player_ = nullptr;    // プレイヤー

	// 当たり判定のサイズ
	float kWidth = 60;  // 当たり判定の幅
	float kHeight = 60; // 当たり判定の高さ
	float kBlank = 2.f; // 当たり判定の余白（境界ずれ防止）

	Vector2 vel_;                                   //　速度
	const float kGravity = 0.98f;                   //　重力
	const float kLimitFallSpeed = kGravity * 20.f;  //　落下速度制限

	//void GroundStates(const CollisonMapInfo& info);      // 地面との接地状態更新

	// --- マップ ---
	void MapCollisionTop(CollisonMapInfo& info);            // 上方向の衝突判定
	void MapCollisionBottom(CollisonMapInfo& info);         // 下方向の衝突判定
	virtual void MapCollisionLeft(CollisonMapInfo& info);   // 左方向の衝突判定
	virtual void MapCollisionRight(CollisonMapInfo& info);  // 右方向の衝突判定
	void GroundStates(const CollisonMapInfo& info);         // 地面との接地状態の更新
	virtual void MapWallCollision(CollisonMapInfo& info);   // 壁との衝突判定
	void MapAfterCollision(const CollisonMapInfo& info);    // 衝突後の状態処理
	virtual void InputGravity(const CollisonMapInfo& info); // 重力処理

	// --- 当たり判定のコーナー位置取得 ---
	Vector2 CornerPos(const Vector2 center, Corner corner);
	bool onGround = false;  // 地面に接地しているか

	// ビヘイビア管理用の関数
	enum class Behavior
	{
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

	// 左右方向
	DrawSprite::LRDirection lrDirection_ = DrawSprite::LRDirection::kRight;

	// アニメーション処理
	virtual void Animation();
	int animationTimer_ = 0;	// アニメーションのタイマー
	int animationMax_ = 4;		// アニメーションの最大画像枚
	int imageWidth_ = 64;		// 画像の横幅サイズ
	int imageHeight_ = 64;		// 画像の縦幅サイズ
	int animePosX_ = 0;			// 画像アニメーションのx位置
	int animePosY_ = 0;			// 画像アニメーションのy位置

	int testX = 0;
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

	// 当たり判定
	void OnCollision() override;
	bool isPushButton(BlockButton* button)override;

	// かぼちゃの速度のゲッター
	float GetVelocity() const { return vel_.y; }
private:
	// ここにかぼちゃ敵固有の変数や処理を追加できる
	void InputGravity(const CollisonMapInfo& info)override;
	void Animation() override;
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

	// 当たり判定
	void OnCollision() override;
	float getRadius() {
		return lightRadius_;
	}

private:
	// 照らす範囲
	float lightRadius_ = 280.0f;

	// アニメーション処理
	void Animation() override;
};

// ------------------ コウモリ（派生） ------------------
class EnemyBat : public Enemies
{
public:
	EnemyBat();
	~EnemyBat();

	// 初期化（カメラ、初期位置）
	void Initialize(Camera* camera, Vector2& pos, MapChipField* mapChipField) override;

	// 更新処理
	void Update() override;

	// 描画処理
	void Draw() override;
	// 当たり判定
	void OnCollision() override;

	bool GetIsDead() { return isDead_;}

private:

	// コウモリの行動
	enum class BatBehavior {
		kNormal,
		kAttack,
		kDead,
		kUnknown,
	};

	BatBehavior behavior_ = BatBehavior::kNormal;
	BatBehavior behaviorNext_ = BatBehavior::kUnknown;

	// プレイヤーの位置とその距離をゲットする関数
	Vector2 GetPlayerPos();
	float GetDistanceToPlayer(const Vector2& playerPos);
	
	void BehaviorNormalInitialize();
	void BehaviorNormalUpdate();
	void BehaviorAttackInitialize();
	void BehaviorAttackUpdate();
	void BehaviorDeadInitialize();
	void BehaviorDeadUpdate();


	Vector2 spawnPos_;					// スポーン位置
	const float maxMovementX_ = 200.0f; // 待機時の最大移動量
	Vector2 currentSpeed_ = { kSpeed.x, kSpeed.y };

	// アニメーション
	void Animation() override;

	bool isDead_ = false;
};

// ------------------ ミイラ（派生） ------------------
class EnemyMummy : public Enemies
{
public:
	EnemyMummy();
	~EnemyMummy();

	// 初期化（カメラ、初期位置）
	void Initialize(Camera* camera, Vector2& pos, MapChipField* mapChipField) override;

	// 更新処理
	void Update() override;

	// 描画処理
	void Draw() override;

	// 当たり判定
	void OnCollision() override;

	bool GetIsStun() { return isStan; }
	bool isPushButton(BlockButton* button)override;

private:
	void MapWallCollision(CollisonMapInfo& info) override;        // 壁との衝突判定
	void MapCollisionRight(CollisonMapInfo& info) override;
	void MapCollisionLeft(CollisonMapInfo& info) override;

	// --- フラグ・タイマー ---
	bool isStan = false;       // 気絶フラグ
	float stanTimer = 0.0f;    // 気絶タイマー
	float stanDuration = 5.0f; // 気絶時間
	// --- アニメーションの行動パターン ---
	enum class AnimationBehavior
	{
		kMove,		// 歩いているとき
		kStop,		// スターンされたとき・プレイヤーに攻撃を受けたとき
		kUnknown
	};

	AnimationBehavior animationBehavior_ = AnimationBehavior::kMove;
	void Animation() override;

};