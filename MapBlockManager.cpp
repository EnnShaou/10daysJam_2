#include "MapBlockManager.h"
#include"keys.h"
#include"Player.h"
#include"EnemyManager.h"
void MapBlockManager::pushBlock(Block* block, Vector2 pos, uint32_t x, uint32_t y)
{
	block_[y][x] = block;
	block_[y][x]->setMapChipField(mapChipField_);
	block_[y][x]->Initialize(pos);
	block_[y][x]->setPlayer(player_);
	block_[y][x]->setEnemies(enemies_);
}

void MapBlockManager::Update()
{
	for (std::vector<Block*>& wtfby : block_) {
		for (Block* wtfb : wtfby) {
			if (wtfb)
			{
				wtfb->Update();
			}
		}
	}
}

void MapBlockManager::Draw()
{
	for (std::vector<Block*>& wtfby : block_) {
		for (Block* wtfb : wtfby) {
			if (wtfb) {
				wtfb->Draw(camera_);
			}

		}
	}
}

void Block::Initialize(Vector2 pos)
{

	sprite = new DrawSprite(Novice::LoadTexture("white1x1.png"), { 64,64 });
	wtf_.Initialize();
	wtf_.translation_ = pos;

}

void Block::Update()
{
	wtf_.Update();
}

void Block::Draw(Camera* camera)
{
	sprite->Draw(wtf_, camera, 0, 0, 64, 64);
}


void Gate::Initialize(Vector2 pos) {
	wtf_.translation_ = pos;
	drawWtf.translation_ = pos;
	sprite = new DrawSprite(Novice::LoadTexture("white1x1.png"), { 64,64 });
	sprite->SetColor(BLUE);
	gateIndex = 0;

	auto index = mapChipField_->GetMapChipIndexByPosition(wtf_.translation_);
	auto type = mapChipField_->GetMapChipTypeIndex(index.xIndex, index.yIndex);
	while (type != MapChipType::kBlock) {
		gateIndex++;
		type = mapChipField_->GetMapChipTypeIndex(index.xIndex, index.yIndex + gateIndex);
	}
	sprite->SetSize({ sprite->GetSize().x,gateIndex * 64.f });
}

void Gate::Update() {
	drawWtf.translation_.y = wtf_.translation_.y - (gateIndex - 1) * 32;
	if (Keys::IsTrigger(DIK_L))
	{
		Close();
	}
	if (Keys::IsTrigger(DIK_O))
	{
		Open();
	}


	wtf_.Update();
	drawWtf.Update();
}

void Gate::Draw(Camera* camera) {
	if (sprite) {
		if (isOpen_) {
			sprite->SetColor(0x33333388);
		}
		else
		{
			sprite->SetColor(BLUE);
		}
		sprite->Draw(drawWtf, camera, 0, 0, 64, 64);
	}
}

void Gate::Open() {

	auto index = mapChipField_->GetMapChipIndexByPosition(wtf_.translation_);
	auto type = mapChipField_->GetMapChipTypeIndex(index.xIndex, index.yIndex);
	if (type == MapChipType::kBlank)
	{
		return;
	}
	int gateIndexOpen = 0;
	while (type != MapChipType::kBlank) {
		mapChipField_->setMapChipData(MapChipType::kBlank, index.xIndex, index.yIndex + gateIndexOpen);
		gateIndexOpen++;
		type = mapChipField_->GetMapChipTypeIndex(index.xIndex, index.yIndex + gateIndexOpen);
		if (gateIndexOpen == gateIndex)
		{
			break;
		}
	}
	isOpen_ = true;
}

void Gate::Close() {
	auto index = mapChipField_->GetMapChipIndexByPosition(wtf_.translation_);
	auto type = mapChipField_->GetMapChipTypeIndex(index.xIndex, index.yIndex);
	if (type == MapChipType::kBlock)
	{
		return;
	}

	int gateIndexClose = 0;
	while (type != MapChipType::kBlock) {
		mapChipField_->setMapChipData(MapChipType::kBlock, index.xIndex, index.yIndex + gateIndexClose);
		gateIndexClose++;
		type = mapChipField_->GetMapChipTypeIndex(index.xIndex, index.yIndex + gateIndexClose);
		if (gateIndexClose == gateIndex)
		{
			break;
		}
	}
	isOpen_ = false;
}

void BlockButtonAndGate::Initialize(Vector2 pos) {
	wtf_.translation_ = pos;
	sprite = new DrawSprite(Novice::LoadTexture("white1x1.png"), { 64,64 });
	sprite->SetColor(RED);
}

void BlockButtonAndGate::Update() {
	bool pressedNow = false;
	if (player_ && player_->isPushButton(this)) pressedNow = true;
	if (!pressedNow && enemies_ && enemies_->isPushButton(this)) pressedNow = true;

	if (pressedNow && !isPressed_) {
		isPressed_ = true;
		if (gate_) gate_->Open();
	}
	else if (!pressedNow && isPressed_) {
		isPressed_ = false;
		if (gate_) gate_->Close();
	}
	wtf_.Update();
}

void BlockButtonAndGate::Draw(Camera* camera) {
	if (sprite) {
		sprite->Draw(wtf_, camera, 0, 0, 64, 64);
	}
}
void MapBlockManager::BindButtonAndGates() {
	for (auto& row : block_) {
		for (Block* b : row) {
			if (auto* button = dynamic_cast<BlockButtonAndGate*>(b)) {
				int id = button->getBindID();
				if (id < 0) continue;

				for (auto& row2 : block_) {
					for (Block* g : row2) {
						if (auto* gate = dynamic_cast<Gate*>(g)) {
							if (gate->getBindID() == id) {
								button->setGate(gate);
							}
						}
					}
				}
			}
		}
	}
}