#pragma once
#include <Novice.h>

class Player;
class UI
{
public:
	// 初期化
	void Initialize();

	// 描画処理
	void Draw(int HP, float timer,float coolTimer);

private:
	// スプライト
	int frameSprite;  // フレーム
	int iconSprite;   // 本体のHPアイコン
	int gaugeSprite;  // 幽体状態のHPゲージ
	int gaugeSprite2; // 幽体状態になるまでのクールタイムゲージ

	int posX = 0;   // 描画位置X
	int posY = 0;   // 描画位置Y
	int width = 0;  // 描画幅X
	int height = 0; // 描画幅Y

	unsigned int color = WHITE; // スプライトの色
};

