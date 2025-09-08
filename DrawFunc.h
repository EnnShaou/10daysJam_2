#pragma once
#include"Wtf.h"
#include"Camera.h"
class DrawSprite
{
public:
	enum LRDirection {
		kRight,
		kLeft,
	};
	LRDirection lrDirection_ = LRDirection::kRight;

	DrawSprite(int texture, Vector2 textureSize);
	~DrawSprite();
	void SetColor(unsigned int color) { color_ = color; }
	Vector2 GetSize()const { return textureSize_; }
	void SetSize(Vector2 size) { textureSize_ = size; }
	void Draw(WtF& wt,               // 物体変換
		Camera* camera,        // カメラ
		int srcX, int srcY, int srcW, int srcH, LRDirection lrDir = kRight);
	
private:
	int texture_ = 0;
	Vector2 textureSize_;
	unsigned int color_;
};

void DrawCircle(WtF& wt,	Camera* camera, int radius, unsigned int color);
class RGBA {
	public:
	int R, G, B, A;
	RGBA(int r = 255, int g = 255, int b = 255, int a = 255)
		: R(int(r)), G(int(g)), B(int(b)), A(int(a)) {
	}
	~RGBA(){}

	operator int() const {
		return (R << 24) | (G << 16) | (B << 8) | A;
	}
	RGBA operator+(const RGBA& other) const {
		return RGBA(
			min(R + other.R, 255),
			min(G + other.G, 255),
			min(B + other.B, 255),
			min(A + other.A, 255)
		);
	}
	RGBA operator-(const RGBA& other) const {
		return RGBA(
			max(R - other.R, 0),
			max(G - other.G, 0),
			max(B - other.B, 0),
			max(A - other.A, 0)
		);
	}
};