#pragma once
#include "PlayerBullets.h"
#include <memory>
#include <list>
#include <vector>

class PlayerBulletManager
{
public:
    PlayerBulletManager() {};

	// --- 初期化処理 ---
    void Initialize(Vector2 offsetCenter, Camera* camera);

	// --- 更新処理 ---
    void Update(const Vector2& playerPos);

	// --- 描画処理 ---
    void Draw();

	// --- 弾発射処理 ---
	void Shot(Vector2 dir);

	// --- 弾削除処理 ---
	void ClearHoldingBullets();

	// --- ゲッター ---
	const std::list<std::unique_ptr<PlayerBullets>>& GetPlayerBullets() const { return activeBullets_; }

private:
	std::vector<std::unique_ptr<PlayerBullets>> holdingBullets_;  // 待機弾
	std::list<std::unique_ptr<PlayerBullets>> activeBullets_;     // 発射弾

	// --- カメラ ---
	Camera* camera_ = nullptr;
};

