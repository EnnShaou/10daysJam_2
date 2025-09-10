#include "UI.h"
#define _USE_MATH_DEFINES
#include <math.h>

void UI::Initialize()
{
	frameSprite = Novice::LoadTexture("./Resources/UI/frame.png"); 
	iconSprite = Novice::LoadTexture("./Resources/UI/hpIcon.png"); 
	gaugeSprite = Novice::LoadTexture("./Resources/UI/gauge.png"); 
	gaugeSprite2 = Novice::LoadTexture("./Resources/UI/gauge2.png");
}

void UI::Draw(int hp,float timer, float coolTimer)
{
	// --- フレームの描画 ---
	Novice::DrawQuad(0, 0, 286, 0, 0, 176, 286, 176, 0, 0, 286, 176, frameSprite, WHITE);

	// --- プレイヤーのHPアイコン描画 ---
	posX = 100;
	posY = 20;
	width = 34 * hp;
	height = 48;
	
	Novice::DrawQuad(
		posX, posY, posX + width, posY,
		posX, posY + height, posX + width, posY + height,
		0, 0, width, height,
		iconSprite,WHITE);

	// --- 幽体状態切り替えのクールタイム描画 --- 
	posX = 63;
	posY = 82;
	width = 160 - static_cast<int>(54 * coolTimer);
	height = 18;

	color = WHITE;

	// クールタイム終了時に発光アニメーション追加
	if (coolTimer <= 0.0f)
	{
		// アニメーション用のタイマー
		static float glowTimer = 0.0f;
		glowTimer += 0.04f;

		// sin波を使ってON/OFF判定
		float wave = sinf(glowTimer * 6.0f); // 倍率を上げて点滅速度調整

		// 0か255にする（表示 or 非表示）
		int alphaInt = (wave > 0.0f) ? 255 : 0;

		color = (alphaInt << 24) | 0xFFFFFF;
	}

	Novice::DrawQuad(
		posX, posY, posX + width, posY,
		posX, posY + height, posX + width, posY + height,
		0, 0, width, height,
		gaugeSprite2, color);
	
	// --- 幽体の制限時間ゲージを描画 ---
	posX = 64;
	posY = 121;
	width = 160 - static_cast<int>(16 * timer);
	height = 18;

	Novice::DrawQuad(
		posX, posY, posX + width, posY,
		posX, posY + height, posX + width, posY + height,
		0, 0, width, height,
		gaugeSprite, WHITE);
}
