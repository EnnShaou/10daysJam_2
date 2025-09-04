#include "EnemyManager.h"
#include "Player.h"

EnemyManager::EnemyManager()
{
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::PushEnemyPumpkin(Vector2 pos, Camera* camera, MapChipField* mapChipField)
{
	EnemyPumpkin* newEnemy = new EnemyPumpkin();
	newEnemy->Initialize(camera, pos, mapChipField);
	newEnemy->SetPlayer(player_);
	enemies_.push_back(newEnemy);

}

void EnemyManager::UpDate(Player* player)
{
	for (Enemies* enemy : enemies_) {
		if (!enemy) {
			continue;
		}
		enemy->Update(player);
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
