#include "MapBlockManager.h"

Block::Block()
{
}

Block::~Block()
{

}

void Block::Initialize(Vector2 pos)
{

	sprite == new DrawSprite(Novice::LoadTexture("white1x1.png"), { 64,64 });
	wtf_.Initialize();
	wtf_.translation_ = pos;
}

void Block::Update()
{
	wtf_.Update()
}

void Block::Draw(Camera* camera)
{
	sprite->Draw(wtf_, camera, 0, 0, 64, 64);
}

void MapBlockManager::pushBlock(Vector2 pos)
{

}

void MapBlockManager::Update()
{
}

void MapBlockManager::Draw()
{
}
