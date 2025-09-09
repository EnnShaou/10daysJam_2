#pragma once
#include "Camera.h"
#include "Wtf.h"
#include "DrawFunc.h"

class PlayerBullets
{
public:
	// --- 初期化 ---
	void Initialize(Camera* camera, Vector2 pos,Vector2 dir);

	// --- デストラクタ ---
	~PlayerBullets();

	// --- 毎フレームの更新処理 ---
	void Update();

	// --- 描画処理 ---
	void Draw();

	// --- 発射開始 ---
	void Launch();

	void Animation();

	// --- ゲッター ---
	bool IsDead() const { return isDead_; }
	Vector2 GetPos() const { Vector2 worldPos = worldTransform_.translation_; return worldPos; }
	Vector2 GetSize() const { return Vector2(kWidth, kHeight); }
	void SetPosition(const Vector2& pos) { worldTransform_.translation_ = pos; worldTransform_.Update();}

	// --- セッター ---
	void SetDead(bool isDead) { isDead_ = isDead; }

private:
	// --- トランスフォーム・カメラ・スプライト ---
	DrawSprite* sprite = nullptr; // プレイヤーの弾スプライト
	WtF worldTransform_;          // ワールドトランスフォーム
	Camera* camera_ = nullptr;    // カメラ参照

	// --- 定数 ---
	static inline const float kWidth = 30.0f;  // 当たり判定幅
	static inline const float kHeight = 30.0f; // 当たり判定高さ
	static inline const float kBlank = 2.0f;   // 当たり判定余白

	// --- 速度 ---
	Vector2 vel_ = {}; // 弾の速度

	// --- 状態フラグ ---
	bool isActive_ = true; // 弾が有効かどうか
	bool isDead_ = false;  // 弾が寿命切れかどうか

	// --- タイマー ---
	float frameTime = 1.0f / 60.0f; // フレーム時間
	float lifeTime_ = 5.0f;         // 弾の寿命（秒）

	// --- 向き ---
	Vector2 dir_ = {};

	

	int animationTimer = 0;
	int animationMax = 3;
	int imageWidth = 32;
	int imageHeight = 32;
	int animePosX = 0;
};