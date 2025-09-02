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
	tentativeWorldTransform_.Initialize();
	playerSprite_ = new DrawSprite(Novice::LoadTexture("./Resources/Player/player.png"), { 58,72 });
	playerSprite_->SetColor(0xffffffff);
	astralBodySprite_ = new DrawSprite(Novice::LoadTexture("white1x1.png"), { 68,72 });
	astralBodySprite_->SetColor(BLUE);
	dathSprite_ = new DrawSprite(Novice::LoadTexture("white1x1.png"), { 68,72 });
	dathSprite_->SetColor(BLACK);
}

void Player::Update()
{
	if (behaviorNext_ != Behavior::kUnknown) 
	{
		behavior_ = behaviorNext_;
		behaviorNext_ = Behavior::kUnknown;

		// 初期化処理
		switch (behavior_)
		{
		case Behavior::kRoot:
			BehaviorRootInitialize();
			break;
		case Behavior::kAttack:
			BehaviorAttackInitialize();
			break;
		case Behavior::kAstral:
			BehaviorAstralInitialize();
			break;
		default:
			break;
		}
	}

	// 行動ごとの更新処理
	switch (behavior_)
	{
	case Behavior::kRoot:
		BehaviorRootUpdate();
		break;
	case Behavior::kAttack:
		BehaviorAttackUpdate();
		break;
	case Behavior::kAstral:
		BehaviorAstralUpdate();
		break;
	default:
		break;
	}

	if (behavior_ != Behavior::kAstral)
	{
		CollisonMapInfo info;
		info.vel = vel_;
		MapCollision(info);
		MapCollisionMove(info);
		MapAfterCollision(info);
		MapWallCollision(info);
		GroundStates(info);
	}


	Animation();
	worldTransform_.Update();
}

void Player::Draw() {
	if (!isDead_ && behavior_ != Behavior::kAstral) 
	{
		playerSprite_->Draw(worldTransform_, camera_, (animationCount * 58) + (lrDir_ == LRDir::kRight ? 0 : 58), static_cast<int>(animationBehavior_) * 72 + 2, (lrDir_ == LRDir::kRight ? 1 : -1) * 58, 72); // プレイヤーを描画
	}

	if (behavior_ == Behavior::kAstral)
	{
		dathSprite_->Draw(tentativeWorldTransform_, camera_, 0, 0, 68, 72); // プレイヤーを描画
		astralBodySprite_->Draw(worldTransform_, camera_, 0, 0, 68, 72);
	}

	Novice::ScreenPrintf(30, 50, "behavior_: %d", behavior_);
	Novice::ScreenPrintf(30, 70, "behaviorNext_: %d", behaviorNext_);
	Novice::ScreenPrintf(30, 90, "coolTimer: %.2f", coolTime_);
	Novice::ScreenPrintf(30, 110, "Astralbehavior_: %d", Astralbehavior_);
	Novice::ScreenPrintf(30, 130, "AstralbehaviorNext_: %d", AstralbehaviorNext_);
	Novice::ScreenPrintf(30, 150, "astralBodyTimer: %f", astralBodyTimer_);
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

void Player::AstralMove()
{
	if (Keys::IsPress(DIK_D) || Keys::IsPress(DIK_A) || Keys::IsPress(DIK_W) || Keys::IsPress(DIK_S))
	{
		Vector2 acc = Vector2();

		if (Keys::IsPress(DIK_D))
		{
			if (astralVel_.x < 0.0f)
			{
				astralVel_.x = kPlayerSpeed;
			}
			acc.x += kPlayerSpeed;
		}
		else if (Keys::IsPress(DIK_A))
		{
			if (astralVel_.x > 0.0f) {
				astralVel_.x = kPlayerSpeed;
			}

			acc.x -= kPlayerSpeed;
		}
		else if (Keys::IsPress(DIK_W))
		{
			if (astralVel_.y < 0.0f)
			{
				astralVel_.y = kPlayerSpeed;
			}
			acc.y += kPlayerSpeed;
		}
		else if (Keys::IsPress(DIK_S))
		{
			if (astralVel_.y > 0.0f)
			{
				astralVel_.y = kPlayerSpeed;
			}
			acc.y -= kPlayerSpeed;
		}
		astralVel_ += acc;
		astralVel_.x = std::clamp(astralVel_.x, -kPlayerSpeedMax, kPlayerSpeedMax);
		astralVel_.y = std::clamp(astralVel_.y, -kPlayerSpeedMax, kPlayerSpeedMax);
		worldTransform_.translation_ += astralVel_;
	}
	else
	{
		astralVel_ = Vector2(0, 0);
	}
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
void Player::MapCollisionMove(const CollisonMapInfo& info) 
{
	worldTransform_.translation_ += info.vel; 
}
void Player::MapAfterCollision(const CollisonMapInfo& info) {

	if (info.Top) {
		vel_.y = 0;
	}
}
void Player::GroundStates(const CollisonMapInfo& info) {

	if (onGround) {
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
				auto index = mapChipField_->GetMapChipIndexByPosition(posNew[corner] + Vector2(0.f, -(kBlank + 1.f)));
				auto type = mapChipField_->GetMapChipTypeIndex(index.xIndex, index.yIndex);
				if (type == MapChipType::kBlock || type == MapChipType::kBlock2) {
					isHit = true;
					break;
				}
			}
			if (!isHit) {
				onGround = false;
			}
		}

	}
	else {
		float Gravity = kGravity;

		vel_ += Vector2(0, -Gravity);
		vel_.y = max(vel_.y, -kLimitFallSpeed);
		if (info.Bottom) {

			onGround = true;
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
void Player::BehaviorRootInitialize()
{
	{
		behavior_ = Behavior::kRoot;
		astralVel_ = Vector2(0, 0);
		worldTransform_.scale_ = Vector2(1.0f, 1.0f);

	}
}
void Player::BehaviorRootUpdate()
{
	if (coolTime_ > 0.0f) {
		coolTime_ -= frameTime;
		coolTime_ = max(0.0f, coolTime_);
	}

	if (Keys::IsTrigger(DIK_F) && onGround)
	{
		SwitchBody();
	}

	Move();
	if (Keys::IsTrigger(DIK_SPACE) && !onGround) {
		behaviorNext_ = Behavior::kAttack; // 攻撃行動に切り替え
	}
}

void Player::BehaviorAttackInitialize()
{

}

void Player::BehaviorAttackUpdate() {


	behaviorNext_ = Behavior::kRoot; // 攻撃行動が終了したらルートに戻る

}

void Player::BehaviorAstralInitialize()
{
	coolTime_ = 5.f;
	astralBodyTimer_ = 0.0f;
	behavior_ = Behavior::kAstral;

}

void Player::BehaviorAstralUpdate()
{
	if (AstralbehaviorNext_ != AstralBehavior::kUnknown) 
	{
		Astralbehavior_ = AstralbehaviorNext_;
		AstralbehaviorNext_ = AstralBehavior::kUnknown;

		// 初期化処理
		switch (Astralbehavior_) 
		{
		case AstralBehavior::kRoot:
			AstralBodyBehaviorRootInitialize();
			break;
		case AstralBehavior::kAttack:
			AstralBodyBehaviorAttackInitialize();
			break;
		default:
			break;
		}
	}

	// 行動ごとの更新処理
	switch (Astralbehavior_) 
	{
	case AstralBehavior::kRoot:
		AstralBodyBehaviorRootUpdate();
		break;
	case AstralBehavior::kAttack:
		AstralBodyBehaviorAttackUpdate();
		break;
	default:
		break;
	}
}

void Player::SwitchBody()
{
	if (coolTime_ > 0.0f)
	{
		return;
	}

	// 霊体化する前の位置を保存
	tentativeWorldTransform_ = worldTransform_;

	behaviorNext_ = Behavior::kAstral;
}

void Player::AstralBodyBehaviorRootInitialize()
{
	Astralbehavior_ = AstralBehavior::kRoot;
}

void Player::AstralBodyBehaviorRootUpdate()
{
	AstralMove();

	astralBodyTimer_ += frameTime;

	if (astralBodyTimer_ >= astralBodyDuration_)
	{
		behaviorNext_ = Behavior::kRoot;
		worldTransform_ = tentativeWorldTransform_;
		
	}
}

void Player::AstralBodyBehaviorAttackInitialize()
{

	Astralbehavior_ = AstralBehavior::kAttack;
	attackTimer = 0.0f;

}

void Player::AstralBodyBehaviorAttackUpdate()
{
}
void Player::Animation() {

	if (animationBehaviorNext_ != AnimationBehavior::kUnknown) {
		animationBehavior_ = animationBehaviorNext_;
		animationBehaviorNext_ = AnimationBehavior::kUnknown;
		animationCount = 0;
		animationTimer = 0;
		switch (animationBehavior_)
		{
		case Player::AnimationBehavior::kRoot:
			animationBehavior_ = Player::AnimationBehavior::kRoot;
			animationMax = 4; // アニメーションの最大数を設定
			break;
		case Player::AnimationBehavior::kMove:
			animationBehavior_ = Player::AnimationBehavior::kMove;
			animationMax = 4;
			break;
		case Player::AnimationBehavior::kJumpUp:
			animationBehavior_ = Player::AnimationBehavior::kJumpUp;
			animationMax = 2;
			break;
		case Player::AnimationBehavior::kJumpDown:
			animationBehavior_ = Player::AnimationBehavior::kJumpDown;
			animationMax = 2;
			break;

		}

	}
	switch (animationBehavior_)
	{
	case Player::AnimationBehavior::kRoot:

		animationTimer++;
		if (!onGround)
		{
			if (vel_.y > 0.f)
			{
				animationBehaviorNext_ = Player::AnimationBehavior::kJumpUp;
			}
			else
			{
				animationBehaviorNext_ = Player::AnimationBehavior::kJumpDown;
			}
		}
		if (Keys::IsPress(DIK_A) || Keys::IsPress(DIK_D))
		{
			animationBehaviorNext_ = Player::AnimationBehavior::kMove;
		}
		break;
	case Player::AnimationBehavior::kMove:

		animationTimer++;
		if (!onGround)
		{
			if (vel_.y > 0.f)
			{
				animationBehaviorNext_ = Player::AnimationBehavior::kJumpUp;
			}
			else
			{
				animationBehaviorNext_ = Player::AnimationBehavior::kJumpDown;
			}
		}
		if (!Keys::IsPress(DIK_A) && !Keys::IsPress(DIK_D))
		{
			animationBehaviorNext_ = Player::AnimationBehavior::kRoot;
		}
		break;
	case Player::AnimationBehavior::kJumpUp:

		animationTimer++;
		if (!onGround && vel_.y < 0.f)
		{
			animationBehaviorNext_ = Player::AnimationBehavior::kJumpDown;
		}
		break;
	case Player::AnimationBehavior::kJumpDown:

		animationTimer++;
		if (onGround)
		{
			animationBehaviorNext_ = Player::AnimationBehavior::kRoot;
		}
		break;
	default:
		break;
	}

	if (animationTimer >= 60 / (animationMax * 2)) {
		animationTimer = 0;// 60フレームに1回
		animationCount = (animationCount + 1) % animationMax;
	}
}
