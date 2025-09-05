#pragma once
#include "PlayerBullets.h"
#include <memory>
#include <list>

class PlayerBulletManager
{
public:
    PlayerBulletManager() {};

	// --- 新しい弾を生成してリストに追加 ---
    void PushBullet(Vector2 pos, Camera* camera, Vector2 vel);

	// --- 弾の更新処理 ---
    void Update();

	// --- 弾の描画処理 ---
    void Draw();

	// --- ゲッター ---
	const std::list<std::unique_ptr<PlayerBullets>>& GetPlayerBullets() const { return playerBullets_; }

private:
	// --- 弾のリスト（全ての弾を格納） ---
    std::list<std::unique_ptr<PlayerBullets>> playerBullets_;
};

