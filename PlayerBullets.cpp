#include "PlayerBullets.h"
#include "Player.h"
void PlayerBullets::Initialize(Camera* camera, Vector2 pos, Vector2 dir)
{
	assert(camera);
	camera_ = camera;
	wtf.Initialize();
	wtf.translation_ = pos;
	sprite = new DrawSprite(Novice::LoadTexture("./Resources/PlayerBullet/bullet.png"), { 32,32 });
	sprite->SetColor(0xffffffff);
	isActive_ = true;
	isDead_ = false;
	dir_ = dir;
}

PlayerBullets::~PlayerBullets()
{
	delete sprite;
	sprite = nullptr;
}

void PlayerBullets::Update()
{
	// 寿命を減少
	lifeTime_ -= frameTime;


	if (isActive_)
	{
		// 向きによる移動
		wtf.translation_.x += vel_.x * dir_.x;

		// 寿命が尽きたら非アクティブにする
		if (lifeTime_ <= 0.0f)
		{
			lifeTime_ = 5.0f;
			isActive_ = false;
			isDead_ = true;
		}
	}

	// ワールドトランスフォーム更新
	wtf.Update();
}

void PlayerBullets::Draw()
{
	if (isActive_)
	{
		sprite->Draw(wtf, camera_, 0, 0, 30, 60);
	}
}
