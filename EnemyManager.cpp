#include "EnemyManager.h"

EnemyManager::EnemyManager()
{
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::PushEnemyPumpkin(Vector2 pos, Camera* camera)
{
	EnemyPumpkin* newEnemy = new EnemyPumpkin();
	newEnemy->Initialize(camera, pos);
	enemies_.push_back(newEnemy);

}

void EnemyManager::UpDate()
{
	for (Enemies* enemy : enemies_) {
		if (!enemy) {
			continue;
		}
		enemy->Update();
	}
}

void EnemyManager::Draw()
{
	for (auto* enemy : enemies_) {
		if (!enemy) {
			continue;
		}
		enemy->Draw();
	}
}

void EnemyManager::setPlayer(Player* player)
{
	player_ = player;
}
