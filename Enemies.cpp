#include "Enemies.h"
#include <array>
#include <algorithm>



Enemies::~Enemies()
{
}

EnemyPumpkin::EnemyPumpkin()
{

}

EnemyPumpkin::~EnemyPumpkin()
{
	delete sprite;
	sprite = nullptr;

	delete camera_;
	camera_ = nullptr;
}

void EnemyPumpkin::Initialize(Camera* camera, Vector2& pos, MapChipField* mapChipField)
{
	sprite = new DrawSprite(Novice::LoadTexture("white1x1.png"), { 64,64 });
	sprite->SetColor(0xff7d00ff);
	camera_ = camera;
	wtf.Initialize();
	wtf.translation_ = pos;
	mapChipField_ = mapChipField;

}

void EnemyPumpkin::AstralBehavior() {
	//　プレイヤーが敵の攻撃範囲にいるかどうかを計算
	Vector2 playerPos = player_->GetPos();
	Vector2 pumpkinPos = wtf.translation_;
	Vector2 enemyToPlayer = { playerPos.x - pumpkinPos.x, playerPos.y - pumpkinPos.y };
	float distanceToPlayer = sqrtf(enemyToPlayer.x * enemyToPlayer.x + enemyToPlayer.y * enemyToPlayer.y);

	// プレイヤーの方向に動く
	if (distanceToPlayer <= kAtkRange) {
		if (playerPos.x >= pumpkinPos.x) {
			vel_.x = +kSpeed.x;
		}
		else {
			vel_.x = -kSpeed.x;
		}

		if (playerPos.y >= pumpkinPos.y) {
			vel_.y = +kSpeed.y;
		}
		else {
			vel_.y = -kSpeed.y;
		}
	}
	else {
		//止まる
		vel_ = { 0.0f, 0.0f };
	}


	if (player_->GetBehavior() == Player::Behavior::kRoot) {
		behaviorNext_ = Behavior::kStop;
	}
}

void EnemyPumpkin::AstralBehaviorInitialize() {
	// プレイヤーが幽霊状態の時の処理
	behavior_ = Behavior::kAstral;

}

void EnemyPumpkin::StopBehavior() {
	if (player_->GetBehavior() == Player::Behavior::kAstral) {
		behaviorNext_ = Behavior::kAstral;
	}
}

void EnemyPumpkin::StopBehaviorInitialize() {
	// プレイヤーが本体状態の時の処理

	behavior_ = Behavior::kStop;
	vel_ = { 0.0f, 0.0f };
}

void EnemyPumpkin::InputGravity(const CollisonMapInfo& info)
{
	if (behavior_ == Behavior::kAstral)
	{
		return;
	}
	if (onGround)
	{
		return;
	}
	float Gravity = kGravity;
	vel_ += Vector2(0, -Gravity);
	vel_.y = max(vel_.y, -kLimitFallSpeed);
	if (info.Bottom) {

		onGround = true;
		vel_.y = 0.0f;
	}
}

void Enemies::InputGravity(const CollisonMapInfo& info)
{
	if (onGround)
	{
		return;
	}
	float Gravity = kGravity;
	vel_ += Vector2(0, -Gravity);
	vel_.y = max(vel_.y, -kLimitFallSpeed);
	if (info.Bottom) {

		onGround = true;
		vel_.y = 0.0f;
	}
}

void EnemyPumpkin::Update() {


	if (behaviorNext_ != Behavior::kUnknown) {
		behavior_ = behaviorNext_;
		behaviorNext_ = Behavior::kUnknown;

		// 初期化処理
		switch (behavior_) {
		case Behavior::kStop:
			StopBehaviorInitialize();
			break;
		case Behavior::kAstral:
			AstralBehaviorInitialize();
			break;
		default:
			break;
		}
	}

	switch (behavior_) {
	case Behavior::kStop:
		StopBehavior();
		break;
	case Behavior::kAstral:
		AstralBehavior();
		break;
	default:
		break;
	}



	// マップチップの当たり判定
	CollisonMapInfo info;
	info.vel = vel_;
	MapCollision(info);
	MapAfterCollision(info);
	MapWallCollision(info);
	GroundStates(info);
	InputGravity(info);


	// 移動更新
	wtf.translation_ += info.vel;

	wtf.Update();
}

void EnemyPumpkin::Draw()
{
	sprite->Draw(wtf, camera_, 0, 0, 64, 64);
}


Vector2 Enemies::CornerPos(const Vector2 center, Corner corner) {

	Vector2 offsetTable[kNumCorner] = {
		Vector2(+kWidth / 2, -kHeight / 2) , // 右下
		Vector2(-kWidth / 2, -kHeight / 2) , // 左下
		Vector2(-kWidth / 2, +kHeight / 2) , // 左上
		Vector2(kWidth / 2, +kHeight / 2)    // 右上
	};
	return (center + offsetTable[static_cast<uint32_t>(corner)]);
}

void Enemies::MapCollision(CollisonMapInfo& info)
{
	MapCollisionLeft(info);
	MapCollisionRight(info);
	MapCollisionTop(info);
	MapCollisionBottom(info);
	MapAfterCollision(info);
	MapWallCollision(info);
	GroundStates(info);
}

void Enemies::MapCollisionLeft(CollisonMapInfo& info) {
	if (info.vel.x >= 0) {
		return;
	}
	std::array<Vector2, kNumCorner> posNew;
	for (uint32_t i = 0; i < posNew.size(); ++i) {
		posNew[i] = CornerPos(wtf.translation_ + info.vel, static_cast<Corner>(i));
	}
	std::array<Corner, 2> checkCorners = { kLeftBottom, kLeftTop };
	for (auto corner : checkCorners) {
		auto index = mapChipField_->GetMapChipIndexByPosition(posNew[corner]);
		auto type = mapChipField_->GetMapChipTypeIndex(index.xIndex, index.yIndex);
		//auto typeNext = enemiesMapChipField_->GetMapChipTypeIndex(index.xIndex - 1, index.yIndex);
		if (type == MapChipType::kBlock) {
			index = mapChipField_->GetMapChipIndexByPosition(wtf.translation_ + info.vel - Vector2(kWidth / 2, 0.f));
			auto indexSetNext = mapChipField_->GetMapChipIndexByPosition(wtf.translation_ - Vector2(kWidth / 2, 0.f));
			if (index.xIndex != indexSetNext.xIndex) {
				//  マップチップの矩形を取得
				auto rect = mapChipField_->GetRectByIndex(index.xIndex, index.yIndex);
				info.vel.x = max(0.0f, rect.right - wtf.translation_.x - (kWidth / 2) + kBlank);
				info.LR = true;
				break;
			}
		}

	}
}

void Enemies::MapCollisionRight(CollisonMapInfo& info) {

	if (info.vel.x <= 0) {
		return;
	}
	std::array<Vector2, kNumCorner> posNew;
	for (uint32_t i = 0; i < posNew.size(); ++i) {
		posNew[i] = CornerPos(wtf.translation_ + info.vel, static_cast<Corner>(i));
	}
	std::array<Corner, 2> checkCorners = { kRightBottom, kRightTop };
	for (auto corner : checkCorners) {
		auto index = mapChipField_->GetMapChipIndexByPosition(posNew[corner]);
		auto type = mapChipField_->GetMapChipTypeIndex(index.xIndex, index.yIndex);
		// auto typeNext = mapChipField_->GetMapChipTypeIndex(index.xIndex + 1, index.yIndex);
		if (type == MapChipType::kBlock) {
			index = mapChipField_->GetMapChipIndexByPosition(wtf.translation_ + info.vel + Vector2(kWidth / 2, 0.f));
			auto indexSetNext = mapChipField_->GetMapChipIndexByPosition(wtf.translation_ + Vector2(kWidth / 2, 0.f));
			if (index.xIndex != indexSetNext.xIndex) {
				//  マップチップの矩形を取得
				auto rect = mapChipField_->GetRectByIndex(index.xIndex, index.yIndex);
				info.vel.x = min(0.0f, rect.left - wtf.translation_.x + (kWidth / 2) + kBlank);
				info.LR = true;
				break;
			}
		}

	}
}

void Enemies::MapWallCollision(CollisonMapInfo& info) {
	if (info.LR) {
		vel_.x = 0;
	}
}

void Enemies::MapCollisionTop(CollisonMapInfo& info) {

	if (info.vel.y <= 0) {
		return;
	}
	std::array<Vector2, 4> posNew;
	for (uint32_t i = 0; i < posNew.size(); ++i) {
		posNew[i] = CornerPos(wtf.translation_ + info.vel, static_cast<Corner>(i));
	}
	std::array<Corner, 2> checkCorners = { kLeftTop, kRightTop };
	for (auto corner : checkCorners) {
		auto index = mapChipField_->GetMapChipIndexByPosition(posNew[corner]);
		auto type = mapChipField_->GetMapChipTypeIndex(index.xIndex, index.yIndex);
		auto typeNext = mapChipField_->GetMapChipTypeIndex(index.xIndex, index.yIndex + 1);
		if (type == MapChipType::kBlock && typeNext != MapChipType::kBlock) {
			index = mapChipField_->GetMapChipIndexByPosition(wtf.translation_ + info.vel + Vector2(0.f, kHeight / 2));
			auto indexSetNext = mapChipField_->GetMapChipIndexByPosition(wtf.translation_ + Vector2(0.f, kHeight / 2));
			if (index.yIndex != indexSetNext.yIndex) {
				//  マップチップの矩形を取得
				auto rect = mapChipField_->GetRectByIndex(index.xIndex, index.yIndex);

				info.vel.y = max(0.0f, rect.bottom - wtf.translation_.y - (kHeight / 2) - kBlank);
				info.Top = true;
				break;
			}

		}
	}
}

void Enemies::MapCollisionBottom(CollisonMapInfo& info) {
	if (info.vel.y >= 0) {
		return;
	}
	std::array<Vector2, kNumCorner> posNew;
	for (uint32_t i = 0; i < posNew.size(); ++i) {
		posNew[i] = CornerPos(wtf.translation_ + info.vel, static_cast<Corner>(i));
	}
	std::array<Corner, 2> checkCorners = { kLeftBottom, kRightBottom };
	for (auto corner : checkCorners) {
		auto index = mapChipField_->GetMapChipIndexByPosition(posNew[corner]);
		auto type = mapChipField_->GetMapChipTypeIndex(index.xIndex, index.yIndex);
		auto typeNext = mapChipField_->GetMapChipTypeIndex(index.xIndex, index.yIndex - 1);
		if (type == MapChipType::kBlock && typeNext != MapChipType::kBlock) {
			index = mapChipField_->GetMapChipIndexByPosition(wtf.translation_ + info.vel - Vector2(0.f, kHeight / 2));
			auto indexSetNext = mapChipField_->GetMapChipIndexByPosition(wtf.translation_ - Vector2(0.f, kHeight / 2));
			if (index.yIndex != indexSetNext.yIndex) {
				auto rect = mapChipField_->GetRectByIndex(index.xIndex, index.yIndex);
				info.vel.y = min(0.0f, rect.top - wtf.translation_.y + (kHeight / 2) + kBlank);
				info.Bottom = true;
				break;
			}

		}
	}
}

void Enemies::MapAfterCollision(const CollisonMapInfo& info) {

	if (info.Top) {
		vel_.y = 0;
	}
}

void Enemies::GroundStates(const CollisonMapInfo& info) {


	if (onGround) {
		if (vel_.y > 0.0f) {
			onGround = false;
		}
		else {
			bool isHit = false;
			std::array<Vector2, kNumCorner> posNew;
			for (uint32_t i = 0; i < posNew.size(); ++i) {
				posNew[i] = CornerPos(wtf.translation_ + info.vel, static_cast<Corner>(i));
			}
			std::array<Corner, 2> checkCorners = { kLeftBottom, kRightBottom };
			for (auto corner : checkCorners) {
				auto index = mapChipField_->GetMapChipIndexByPosition(posNew[corner] + Vector2(0.f, -(kBlank + 1.f)));
				auto type = mapChipField_->GetMapChipTypeIndex(index.xIndex, index.yIndex);
				if (type == MapChipType::kBlock) {
					isHit = true;
					break;
				}
			}
			if (!isHit) {
				onGround = false;
			}
		}

	}

	InputGravity(info);

}
