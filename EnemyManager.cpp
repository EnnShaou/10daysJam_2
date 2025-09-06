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

void EnemyManager::PushEnemyLamp(Vector2 pos, Camera* camera, MapChipField* mapChipField)
{
	EnemyLamp* newEnemy = new EnemyLamp();
	newEnemy->Initialize(camera, pos, mapChipField);
	newEnemy->SetPlayer(player_);
	enemies_.push_back(newEnemy);
}

void EnemyManager::PushEnemyBat(Vector2 pos, Camera* camera, MapChipField* mapChipField)
{
	EnemyBat* newEnemy = new EnemyBat();
	newEnemy->Initialize(camera, pos, mapChipField);
	newEnemy->SetPlayer(player_);
	enemies_.push_back(newEnemy);
}

void EnemyManager::PushEnemyMummy(Vector2 pos, Camera* camera, MapChipField* mapChipField)
{
	EnemyMummy* newEnemy = new EnemyMummy();
	newEnemy->Initialize(camera, pos, mapChipField);
	newEnemy->SetPlayer(player_);
	enemies_.push_back(newEnemy);
}

void EnemyManager::Update()
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
bool EnemyManager::isPushButton(BlockButtonAndGate* button)
{
	for (auto* enemy : enemies_)
	{
		if (enemy->isPushButton(button)) {
			return true; 
		}
	}
	return false;
}
