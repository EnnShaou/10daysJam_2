#include "Camera.h"
#include"Player.h"
Camera::Camera(Vector2 pos)
	: m_pos(pos), m_scale({ 0.8f, 0.8f }), m_theta(0.0f),
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
	// ---- プロジェクション行列の更新 ----
	const float W = static_cast<float>(kWindowWidth_);
	const float H = static_cast<float>(kWindowHeight_);
	const float left = -(W / 2.0f);
	const float top = (H / 2.0f);
	const float right = (W / 2.0f);
	const float bottom = -(H / 2.0f);

	matProjection = MakeOrthographicMatrix(left, top, right, bottom);
	if (!target_) return;

	// ---- 1. X方向：進行方向の先読みオフセット（スムーズ版） ----
	Vector2 targetPos = target_->GetPos() * ((1.f - m_scale.x) + 1);

	float maxLookAheadX = 240.0f;   // 最大オフセット
	float threshold = 3.9f;         // 発動速度閾値
	float targetLookAheadX = 0.0f;  // このフレームの目標オフセット

	if (target_->GetVel().x > threshold) {
		targetLookAheadX = maxLookAheadX;
	}
	else if (target_->GetVel().x < -threshold) {
		targetLookAheadX = -maxLookAheadX;
	}

	// currentLookAheadX_ は Camera のメンバ変数（初期値 0）
	float lookAheadSmooth = 5.0f;          // 補間スピード
	float deltaTime = 1.0f / 60.0f;        // 実際のフレーム時間を渡すとより自然

	currentLookAheadX_ += (targetLookAheadX - currentLookAheadX_) * lookAheadSmooth * deltaTime;
	targetPos.x += currentLookAheadX_;

	// ---- 2. Y方向：重力を考慮した自然な追従 ----
	float playerY = target_->GetPos().y * ((1.f - m_scale.x) + 1);
	desiredY_ = playerY;

	if (target_->GetVel().y < -5.0f) {
		desiredY_ = playerY - 80.0f;   // 落下中：下にシフト
	}
	else if (target_->GetVel().y > 5.0f) {
		desiredY_ = playerY + 60.0f;   // 上昇中：上にシフト
	}

	// ---- 3. Y軸のスムーズ追従 ----
	float smoothSpeedY = 3.0f;
	m_pos.y += (desiredY_ - m_pos.y) * smoothSpeedY * deltaTime;

	// ---- 4. X軸のスムーズ追従 ----
	float smoothSpeedX = 5.0f;
	m_pos.x += (targetPos.x - m_pos.x) * smoothSpeedX * deltaTime;

	// ---- 5. ビュー行列の更新 ----
	matView = MakeAffineMatrix(
		{ 1.0f / m_scale.x, 1.0f / m_scale.y },
		m_theta,
		{ -m_pos.x, -m_pos.y }
	);
}






