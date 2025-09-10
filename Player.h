#pragma once
#include "Camera.h"
#include "Vector2.h"
#include "DrawFunc.h"
#include "Wtf.h"
#include "PlayerBulletManager.h"
#include"MapBlockManager.h"
#include "UI.h"
class Enemies;
class MapChipField;
class Player
{
public:
	Player();
	~Player();

	// --- プレイヤーの向き ---

	// --- マップ衝突情報構造体 ---
	struct CollisonMapInfo
	{
		bool Top = false;          // 上方向の衝突
		bool Bottom = false;       // 下方向の衝突
		bool LR = false;           // 左右方向の衝突
		Vector2 vel;               // 移動量（速度ベクトル）
	};

	// --- アニメーションの行動パターン ---
	enum class AnimationBehavior
	{
		kRoot,
		kMove,
		kJumpUp,
		kJumpDown,
		kDamage, 
		kAstralBodyIdle,
		kDeath,
		kAstralRoot,     // 幽体通常状態
		kAstralAttack,   // 幽体攻撃
		kAstralDeath,    // 幽体死亡
		kUnknown
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
		kKnockback,
		kDead,
		kUnknown
	};

	// --- 幽体状態の行動パターン ---
	enum class AstralBehavior
	{
		kRoot,
		kAttack,
		kKnockback,
		kUnknown
	};

	// --- 初期化 ---
	void Initialize(Camera* camera_, Vector2& pos);

	// --- 毎フレームの更新処理 ---
	void Update();

	// --- 描画処理 ---
	void Draw();

	// --- 移動処理 ---
	void Move();                                                           // 通常移動
	void AstralMove();                                                     // 幽体状態の移動
	Vector2 InputMove(bool allowVertical, bool& moving, DrawSprite::LRDirection& direction); // 入力検知

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
	void OnCollisionNomal(const Enemies* enemies);

	void OnCollisionAstral(const Enemies* enemies);



	// --- 行動パターン初期化・更新 ---
	void BehaviorRootInitialize(); 
	void BehaviorRootUpdate();

	void BehaviorAttackInitialize();
	void BehaviorAttackUpdate();

	void BehaviorAstralInitialize();
	void BehaviorAstralUpdate();

	void BehaviorKnockbackInitialize();
	void BehaviorKnockbackUpdate();

	void BehaviorDeadInitialize();
	void BehaviorDeadUpdate();

	// --- 幽体状態の切り替え処理 ---
	void SwitchBody();

	// --- 幽体状態の行動（ルート、攻撃）初期化・更新 ---
	void AstralBodyBehaviorRootInitialize();
	void AstralBodyBehaviorRootUpdate();

	void AstralBodyBehaviorAttackInitialize();
	void AstralBodyBehaviorAttackUpdate();

	void AstralBodyBehaviorKnockbackInitialize();
	void AstralBodyBehaviorKnockbackUpdate();

	// --- アニメーション関連 ---
	void Animation();

	bool isPushButton(BlockButton* button);

	// --- ゲッター ---
	Vector2 GetPos() const { Vector2 worldPos = worldTransform_.translation_; return worldPos; }                   
	Vector2 GetTentativePos() const { Vector2 worldPos = tentativeWorldTransform_.translation_; return worldPos; } 
	Vector2 GetScale() const { Vector2 worldScale = worldTransform_.scale_; return worldScale; }                   
	float getRotation() const { float worldRotation = worldTransform_.rotation_; return worldRotation; }           
	Vector2& GetVel() { return vel_; }                                                                             
	Vector2 GetDir() const { return lrDirection_ == DrawSprite::LRDirection::kRight ? Vector2(1, 0) : Vector2(-1, 0); }
	Behavior& GetBehavior()  { return behavior_; }                                                                 
	bool IsDead() const { return isDead_; }                                                                        
	bool IsClear() const { return isClear; }                                                                       
	Vector2 GetNomalSize() const { return nomalSize; }

	Vector2 GetAstralSize() const { return astralSize; }

	bool IsAstral() const { return isAstral; }                                                                     
	PlayerBulletManager& GetBullets()  { return playerBullets_; } 

	int GetHp() const { return nomalBodyHP; }

	float GetAstralTimer() const { return astralBodyTimer_; }

	float GetCoolTime() const { return coolTime_; }
	
	// --- セッター ---
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	bool GetInvincible() const { return isInvincible_; }

	

private:
	// --- 弾関連 ---
	PlayerBulletManager playerBullets_; // プレイヤーの弾
	int currentBullets_ = 0;            // 現在撃っている弾の数
	const int maxBullets_ = 5;          // 1度に撃てる弾の最大数

	// --- トランスフォーム・カメラ ---
	WtF worldTransform_;          // ワールドトランスフォーム
	WtF tentativeWorldTransform_; // 仮のワールドトランスフォーム
	Camera* camera_ = nullptr;    // カメラポインタ

	// --- 速度・向き ---
	Vector2 vel_;                 // 移動速度
	DrawSprite::LRDirection lrDirection_ = DrawSprite::LRDirection::kRight;
	DrawSprite::LRDirection templrDirection_ = DrawSprite::LRDirection::kRight;

	// --- 定数 ---
	static inline const float kPlayerSpeed = 1.0f;                  // プレイヤーの基本移動速度
	static inline const float kPlayerSpeedMax = kPlayerSpeed * 5.f; // 移動速度の最大値
	static inline const float kGravity = 0.98f;                     // 重力加速度
	static inline const float kLimitFallSpeed = kGravity * 20.f;    // 落下速度制限
	static inline const float kJumpAcceleration = 15.00f;           // ジャンプの加速度
	static inline const float kWidth = 30;                          // 当たり判定幅
	static inline const float kHeight = 60;                         // 当たり判定高さ
	static inline const float kBlank = 2.f;                         // 当たり判定余白
	static inline const float kAstralBodyMaxDistance_ = 500.0f;     // 幽体状態の最大移動距離

	// --- 状態フラグ ---
	bool onGround = false;             // 地面に接地しているか
	bool isDead_ = false;              // 死亡状態
	bool isMove = false;               // 移動中かどうか
	bool isClear = false;              // ステージクリアしたかどうか
	bool isAstral = false;             // 幽体状態かどうか
	bool pendingAstralDamage_ = false; // ダメージを減らす状態かどうか
	bool deathAnimEnded_ = false;      // アニメーションが終わっているかどうか

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
	int damageCooldown_ = 0;
	const int damageCooldownMax = 30;
	float deathCount;

	int deathWaitTimer_ = 0;

	float legSpeed = 0.05f;

	// --- スプライト ---
	DrawSprite* playerSprite_ = nullptr;
	DrawSprite* astralBodySprite_ = nullptr;

	// --- 幽体状態関連 ---
	float astralBodyTimer_ = 0.0f;           // 幽体状態の残り時間タイマー
	const float astralBodyDuration_ = 10.0f; // 幽体状態持続時間
	float coolTime_ = 3.0f;                  // 幽体状態になるまでのクールタイム

	// --- 体力 ---
	static inline const int maxNomalBodyHP = 3;  // 通常状態の最大体力
	static inline const int maxAstralBodyHP = 1; // 幽体状態の最大体力
	int nomalBodyHP = maxNomalBodyHP;            // 通常状態の体力
	int astralBodyHP = maxAstralBodyHP;          // 幽体状態の体力

	// --- アニメーション ---
	AnimationBehavior animationBehavior_ = AnimationBehavior::kRoot;
	AnimationBehavior animationBehaviorNext_ = AnimationBehavior::kUnknown;
	int animationCount;
	int animationTimer;
	int animationMax = 4;

	float animationLagTime = 0.3f;

	// --- ノックバック ---
	bool isKnockback_ = false; 
	Vector2 knockback_ = { 5.2f, 0.0f };
	float knockbackTimer_ = 0.0f;

	// --- 当たり判定 ---
	Vector2 nomalSize = { 25.0f,50.0f };
	Vector2 astralSize = { 45.0f,45.0f };

	const float knockbackDuration_ = 0.5f; // ノックバックが続く秒数

	// 無敵時間
	int invincibleTimer_ = 0;
	const int maxInvincibleTimer = 90;
	bool isInvincible_ = false;

	void Invincible();

	// オーディオファイル
	int bodyDamagedSFX;
	int ghostDamagedSFX;
	int shootSFX;
	int switchBodySFX;
	int jumpSFX;

	// UI
	UI* ui_ = nullptr;

};
