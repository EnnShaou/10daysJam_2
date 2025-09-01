#pragma once
#include"Vector2.h"
class WtF
{
public:
	WtF();
	~WtF();
	void Initialize();
	void Update();

	Vector2 translation_ = { 0,0 };
	Vector2 scale_ = { 1,1 };
	float rotation_ = 0.0f;
	// ローカル → ワールド変換行列
	Matrix3x3 matWorld_;
};

