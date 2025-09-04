#include "PlayerBulletManager.h"

void PlayerBulletManager::PushBullet(Vector2 pos, Camera* camera, Vector2 dir)
{
	// 新しい弾を生成してリストに追加
	auto newBullet = std::make_unique<PlayerBullets>();
	newBullet->Initialize(camera, pos,dir);
	playerBullets_.push_back(std::move(newBullet));
}

void PlayerBulletManager::Update()
{
	// 全ての弾を更新
	for (auto& bullet : playerBullets_) 
	{
		bullet->Update();
	}

	// 寿命が尽きた弾をリストから削除
	playerBullets_.remove_if([](const std::unique_ptr<PlayerBullets>& bullet) 
	{
		return bullet->IsDead();
	});
}

void PlayerBulletManager::Draw()
{
	// 全ての弾を描画
	for (const auto& bullet : playerBullets_) 
	{
		bullet->Draw();
	}
}
