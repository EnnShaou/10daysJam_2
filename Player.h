#pragma once
#include "Camera.h"
#include "Vector2.h"
#include "DrawFunc.h"
#include "Wtf.h"

class Enemies;
class MapChipField;
class PlayerBullet;
class Player
{
public:
	Player();
	~Player();

	// --- プレイヤーの向き ---
	enum LRDir { kLeft, kRight };

	// --- マップ衝突情報構造体 ---
	struct CollisonMapInfo
	{
		bool Top = false;          // 上方向の衝突
		bool Bottom = false;       // 下方向の衝突
		bool LR = false;           // 左右方向の衝突
		Vector2 vel;               // 移動量（速度ベクトル）
	};
	enum class AnimationBehavior
	{
		kRoot,
		kMove,
		kJumpUp,
		kJumpDown,
	};
	// --- プレイヤー当たり判定のコーナー ---
	enum Corner
	{
		kRightBottom,
		kLeftBottom,
		kLeftTop,
		kRightTop,
		kNumCorner
	};

	// --- プレイヤーの行動パターン ---
	enum class Behavior
	{
		kRoot,
		kAttack,
		kAstral,
		kUnknown
	};

	// --- 幽体状態の行動パターン ---
	enum class AstralBehavior
	{
		kRoot,
		kAttack,
		kUnknown
	};

	// --- 初期化 ---
	void Initialize(Camera* camera_, Vector2& pos);

	// --- 毎フレームの更新処理 ---
	void Update();

	// --- 描画処理 ---
	void Draw();

	// --- 移動処理 ---
	void Move();

	// --- 向きの変更処理 ---
	void Turn();

	// --- 衝突判定関連 ---
	void MapCollision(CollisonMapInfo& info);            // マップ全体の衝突判定
	void MapCollisionTop(CollisonMapInfo& info);         // 上方向の衝突判定
	void MapCollisionBottom(CollisonMapInfo& info);      // 下方向の衝突判定
	void MapCollisionLeft(CollisonMapInfo& info);        // 左方向の衝突判定
	void MapCollisionRight(CollisonMapInfo& info);       // 右方向の衝突判定
	void MapWallCollision(CollisonMapInfo& info);        // 壁との衝突判定
	void MapCollisionMove(const CollisonMapInfo& info);  // 衝突後の移動処理
	void MapAfterCollision(const CollisonMapInfo& info); // 衝突後の状態処理
	void GroundStates(const CollisonMapInfo& info);      // 地面との接地状態の更新

	// --- 当たり判定のコーナー位置取得 ---
	Vector2 CornerPos(const Vector2 center, Corner corner);

	// --- 敵との衝突判定 ---
	void OnCollision(const Enemies* enemies);

	// --- 行動パターン初期化・更新 ---
	void BehaviorRootInitialize();
	void BehaviorRootUpdate();

	void BehaviorAttackInitialize();
	void BehaviorAttackUpdate();

	void BehaviorAstralInitialize();
	void BehaviorAstralUpdate();

	// --- 幽体状態の切り替え処理 ---
	void SwitchBody();

	// --- 幽体状態の行動（ルート、攻撃）初期化・更新 ---
	void AstralBodyBehaviorRootInitialize();
	void AstralBodyBehaviorRootUpdate();

	void AstralBodyBehaviorAttackInitialize();
	void AstralBodyBehaviorAttackUpdate();

	// --- アニメーション関連 ---

	void Animation();

	// --- ゲッター ---
	Vector2 GetPos() const { Vector2 worldPos = worldTransform_.translation_; return worldPos; }
	Vector2 GetScale() const { Vector2 worldScale = worldTransform_.scale_; return worldScale; }
	float getRotation() const { float worldRotation = worldTransform_.rotation_; return worldRotation; }
	Vector2& GetVel() { return vel_; }

	bool IsDead() const { return isDead_; }
	bool IsClear() const { return isClear; }

	// --- セッター ---
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

private:
	// --- 弾関連 ---
	PlayerBullet* playerBullet_ = nullptr; // プレイヤーの弾

	// --- トランスフォーム・カメラ ---
	WtF worldTransform_;          // ワールドトランスフォーム
	Camera* camera_ = nullptr;    // カメラポインタ

	// --- 速度・向き ---
	Vector2 vel_;                 // 移動速度
	LRDir lrDir_ = LRDir::kRight; // 向き

	// --- 定数 ---
	static inline const float kPlayerSpeed = 1.0f;                  // プレイヤーの基本移動速度
	static inline const float kPlayerSpeedMax = kPlayerSpeed * 5.f; // 移動速度の最大値
	static inline const float kGravity = 0.98f;                     // 重力加速度
	static inline const float kLimitFallSpeed = kGravity * 20.f;    // 落下速度制限
	static inline const float kJumpAcceleration = 15.00f;           // ジャンプの加速度
	static inline const float kWidth = 60;                          // 当たり判定幅
	static inline const float kHeight = 60;                         // 当たり判定高さ
	static inline const float kBlank = 2.f;                         // 当たり判定余白

	// --- 状態フラグ ---
	bool onGround = false;  // 地面に接地しているか
	bool isJumping = false; // ジャンプ中かどうか
	bool isDead_ = false;   // 死亡状態
	bool isMove = false;    // 移動中かどうか
	bool isClear = false;   // ステージクリアしたかどうか

	// --- マップ ---
	MapChipField* mapChipField_ = nullptr; // マップフィールドポインタ

	// --- 行動状態 ---
	Behavior behavior_ = Behavior::kRoot;
	Behavior behaviorNext_ = Behavior::kUnknown;

	AstralBehavior Astralbehavior_ = AstralBehavior::kRoot;
	AstralBehavior AstralbehaviorNext_ = AstralBehavior::kUnknown;

	// --- タイマー・アニメーション ---
	float attackTimer = 0.0f;
	const float kAttackTime = 0.5f;
	const float frameTime = 1.0f / 60.0f;

	float animationTimer = 0;
	float legSpeed = 0.05f;

	// --- スプライト ---
	DrawSprite* playerSprite_ = nullptr;

	// --- 幽体状態関連 ---
	float astralBodyTimer_ = 0.0f;           // 幽体状態の残り時間タイマー
	const float astralBodyDuration_ = 10.0f; // 幽体状態持続時間
	float coolTime_ = 5.0f;                  // 幽体状態になるまでのクールタイム

	// --- 体力 ---
	const float nomalBodyHP = 3.0f;  // 通常状態の体力
	const float astralBodyHP = 1.0f; // 幽体状態の体力

	// --- アニメーション ---
	AnimationBehavior animationBehavior_ = AnimationBehavior::kRoot;
	int animationCount = 0;
};
