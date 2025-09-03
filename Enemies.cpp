#include "Enemies.h"



Enemies::~Enemies()
{
}





EnemyPumpkin::EnemyPumpkin()
{
}

EnemyPumpkin::~EnemyPumpkin()
{
}

void EnemyPumpkin::Initialize(Camera* camera, Vector2& pos)
{
	sprite = new DrawSprite(Novice::LoadTexture("white1x1.png"), { 64,64 });
	sprite->SetColor(0xff7d00ff);
	camera_ = camera;
	wtf.Initialize();
	wtf.translation_ = pos;

}

void EnemyPumpkin::Update()
{
	wtf.Update();
}

void EnemyPumpkin::Draw()
{
	sprite->Draw(wtf, camera_, 0, 0, 64, 64);
}
void Enemies::MapColdlision(CollisonMapInfo& info)
{
	info;
}
