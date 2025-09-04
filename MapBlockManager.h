#pragma once
#include"WtF.h"
#include"DrawFunc.h"
#include"vector"
class MapBlockManager
{
public:
	void pushBlock(Vector2 pos);
	void Update();
	void Draw();

	void setCamera(Camera* camera) { camera_ = camera; }


private:
	Camera* camera_ = nullptr;
	std::vector<std::vector<Block>> block;
};
class Block
{
public:
	Block();
	~Block() {

	}
	virtual void  Initialize(Vector2 pos);
	// 更新処理
	virtual void Update();

	// 描画処理
	virtual void Draw(Camera* camera);
protected:

	WtF wtf_;
	DrawSprite* sprite = nullptr;
};

