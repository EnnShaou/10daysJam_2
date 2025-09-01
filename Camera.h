#pragma once
#include "Vector2.h"
class Camera
{
public:
	Camera(Vector2 pos);

	~Camera();
	void Initialize(UINT kWindowWidth, UINT kWindowHeight);
	void Update();
	//Vector2 CameraTransform(Vector2 pos, Vector2 localSpace);

	//void Move(char* keys, char* preKyes, const float map_maxX, const float map_maxY, Player* player);
	Matrix3x3 matView;
	Matrix3x3 matProjection;
	Matrix3x3 viewPort;
	//CameraState m_state;
	Vector2 m_pos;
	Vector2 m_scale;
	float m_theta;
private:
	UINT kWindowWidth_; // ウィンドウの横幅
	UINT kWindowHeight_; // ウィンドウの縦幅
	
	// コピー禁止
	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;
};
