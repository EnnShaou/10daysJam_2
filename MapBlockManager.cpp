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
			if (!wtfb)
			{
				continue;
			}
			if (wtfb == dynamic_cast<BackGroundBlock*>(wtfb)) {
				wtfb->Draw(camera_);
			}

		}
	}
	for (std::vector<Block*>& wtfby : block_) {
		for (Block* wtfb : wtfby) {
			if (!wtfb)
			{
				continue;
			}
			if (wtfb != dynamic_cast<BackGroundBlock*>(wtfb)) {
				wtfb->Draw(camera_);
			}

		}
	}
}

void MapBlockManager::BindButtonAndGates() {
	for (auto& row : block_) {
		for (Block* b : row) {
			if (auto* button = dynamic_cast<BlockButton*>(b)) {
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


void Block::Initialize(Vector2 pos)
{

	sprite = new DrawSprite(Novice::LoadTexture("./Resources/Block/block1.png"), { 64,64 });
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
	openTexture = Novice::LoadTexture("./Resources/Block/Gate2.png");
	closeTexture = Novice::LoadTexture("./Resources/Block/Gate1.png");
	sprite = new DrawSprite(closeTexture, { 64,64 });
	gateIndex = 0;

	auto index = mapChipField_->GetMapChipIndexByPosition(wtf_.translation_);
	auto type = mapChipField_->GetMapChipTypeIndex(index.xIndex, index.yIndex);
	while (type != MapChipType::kBlock) {
		mapChipField_->setMapChipData(MapChipType::kBlock, index.xIndex, index.yIndex + gateIndex);
		gateIndex++;
		type = mapChipField_->GetMapChipTypeIndex(index.xIndex, index.yIndex + gateIndex);

	}
	sprite->SetSize({ sprite->GetSize().x,gateIndex * 64.f });

	// audio
	gateOpenSFX = Novice::LoadAudio("./Resources/Audio/sfx/gateOpen.mp3");
	gateCloseSFX = Novice::LoadAudio("./Resources/Audio/sfx/gateClose.mp3");
}

void Gate::Update() {
	drawWtf.translation_.y = wtf_.translation_.y - (gateIndex - 1) * 32;
	/*if (Keys::IsTrigger(DIK_L))
	{
		Close();
	}
	if (Keys::IsTrigger(DIK_O))
	{
		Open();
	}*/


	wtf_.Update();
	drawWtf.Update();
}

void Gate::Draw(Camera* camera) {
	if (sprite) {
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
	sprite->SetTexture(openTexture);
	sprite->SetColor(0xffffff99);
	while (type != MapChipType::kBlank) {
		mapChipField_->setMapChipData(MapChipType::kBlank, index.xIndex, index.yIndex + gateIndexOpen);
		gateIndexOpen++;
		type = mapChipField_->GetMapChipTypeIndex(index.xIndex, index.yIndex + gateIndexOpen);
		if (gateIndexOpen == gateIndex)
		{
			break;
		}
	}
	Novice::PlayAudio(gateOpenSFX, 0, 1.0f);
	isOpen_ = true;
}

void Gate::Close() {
	auto index = mapChipField_->GetMapChipIndexByPosition(wtf_.translation_);
	auto type = mapChipField_->GetMapChipTypeIndex(index.xIndex, index.yIndex);
	if (type == MapChipType::kBlock)
	{
		return;
	}
	sprite->SetTexture(closeTexture);
	sprite->SetColor(0xffffffff);
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
	Novice::PlayAudio(gateCloseSFX, 0, 1.0f);
	isOpen_ = false;
}

void BlockButton::Initialize(Vector2 pos) {
	wtf_.translation_ = pos;
	ButtonTexture = Novice::LoadTexture("./Resources/Block/Button1.png");
	pushButtonTexture = Novice::LoadTexture("./Resources/Block/Button2.png");
	sprite = new DrawSprite(ButtonTexture, { 64,64 });
}

void BlockButton::Update() {
	bool pressedNow = false;
	if (player_ && player_->isPushButton(this))
	{
		pressedNow = true;
	}
	if (!pressedNow && enemies_ && enemies_->isPushButton(this)) pressedNow = true;

	if (pressedNow && !isPressed_) {
		isPressed_ = true;
		sprite->SetTexture(pushButtonTexture);
		if (gate_) gate_->Open();
	}
	else if (!pressedNow && isPressed_) {
		isPressed_ = false;
		sprite->SetTexture(ButtonTexture);
		if (gate_) gate_->Close();
	}
	wtf_.Update();
}

void BlockButton::Draw(Camera* camera) {
	if (sprite) {
		sprite->Draw(wtf_, camera, 0, 0, 64, 64);
	}
}
void HiddenFloor::Initialize(Vector2 pos)
{
	showTexture = Novice::LoadTexture("./Resources/Block/HiddenFloor1.png");
	texture = Novice::LoadTexture("./Resources/Block/HiddenFloor2.png");
	sprite = new DrawSprite(texture, { 64,64 });
	wtf_.Initialize();
	wtf_.translation_ = pos;
	auto index = mapChipField_->GetMapChipIndexByPosition(wtf_.translation_);
	mapChipField_->setMapChipData(MapChipType::kBlock, index.xIndex, index.yIndex);
}

void HiddenFloor::Update()
{
	isShow = false;

	if (enemies_) {
		for (auto* enemy : enemies_->GetEnemies()) {
			if (auto* lamp = dynamic_cast<EnemyLamp*>(enemy)) {
				float radius = lamp->getRadius();

				Vector2 floorPos = wtf_.translation_;
				Vector2 lampPos = lamp->GetPos();

				float dx = floorPos.x - lampPos.x;
				float dy = floorPos.y - lampPos.y;
				float distSq = sqrtf(dx * dx + dy * dy);

				if (distSq <= radius) {
					isShow = true;
					break;
				}
			}
		}
	}


	auto index = mapChipField_->GetMapChipIndexByPosition(wtf_.translation_);
	if (isShow) {

		mapChipField_->setMapChipData(MapChipType::kBlock, index.xIndex, index.yIndex);
		sprite->SetTexture(showTexture);
		sprite->SetColor(0xffffffff);

	}
	else {

		mapChipField_->setMapChipData(MapChipType::kBlank, index.xIndex, index.yIndex);
		sprite->SetTexture(texture);
		sprite->SetColor(0xffffff11);

	}
	wtf_.Update();
}


void HiddenFloor::Draw(Camera* camera)
{
	sprite->Draw(wtf_, camera, 0, 0, 64, 64);
}

void Thorn::Initialize(Vector2 pos)
{
	sprite = new DrawSprite(Novice::LoadTexture("./Resources/Block/Thorn.png"), { 64,64 });
	wtf_.Initialize();
	wtf_.translation_ = pos;


}

void Thorn::Update()
{
	wtf_.Update();
}

void Thorn::Draw(Camera* camera)
{
	sprite->Draw(wtf_, camera, 0, 0, 64, 64);
}

void Clear::Initialize(Vector2 pos)
{
	sprite = new DrawSprite(Novice::LoadTexture("./Resources/Block/clear.png"), { 96,128 });
	wtf_.Initialize();
	wtf_.translation_ = pos;

	wtf_.translation_.y += 16.f;
}

void Clear::Update()
{
	wtf_.Update();
}

void Clear::Draw(Camera* camera)
{
	sprite->Draw(wtf_, camera, 0, 0, 98, 128);
}

void BackGroundBlock::Initialize(Vector2 pos)
{
	sprite = new DrawSprite(Novice::LoadTexture("./Resources/Block/Block2.png"), { 64,64 });
	wtf_.Initialize();
	wtf_.translation_ = pos;
}

void BackGroundBone::Initialize(Vector2 pos)
{
	sprite = new DrawSprite(Novice::LoadTexture("./Resources/Block/bone.png"), { 192,192 });
	wtf_.Initialize();
	wtf_.translation_ = pos;
}

void BackGroundBone::Draw(Camera* camera)
{
	sprite->Draw(wtf_, camera, 0, 0, 192, 192);
}

void BackGroundCandlestick::Initialize(Vector2 pos)
{
	texture[0] = Novice::LoadTexture("./Resources/Block/candlestick1.png");
	texture[1] = Novice::LoadTexture("./Resources/Block/candlestick2.png");
	sprite = new DrawSprite(texture[0], { 64,64 });
	wtf_.Initialize();
	wtf_.translation_ = pos;

}

void BackGroundCandlestick::Draw(Camera* camera)
{
	time++;
	if (time >= 60)
	{
		time = 0;
		count = (count + 1) % 2;
		sprite->SetTexture(texture[count]);
	}
	sprite->Draw(wtf_, camera, 0, 0, 64, 64);
}

void BackGroundPumpkin::Initialize(Vector2 pos)
{
	sprite = new DrawSprite(Novice::LoadTexture("./Resources/Block/pumpkin1.png"), { 64,64 });
	wtf_.Initialize();
	wtf_.translation_ = pos;
}

void BackJump::Initialize(Vector2 pos)
{
	sprite = new DrawSprite(Novice::LoadTexture("./Resources/Block/tutorialJump.png"), { 72,84 });
	wtf_.Initialize();
	wtf_.translation_ = pos;
}

void BackJump::Draw(Camera* camera)
{
	sprite->Draw(wtf_, camera, 0, 0, 72, 84);
}

void BackMove::Initialize(Vector2 pos)
{
	sprite = new DrawSprite(Novice::LoadTexture("./Resources/Block/tutorialWalk.png"), { 80,72 });
	wtf_.Initialize();
	wtf_.translation_ = pos;
}

void BackMove::Draw(Camera* camera)
{
	sprite->Draw(wtf_, camera, 0, 0, 80, 72);
}

void BackAttack::Initialize(Vector2 pos)
{
	sprite = new DrawSprite(Novice::LoadTexture("./Resources/Block/tutorialShoot.png"), { 128,64 });
	wtf_.Initialize();
	wtf_.translation_ = pos;
}

void BackAttack::Draw(Camera* camera)
{
	sprite->Draw(wtf_, camera, 0, 0, 128, 64);
}

void BackAstral::Initialize(Vector2 pos)
{
	sprite = new DrawSprite(Novice::LoadTexture("./Resources/Block/tutorialSwitch.png"), { 128,90 });
	wtf_.Initialize();
	wtf_.translation_ = pos;
}

void BackAstral::Draw(Camera* camera)
{
	sprite->Draw(wtf_, camera, 0, 0, 128, 90);
}
