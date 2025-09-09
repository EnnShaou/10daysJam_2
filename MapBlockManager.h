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
};

class BlockButtonAndGate : public Block
{
public:
	BlockButtonAndGate() {}
	~BlockButtonAndGate() {}

	void Initialize(Vector2 pos) override;
	void Update() override;
	void Draw(Camera* camera) override;
	void setGate(Gate* gate) {
		gate_ = gate;
	}

private:

	Gate* gate_ = nullptr;
	bool isPressed_ = false;

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