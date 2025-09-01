#include "Camera.h"

Camera::Camera(Vector2 pos)
	: m_pos(pos), m_scale({ 1.0f, 1.0f }), m_theta(0.0f),
	kWindowWidth_(1280), kWindowHeight_(720)
{
	matView = Identity();
	matProjection = Identity();
	viewPort = Identity();
}

Camera::~Camera() {}

void Camera::Initialize(UINT kWindowWidth, UINT kWindowHeight)
{
	kWindowWidth_ = kWindowWidth;
	kWindowHeight_ = kWindowHeight;

	// ビューポート変換行列 (左上原点 → スクリーン座標)
	// スクリーン座標系: [-1,1] → [0,width], [0,height]
	viewPort = MakeViewportMatrix(0, 0, static_cast<float>(kWindowWidth_), static_cast<float>(kWindowHeight_));
}
void Camera::Update()
{

	matView = MakeAffineMatrix({ 1.0f / m_scale.x, 1.0f / m_scale.y }, m_theta, m_pos);
	const float W = static_cast<float>(kWindowWidth_);
	const float H = static_cast<float>(kWindowHeight_);
	const float left = -(W / 2.0f);
	const float top = H / 2.0f;
	const float right = W / 2.0f;
	const float bottom = -(H / 2.0f);
	// Projection 行列 (正射影)
	matProjection = MakeOrthographicMatrix(left, top, right, bottom);


	// 最終的には → Projection * View * Viewport
	// （ここで合成するか、描画時に合成するかは設計次第）
}