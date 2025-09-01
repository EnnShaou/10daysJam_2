#include"DrawFunc.h"

DrawSprite::DrawSprite(int texture, float textureSize) :texture_(texture), color_(0xffffffff), textureSize_(textureSize)
{
}

DrawSprite::~DrawSprite()
{
}

void DrawSprite::Draw(WtF& wt, Camera* camera, int srcX, int srcY, int srcW, int srcH)
{

	// --- 1. ローカル座標 → ワールド座標
	Vector2 worldPos[4];
	Vector2 localVertices[4] = {
	{ -textureSize_ / 2.0f,textureSize_ / 2.0f},
	{  textureSize_ / 2.0f, textureSize_ / 2.0f},
	{ -textureSize_ / 2.0f,-textureSize_ / 2.0f},
	{  textureSize_ / 2.0f,  -textureSize_ / 2.0f}
	};
	for (int i = 0; i < 4; i++) {
		worldPos[i] = Transform(localVertices[i], wt.matWorld_);
	}

	// --- 2. ワールド座標 → スクリーン座標
	Matrix3x3 vpMatrix = camera->matView * camera->matProjection * camera->viewPort;
	Vector2 screenPos[4];
	for (int i = 0; i < 4; i++) {
		screenPos[i] = Transform(worldPos[i], vpMatrix);
	}

	// --- 3. Novice::DrawQuadで描画
	Novice::DrawQuad(
		static_cast<int>(screenPos[0].x), static_cast<int>(screenPos[0].y),
		static_cast<int>(screenPos[1].x), static_cast<int>(screenPos[1].y),
		static_cast<int>(screenPos[2].x), static_cast<int>(screenPos[2].y),
		static_cast<int>(screenPos[3].x), static_cast<int>(screenPos[3].y),
		srcX, srcY,
		srcW, srcH,
		texture_, color_
	);
}
