#include "Player.h"
#include "ImGui.h"
#include "MapChipField.h"
#include <algorithm>
#include "keys.h"
#include <array>
Player::Player() : vel_(0, 0) {}

Player::~Player() {}

void Player::Initialize(Camera* camera, Vector2& pos) {

	assert(camera);
	camera_ = camera;
	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	playerSprite_ = new DrawSprite(Novice::LoadTexture("white1x1.png"), 32);
	playerSprite_->SetColor(RED);

}

void Player::Update() {

	if (behaviorNext_ != Behavior::kUnknown) {
		behavior_ = behaviorNext_; // 次の行動に切り替える
		switch (behavior_) {
		case Player::kRoot:
		default:
			BehaviorRootInitialize();
			break;

		case Player::kAttack:
			BehaviorAttackInitialize();
			break;

		}

		behaviorNext_ = Behavior::kUnknown; // 次の行動をリセット
	}

	switch (behavior_) {
	case Player::kRoot:
		BehaviorRootUpdate();
		break;
	case Player::kAttack:
		BehaviorAttackUpdate();
		break;

	}
	CollisonMapInfo info;
	info.vel = vel_;
	MapCollision(info);
	MapCollisionMove(info);
	MapAfterCollision(info);
	MapWallCollision(info);
	GroundStates(info);
	worldTransform_.Update();

}

void Player::Draw() {
	if (!isDead_) {
		playerSprite_->Draw(worldTransform_, camera_, 0, 0, 32, 32); // プレイヤーを描画
	}
}
void Player::Move() {
	if (isDead_) {
		return;
	}
	if (Keys::IsPress(DIK_D) || Keys::IsPress(DIK_A)) {
		Vector2 acc = Vector2();
		isMove = true;
		if (Keys::IsPress(DIK_D)) {
			if (vel_.x < 0.0f) {
				vel_.x = kPlayerSpeed;
			}
			if (lrDir_ != kRight) {
				lrDir_ = kRight;
			}
			acc.x += kPlayerSpeed;
		}
		else if (Keys::IsPress(DIK_A)) {
			if (vel_.x > 0.0f) {
				vel_.x = kPlayerSpeed;
			}
			if (lrDir_ != kLeft) {
				lrDir_ = kLeft;
			}

			acc.x -= kPlayerSpeed;
		}
		vel_ += acc;
		vel_.x = std::clamp(vel_.x, -kPlayerSpeedMax, kPlayerSpeedMax);
	}
	else {
		isMove = false;
		vel_.x = 0;
	}

	if (onGround) {
		if (Keys::IsTrigger(DIK_SPACE)) {
			vel_.y = kJumpAcceleration; // ジャンプの加速度を設定
		}
	}
}

void Player::Turn() {

}
void Player::MapCollision(CollisonMapInfo& info) {
	MapCollisionTop(info);
	MapCollisionBottom(info);
	MapCollisionLeft(info);
	MapCollisionRight(info);
}
void Player::MapCollisionTop(CollisonMapInfo& info) {

	if (info.vel.y <= 0) {
		return;
	}
	std::array<Vector2, 4> posNew;
	for (uint32_t i = 0; i < posNew.size(); ++i) {
		posNew[i] = CornerPos(worldTransform_.translation_ + info.vel, static_cast<Corner>(i));
	}
	std::array<Corner, 2> checkCorners = { kLeftTop, kRightTop };
	for (auto corner : checkCorners) {
		auto index = mapChipField_->GetMapChipIndexByPosition(posNew[corner]);
		auto type = mapChipField_->GetMapChipTypeIndex(index.xIndex, index.yIndex);
		auto typeNext = mapChipField_->GetMapChipTypeIndex(index.xIndex, index.yIndex + 1);
		if ((type == MapChipType::kBlock || type == MapChipType::kBlock2) && (typeNext != MapChipType::kBlock && typeNext != MapChipType::kBlock2)) {
			index = mapChipField_->GetMapChipIndexByPosition(worldTransform_.translation_ + info.vel + Vector2(0.f, kHeight / 2));
			auto indexSetNext = mapChipField_->GetMapChipIndexByPosition(worldTransform_.translation_ + Vector2(0.f, kHeight / 2));
			if (index.yIndex != indexSetNext.yIndex) {
				//  マップチップの矩形を取得
				auto rect = mapChipField_->GetRectByIndex(index.xIndex, index.yIndex);

				info.vel.y = max(0.0f, rect.bottom - worldTransform_.translation_.y - (kHeight / 2) - kBlank);
				info.Top = true;
				break;
			}
			if (type == MapChipType::Thorn) {
				isDead_ = true;
			}
			if (type == MapChipType::Clear) {
				isClear = true;
			}
		}
	}
}
void Player::MapCollisionBottom(CollisonMapInfo& info) {
	if (info.vel.y >= 0) {
		return;
	}
	std::array<Vector2, kNumCorner> posNew;
	for (uint32_t i = 0; i < posNew.size(); ++i) {
		posNew[i] = CornerPos(worldTransform_.translation_ + info.vel, static_cast<Corner>(i));
	}
	std::array<Corner, 2> checkCorners = { kLeftBottom, kRightBottom };
	for (auto corner : checkCorners) {
		auto index = mapChipField_->GetMapChipIndexByPosition(posNew[corner]);
		auto type = mapChipField_->GetMapChipTypeIndex(index.xIndex, index.yIndex);
		auto typeNext = mapChipField_->GetMapChipTypeIndex(index.xIndex, index.yIndex - 1);
		if ((type == MapChipType::kBlock || type == MapChipType::kBlock2) && (typeNext != MapChipType::kBlock && typeNext != MapChipType::kBlock2)) {
			index = mapChipField_->GetMapChipIndexByPosition(worldTransform_.translation_ + info.vel - Vector2(0.f, kHeight / 2));
			auto indexSetNext = mapChipField_->GetMapChipIndexByPosition(worldTransform_.translation_ - Vector2(0.f, kHeight / 2));
			if (index.yIndex != indexSetNext.yIndex) {
				auto rect = mapChipField_->GetRectByIndex(index.xIndex, index.yIndex);
				info.vel.y = min(0.0f, rect.top - worldTransform_.translation_.y + (kHeight / 2) + kBlank);
				info.Bottom = true;
				break;
			}
			if (type == MapChipType::Thorn) {
				isDead_ = true;
			}
			if (type == MapChipType::Clear) {
				isClear = true;
			}
		}
	}
}
void Player::MapCollisionLeft(CollisonMapInfo& info) {

	if (info.vel.x >= 0) {
		return;
	}
	std::array<Vector2, kNumCorner> posNew;
	for (uint32_t i = 0; i < posNew.size(); ++i) {
		posNew[i] = CornerPos(worldTransform_.translation_ + info.vel, static_cast<Corner>(i));
	}
	std::array<Corner, 2> checkCorners = { kLeftBottom, kLeftTop };
	for (auto corner : checkCorners) {
		auto index = mapChipField_->GetMapChipIndexByPosition(posNew[corner]);
		auto type = mapChipField_->GetMapChipTypeIndex(index.xIndex, index.yIndex);
		// auto typeNext = mapChipField_->GetMapChipTypeIndex(index.xIndex - 1, index.yIndex);
		if (type == MapChipType::kBlock || type == MapChipType::kBlock2 /*&& typeNext != MapChipType::kBlock*/) {
			index = mapChipField_->GetMapChipIndexByPosition(worldTransform_.translation_ + info.vel - Vector2(kWidth / 2, 0.f));
			auto indexSetNext = mapChipField_->GetMapChipIndexByPosition(worldTransform_.translation_ - Vector2(kWidth / 2, 0.f));
			if (index.xIndex != indexSetNext.xIndex) {
				//  マップチップの矩形を取得
				auto rect = mapChipField_->GetRectByIndex(index.xIndex, index.yIndex);
				info.vel.x = max(0.0f, rect.right - worldTransform_.translation_.x - (kWidth / 2) + kBlank);
				info.LR = true;
				break;
			}
		}
		if (type == MapChipType::Thorn) {
			isDead_ = true;
		}
		if (type == MapChipType::Clear) {
			isClear = true;
		}
	}
}
void Player::MapCollisionRight(CollisonMapInfo& info) {

	if (info.vel.x <= 0) {
		return;
	}
	std::array<Vector2, kNumCorner> posNew;
	for (uint32_t i = 0; i < posNew.size(); ++i) {
		posNew[i] = CornerPos(worldTransform_.translation_ + info.vel, static_cast<Corner>(i));
	}
	std::array<Corner, 2> checkCorners = { kRightBottom, kRightTop };
	for (auto corner : checkCorners) {
		auto index = mapChipField_->GetMapChipIndexByPosition(posNew[corner]);
		auto type = mapChipField_->GetMapChipTypeIndex(index.xIndex, index.yIndex);
		// auto typeNext = mapChipField_->GetMapChipTypeIndex(index.xIndex + 1, index.yIndex);
		if (type == MapChipType::kBlock || type == MapChipType::kBlock2 /*&& typeNext != MapChipType::kBlock*/) {
			index = mapChipField_->GetMapChipIndexByPosition(worldTransform_.translation_ + info.vel + Vector2(kWidth / 2, 0.f));
			auto indexSetNext = mapChipField_->GetMapChipIndexByPosition(worldTransform_.translation_ + Vector2(kWidth / 2, 0.f));
			if (index.xIndex != indexSetNext.xIndex) {
				//  マップチップの矩形を取得
				auto rect = mapChipField_->GetRectByIndex(index.xIndex, index.yIndex);
				info.vel.x = min(0.0f, rect.left - worldTransform_.translation_.x + (kWidth / 2) + kBlank);
				info.LR = true;
				break;
			}
		}
		if (type == MapChipType::Thorn) {
			isDead_ = true;
		}
		if (type == MapChipType::Clear) {
			isClear = true;
		}
	}
}
Vector2 Player::CornerPos(const Vector2 center, Corner corner) {

	Vector2 offsetTable[kNumCorner] = {
		Vector2(+kWidth / 2, -kHeight / 2) , // 右下
		Vector2(-kWidth / 2, -kHeight / 2) , // 左下
		Vector2(-kWidth / 2, +kHeight / 2) , // 左上
		Vector2(kWidth / 2, +kHeight / 2)    // 右上
	};
	return (center + offsetTable[static_cast<uint32_t>(corner)]);
}
void Player::MapCollisionMove(const CollisonMapInfo& info) { worldTransform_.translation_ += info.vel; }
void Player::MapAfterCollision(const CollisonMapInfo& info) {

	if (info.Top) {
		vel_.y = 0;
	}
}
void Player::GroundStates(const CollisonMapInfo& info) {

	if (onGround) {
		isJumping = false; // ジャンプ中フラグを下ろす
		if (vel_.y > 0.0f) {
			onGround = false;
		}
		else {
			bool isHit = false;
			std::array<Vector2, kNumCorner> posNew;
			for (uint32_t i = 0; i < posNew.size(); ++i) {
				posNew[i] = CornerPos(worldTransform_.translation_ + info.vel, static_cast<Corner>(i));
			}
			std::array<Corner, 2> checkCorners = { kLeftBottom, kRightBottom };
			for (auto corner : checkCorners) {
				auto index = mapChipField_->GetMapChipIndexByPosition(posNew[corner] + Vector2(0.f, -kBlank));
				auto type = mapChipField_->GetMapChipTypeIndex(index.xIndex, index.yIndex);
				if (type == MapChipType::kBlock || type == MapChipType::kBlock2) {
					isHit = true; // 地面にいない
					break;
				}
			}
			if (!isHit) {
				onGround = false;
			}
		}

	}
	else {
		isJumping = true; // ジャンプ中フラグを立てる
		float Gravity = kGravity;
		if (slide) {
			Gravity = slideSpeed;
		}
		vel_ += Vector2(0, -Gravity);
		vel_.y = max(vel_.y, -kLimitFallSpeed);
		if (info.Bottom) {

			onGround = true;
			vel_.x *= (1.0f - kPlayerSpeed);
			vel_.y = 0.0f;
		}
	}
}
void Player::MapWallCollision(CollisonMapInfo& info) {
	if (info.LR) {
		vel_.x = 0;
	}
}
//MathUtility::AABB Player::GetAABB() const {
//	AABB aabb;
//	Vector3 center = GetPos();
//	aabb.min = Vector3(center.x - kWidth / 2 + kBlank, center.y - kHeight / 2 + kBlank, center.z);
//	aabb.max = Vector3(center.x + kWidth / 2 - kBlank, center.y + kHeight / 2 - kBlank, center.z);
//	return aabb;
//}
void Player::OnCollision(const Enemies* enemies) {
	(void)enemies; // プレイヤーとの衝突処理はまだ実装されていない
	// float enemyPosX = enemies->GetPos().x;
	/*float attackPosX = 0.2f;
	float attackPosY = 0.1f;*/
	// if (enemyPosX - GetPos().x > 0) {
	//	attackPosX = -attackPosX; // 敵が左側にいる場合は攻撃位置を反転
	// }
	// Vector3 collisionPos = {attackPosX, attackPosY, 0.f};
	// vel_ += collisionPos;
	// if (vel_.x == 0.f && vel_.y == 0.f) {
	//
	// }
	isDead_ = true; // プレイヤーが死亡
}
void Player::BehaviorRootInitialize() {
	{
		behavior_ = Behavior::kRoot;
		worldTransform_.scale_ = Vector2(1.0f, 1.0f);
		slide = false;

	}
}
void Player::BehaviorRootUpdate() {

	Move();
	Turn();
	if (Keys::IsTrigger(DIK_SPACE) && !onGround) {
		behaviorNext_ = Behavior::kAttack; // 攻撃行動に切り替え
	}
	JumpAnimetion();
	MoveAnimation();
}

void Player::BehaviorAttackInitialize() {
	behavior_ = Behavior::kAttack;

}

void Player::BehaviorAttackUpdate() {


	behaviorNext_ = Behavior::kRoot; // 攻撃行動が終了したらルートに戻る

}


void Player::JumpAnimetion() {


}

void Player::MoveAnimation() {

}
