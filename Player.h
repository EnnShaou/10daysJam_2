#pragma once
#pragma once
#include"Camera.h"
#include"Vector2.h"
#include"DrawFunc.h"
#include"Wtf.h"
class Enemies; // 前方宣言
class MapChipField;
class PlayerBullet;
class Player {
public:
	Player();

	~Player();
	enum LRDir { kLeft, kRight };
	struct CollisonMapInfo {
		bool Top = false;          // 上
		bool Bottom = false;       // 下
		bool LR = false;           // 左右
		Vector2 vel; // 移動量
	};
	enum Corner {
		kRightBottom,
		kLeftBottom,
		kLeftTop,
		kRightTop,

		kNumCorner

	};
	enum Behavior {
		kRoot,   // ルート
		kAttack, // 攻撃
		kUnknown // 未知の状態

	};
	// 初期
	void Initialize(Camera* camera_, Vector2& pos);
	// 更新
	void Update();
	// 描画
	void Draw();
	void Move(); // プレイヤーの移動処理
	void Turn(); // プレイヤーの向きを変える処理
	//// プレイヤーのワールドトランスフォームを取得
	// KamataEngine::WorldTransform& GetWorldTransform() { return worldTransform_; }
	// posを取得
	Vector2 GetPos() const {
		Vector2 worldPos = worldTransform_.translation_;
		return worldPos;
	}
	// Scaleを取得
	Vector2 GetScale() const {
		Vector2 worldScale = worldTransform_.scale_;
		return worldScale;
	}
	// Rotationを取得
	float getRotation() const {
		float worldRotation = worldTransform_.rotation_;
		return worldRotation;
	}
	// プレイヤーの速度を取得
	Vector2& GetVel() { return vel_; }
	// マップチップフィールドを取得
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }
	// マップとの衝突判定
	void MapCollision(CollisonMapInfo& info);
	// 上の衝突判定
	void MapCollisionTop(CollisonMapInfo& info);
	// 下の衝突判定
	void MapCollisionBottom(CollisonMapInfo& info);
	// 左の衝突判定
	void MapCollisionLeft(CollisonMapInfo& info);
	// 右の衝突判定
	void MapCollisionRight(CollisonMapInfo& info);
	// コーナーの位置を取得
	Vector2 CornerPos(const Vector2 center, Corner corner);
	// マップとの衝突後の移動
	void MapCollisionMove(const CollisonMapInfo& info);
	// マップとの衝突後の処理
	void MapAfterCollision(const CollisonMapInfo& info);
	// 地面との状態を更新
	void GroundStates(const CollisonMapInfo& info);
	//	壁との衝突判定
	void MapWallCollision(CollisonMapInfo& info);
	//MathUtility::AABB GetAABB() const;
	void OnCollision(const Enemies* enemies);
	// プレイヤーが死亡しているかどうかを取得
	bool IsDead() const { return isDead_; }
	void BehaviorRootInitialize(); // ルートの初期化
	void BehaviorRootUpdate();
	// 攻撃の初期化
	void BehaviorAttackInitialize(); // 攻撃の初期化
	void BehaviorAttackUpdate();
	void JumpAnimetion();
	void MoveAnimation();
	PlayerBullet* playerBullet_ = nullptr; // プレイヤーの弾
	bool IsClear() const { return isClear; }

private:
	WtF worldTransform_;          // ワールドトランスフォーム
	Camera* camera_ = nullptr;            // カメラのハンドル
	Vector2 vel_;                         // プレイヤーの移動速度
	LRDir lrDir_ = LRDir::kRight;                       // プレイヤーの向き
	static inline const float kPlayerSpeed = 2.0f;    // プレイヤーの移動速度の減少率
	static inline const float kPlayerSpeedMax = kPlayerSpeed * 2.f;   // プレイヤーの移動速度の最大値
	bool onGround = false;                              // 地面にいるかどうか
	bool isJumping = false;                             // ジャンプ中かどうか
	static inline const float kGravity = 0.98f;         // 重力
	static inline const float kLimitFallSpeed = kGravity * 10.f;   // 落下速度の制限
	static inline const float kJumpAcceleration = 10.00f; // ジャンプの加速度
	MapChipField* mapChipField_ = nullptr;              // マップチップフィールド
	// std::array<KamataEngine::Vector3, kNumCorner> posNew; // コーナーの位置を格納する配列
	static inline const float kWidth = 30;     // 当たり判定の幅
	static inline const float kHeight = 30;    // 当たり判定の高さ
	static inline const float kBlank =1.f;     // 当たり判定の余白
	bool isDead_ = false;                        // プレイヤーが死亡しているかどうか
	Behavior behavior_ = Behavior::kRoot;        // プレイヤーの行動
	Behavior behaviorNext_ = Behavior::kUnknown; // 次のプレイヤーの行動
	float attackTimer = 0.0f;
	const float kAttackTime = 0.5f;
	const float frameTime = 1.0f / 60.0f;
	DrawSprite* playerSprite_ = nullptr; // プレイヤーのスプライト
	float animationTimer = 0;
	bool isMove = false;
	static inline float slideSpeed = 0.0025f;
	bool slide = false;
	float legSpeed = 0.05f;
	bool isClear = false;
};
