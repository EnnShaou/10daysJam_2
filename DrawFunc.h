#pragma once
#include"Wtf.h"
#include"Camera.h"
class DrawSprite
{
public:
	DrawSprite(int texture, Vector2 textureSize);
	~DrawSprite();
	void SetColor(unsigned int color) { color_ = color; }
	Vector2 GetSize()const { return textureSize_; }
	void SetSize(Vector2 size) { textureSize_ = size; }
	void Draw(WtF& wt,               // 物体変換
		Camera* camera,        // カメラ
		int srcX, int srcY, int srcW, int srcH);

private:
	int texture_ = 0;
	Vector2 textureSize_;
	unsigned int color_;
};

