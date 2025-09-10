#pragma once
#include"WtF.h"
#include"DrawFunc.h"
#include"vector"
#include"MapChipField.h"

class Player;
class EnemyManager;
class Block
{
public:
	Block() {}
	~Block() {}


	virtual void  Initialize(Vector2 pos);
	// 更新処理
	virtual void Update();

	// 描画処理
	virtual void Draw(Camera* camera);
	void setPlayer(Player* player) {
		player_ = player;
	}
	void setEnemies(EnemyManager* enemy) {

		enemies_ = enemy;
	}
	void setMapChipField(MapChipField* mapChipField) {

		mapChipField_ = mapChipField;
	}
	void setBindID(int id) { bindID_ = id; }
	int getBindID() const { return bindID_; }
	Vector2 getPos() { return wtf_.translation_; }
protected:

	WtF wtf_;
	DrawSprite* sprite = nullptr;
	Player* player_ = nullptr;
	EnemyManager* enemies_ = nullptr;
	MapChipField* mapChipField_ = nullptr;
	int bindID_ = -1;
};

class Gate : public Block
{
public:
	Gate() {}
	~Gate() {}

	void Initialize(Vector2 pos) override;
	void Update() override;
	void Draw(Camera* camera) override;
	void Open();
	void Close();

	bool IsOpen() const { return isOpen_; }

private:
	bool isOpen_ = false;
	int gateIndex = 0;
	WtF drawWtf;

	// オーディオ
	int gateOpenSFX;
	int gateCloseSFX;

	int openTexture;
	int closeTexture;
};

class BlockButton : public Block
{
public:
	BlockButton() {}
	~BlockButton() {}

	void Initialize(Vector2 pos) override;
	void Update() override;
	void Draw(Camera* camera) override;
	void setGate(Gate* gate) {
		gate_ = gate;
	}

private:

	Gate* gate_ = nullptr;
	bool isPressed_ = false;
	int pushButtonTexture;
	int ButtonTexture;

};

class HiddenFloor : public Block
{
public:
	HiddenFloor() {}
	~HiddenFloor() {}
	void Initialize(Vector2 pos) override;
	void Update() override;
	void Draw(Camera* camera) override;
private:
	bool isShow = false;
	int showTexture;
	int texture;
};
class Thorn : public Block
{
public:
	Thorn() {}
	~Thorn() {}
	void Initialize(Vector2 pos) override;
	void Update() override;
	void Draw(Camera* camera) override;
private:
};
class Clear : public Block
{
public:
	Clear() {}
	~Clear() {}
	void Initialize(Vector2 pos) override;
	void Update() override;
	void Draw(Camera* camera) override;
private:
};
class BackGroundBlock : public Block
{
public:
	BackGroundBlock() {}
	~BackGroundBlock() {}
	void Initialize(Vector2 pos) override;
private:

};
class BackGroundBone : public Block
{
public:
	BackGroundBone() {}
	~BackGroundBone() {}
	void Initialize(Vector2 pos) override;
	void Draw(Camera* camera) override;
private:

};
class BackGroundCandlestick : public Block
{
public:
	BackGroundCandlestick() {}
	~BackGroundCandlestick() {}
	void Initialize(Vector2 pos) override;
	void Draw(Camera* camera) override;
private:
	int handle;
	int time = 0;
	int count = 0;
	int texture[2];
};
class BackGroundPumpkin : public Block
{
public:
	BackGroundPumpkin() {}
	~BackGroundPumpkin() {}
	void Initialize(Vector2 pos) override;
private:

};
class BackJump : public Block
{
public:
	BackJump() {}
	~BackJump() {}
	void Initialize(Vector2 pos) override;
	void Draw(Camera* camera) override;
private:

};
class BackMove : public Block
{
public:
	BackMove() {}
	~BackMove() {}
	void Initialize(Vector2 pos) override;
	void Draw(Camera* camera) override;
private:

};
class BackAttack : public Block
{
public:
	BackAttack() {}
	~BackAttack() {}
	void Initialize(Vector2 pos) override;
	void Draw(Camera* camera) override;
private:

};
class BackAstral : public Block
{
public:
	BackAstral() {}
	~BackAstral() {}
	void Initialize(Vector2 pos) override;
	void Draw(Camera* camera) override;
private:

};
class MapBlockManager
{
public:
	void pushBlock(Block* block, Vector2 pos, uint32_t x, uint32_t y);
	void Update();
	void Draw();
	void setSize(uint32_t x, uint32_t y) {

		block_.resize(y);
		for (uint32_t i = 0; i < y; i++) {
			block_[i].resize(x);
		}
	}
	void setCamera(Camera* camera) { camera_ = camera; }
	void setPlayer(Player* player) {
		player_ = player;
	}
	void setEnemies(EnemyManager* enemy) {

		enemies_ = enemy;
	}
	void setMapChipField(MapChipField* mapChipField) {

		mapChipField_ = mapChipField;
	}
	void BindButtonAndGates();
private:
	Camera* camera_ = nullptr;
	std::vector<std::vector<Block*>> block_;
	Player* player_ = nullptr;
	EnemyManager* enemies_ = nullptr;
	MapChipField* mapChipField_ = nullptr;
};