#include "PlayerBullets.h"
#include "Player.h"
void PlayerBullets::Initialize(Camera* camera, Vector2 pos, Vector2 dir)
{
	assert(camera);
	camera_ = camera;
	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	sprite = new DrawSprite(Novice::LoadTexture("./Resources/PlayerBullet/bullet.png"), { 32,32 });
	sprite->SetColor(0xffffffff);
	isActive_ = true;
	isDead_ = false;
	dir_ = dir;
	vel_ = { 0.0f,0.0f };
	lifeTime_ = 5.0f;
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
		worldTransform_.translation_.x += vel_.x * dir_.x;

		// 寿命が尽きたら非アクティブにする
		if (lifeTime_ <= 0.0f)
		{
			lifeTime_ = 5.0f;
			isActive_ = false;
			isDead_ = true;
		}
	}

	// ワールドトランスフォーム更新
	worldTransform_.Update();
}

void PlayerBullets::Draw()
{
	if (isActive_)
	{
		sprite->Draw(worldTransform_, camera_, 0, 0, 32, 32);
	}
}

void PlayerBullets::Launch()
{
	if (!isActive_ || isDead_)
	{
		return;
	}

	vel_ = { 10.0f, 0.0f };
}
