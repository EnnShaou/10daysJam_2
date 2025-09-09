#include "PlayerBulletManager.h"

void PlayerBulletManager::Initialize(Vector2 offsetCenter, Camera* camera)
{
    camera_ = camera;
    Vector2 dir = { 1.0f, 0.0f }; // 初期の仮方向
    holdingBullets_.clear();      // 保持中の弾リストを初期化

    const int bulletCount = 5;    // 保持する弾の数
    const float radius = 30.0f;   // プレイヤー周囲の半径

    // 弾を円形に配置して生成
    for (int i = 0; i < bulletCount; ++i)
    {
        // 弾を配置する角度（円形に等間隔）
        float angle = (2.0f * 3.14159265f * i) / bulletCount;

        // 円形に配置するためのオフセット
        Vector2 offset =
        {
            cosf(angle) * radius,
            sinf(angle) * radius
        };

        // 新しい弾を生成し初期化
        auto bullet = std::make_unique<PlayerBullets>();
        bullet->Initialize(camera_, offsetCenter + offset, dir); // 仮の位置と方向で初期化
        holdingBullets_.push_back(std::move(bullet));            // 保持リストに追加
    }
}

void PlayerBulletManager::Update(const Vector2& playerPos)
{
    const float radius = 40.0f;   // プレイヤー周囲の半径
    const int bulletCount = static_cast<int>(holdingBullets_.size());

    // 保持中の弾をプレイヤーの周囲に配置（追従させる）
    for (int i = 0; i < bulletCount; ++i)
    {
        float angle = (2.0f * 3.14159265f * i) / bulletCount;

        Vector2 offset =
        {
            cosf(angle) * radius,
            sinf(angle) * radius
        };

        auto& bullet = holdingBullets_[i];
        bullet->SetPosition(playerPos + offset); // プレイヤー位置に応じて追従配置
        bullet->Animation();
    }


    // 発射された弾の更新処理
    for (auto& bullet : activeBullets_)
    {
        bullet->Update();
    }

    // 死亡した弾をリストから削除
    activeBullets_.remove_if([](const std::unique_ptr<PlayerBullets>& bullet)
    {
        return bullet->IsDead();
    });
}

void PlayerBulletManager::Draw()
{
    // 保持中の弾を描画
    for (const auto& bullet : holdingBullets_)
    {
        bullet->Draw();
    }

    // 発射済みの弾を描画
    for (const auto& bullet : activeBullets_)
    {
        bullet->Draw();
    }
}

void PlayerBulletManager::Shot(Vector2 dir)
{
    // 保持中の弾がある場合に発射可能
    if (!holdingBullets_.empty())
    {
        auto& bullet = holdingBullets_.front(); // 先頭の弾を取得（発射対象）

        // 発射用に新たな弾を生成・初期化
        auto launchedBullet = std::make_unique<PlayerBullets>();
        launchedBullet->Initialize(camera_, bullet->GetPos(), dir);  // 現在位置と指定方向で初期化
        launchedBullet->Launch();                                    // 発射状態に変更

        activeBullets_.push_back(std::move(launchedBullet));         // 発射済みリストに追加
        holdingBullets_.erase(holdingBullets_.begin());              // 保持中リストから削除
    }
}

void PlayerBulletManager::ClearHoldingBullets()
{
    // 保持中の弾を全て死亡状態に
    for (auto& bullet : holdingBullets_)
    {
        bullet->SetDead(true);
    }
    holdingBullets_.clear();

    // 発射済みの弾も死亡状態に
    for (auto& bullet : activeBullets_)
    {
        bullet->SetDead(true);
    }

    // 死亡した弾をリストから削除
    activeBullets_.remove_if([](const std::unique_ptr<PlayerBullets>& bullet)
    {
        return bullet->IsDead();
    });
}
