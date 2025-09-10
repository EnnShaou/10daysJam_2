#include "Player.h"
#include "MapChipField.h"
#include <algorithm>
#include "keys.h"
#include <array>
#include "EnemyManager.h"

Player::Player() : vel_(0, 0) {}

Player::~Player() {}

void Player::Initialize(Camera* camera, Vector2& pos)
{
	assert(camera);
	camera_ = camera;
	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	tentativeWorldTransform_.Initialize();
	playerSprite_ = new DrawSprite(Novice::LoadTexture("./Resources/Player/player.png"), { 58,72 });
	astralBodySprite_ = new DrawSprite(Novice::LoadTexture("./Resources/PlayerGhost/playerGhost.png"), { 64,64 });
	astralBodyHP = maxAstralBodyHP;
	nomalBodyHP = maxNomalBodyHP;
	damageCooldown_ = 0;

	// オーディオファイル
	bodyDamagedSFX = Novice::LoadAudio("./Resources/Audio/sfx/playerDamaged.mp3");
	ghostDamagedSFX = Novice::LoadAudio("./Resources/Audio/sfx/ghostDamaged.mp3");
	shootSFX = Novice::LoadAudio("./Resources/Audio/sfx/shoot.mp3");
	switchBodySFX = Novice::LoadAudio("./Resources/Audio/sfx/switchBody.mp3");
	jumpSFX = Novice::LoadAudio("./Resources/Audio/sfx/jump.mp3");

	ui_ = new UI();
	ui_->Initialize();

	//deathCount = 0.01f;

	deathAnimEnded_ = false;     // 死亡アニメーションが切り替わったのでリセット
	deathWaitTimer_ = 0;         // 待機タイマーもリセット
}

void Player::Update()
{
	if (isDead_)
	{
		return;
	}

	if (damageCooldown_ > 0)
	{
		damageCooldown_--;
	}

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
		case Behavior::kKnockback:
			BehaviorKnockbackInitialize();
			break;
		case Behavior::kDead:
			BehaviorDeadInitialize();
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
	case Behavior::kKnockback:
		BehaviorKnockbackUpdate();
		break;
	case Behavior::kDead:
		BehaviorDeadUpdate();
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
	Invincible();
	playerBullets_.Update(GetPos());
	Animation();

	worldTransform_.Update();
}

void Player::Draw()
{
	if (isDead_)
	{
		return;
	}

	if (behavior_ == Behavior::kAstral)
	{
		int texX = (animationCount * 64);
		int texY = static_cast<int>(animationBehavior_) * 64 + 2;
		int scaleX = 64;

		switch (animationBehavior_)
		{
		case AnimationBehavior::kAstralRoot:
			texY = 0;
			break;
		case AnimationBehavior::kAstralAttack:
			texY = 64;
			//texX = 0;
			break;
		case AnimationBehavior::kAstralDeath:
			texY = 128;
			if (texX >= (3 * 64) - 1) {
				texX = 192;
			}
			break;
		default:
			texY = 0;
			break;
		}

	

		astralBodySprite_->Draw(worldTransform_, camera_, texX, texY, scaleX, 64, lrDirection_);

		// 肉体（留まってる本体）描画も必要なら
		int playerTexY = 72 * 5;
		playerSprite_->Draw(tentativeWorldTransform_, camera_, 0, playerTexY, 68, 72, templrDirection_);

		//DrawHitBox(worldTransform_, camera_, int(astralSize.x), int(astralSize.y));
	}
	else
	{

		if (invincibleTimer_ % 6 == 0) {
			int texX = (animationCount * 58);
			int texY = static_cast<int>(animationBehavior_) * 72 + 2;
			int scaleX = 58;

			playerSprite_->SetColor(0xffffffff); // 通常色
			playerSprite_->Draw(worldTransform_, camera_, texX, texY, scaleX, 72, lrDirection_);
		}

		//DrawHitBox(worldTransform_, camera_, int(nomalSize.x), int(nomalSize.y));
	}

	if (animationBehavior_ == AnimationBehavior::kDamage)
	{
		//int texX = (animationCount * 58);
		//playerSprite_->Draw(worldTransform_, camera_, texX, 72 * 4, 58, 72, lrDirection_);
	}


	playerBullets_.Draw();

	ui_->Draw(GetHp(),GetAstralTimer(), GetCoolTime());

	// Debug表示
	/*Novice::ScreenPrintf(30, 50, "behavior_: %d", behavior_);
	Novice::ScreenPrintf(30, 70, "behaviorNext_: %d", behaviorNext_);
	Novice::ScreenPrintf(30, 90, "coolTimer: %.2f", coolTime_);
	Novice::ScreenPrintf(30, 110, "Astralbehavior_: %d", Astralbehavior_);
	Novice::ScreenPrintf(30, 130, "AstralbehaviorNext_: %d", AstralbehaviorNext_);
	Novice::ScreenPrintf(30, 150, "astralBodyTimer: %f", astralBodyTimer_);
	Novice::ScreenPrintf(1000, 10, "invincible: %d", isInvincible_);*/

	Novice::ScreenPrintf(0, 500, "currentBehavier = %d", behavior_);
}
void Player::Move()
{
	if (isDead_)
	{
		return;
	}

	Vector2 acc = InputMove(false, isMove, lrDirection_);

	if (isMove)
	{
		vel_ += acc;
		vel_.x = std::clamp(vel_.x, -kPlayerSpeedMax, kPlayerSpeedMax);
	}
	else
	{
		vel_.x = 0.0f;
	}

	if (onGround)
	{
		if (Keys::IsTrigger(DIK_SPACE) || Keys::IsTrigger(DIK_Z))
		{
			Novice::PlayAudio(jumpSFX, 0, 0.5f);
			vel_.y = kJumpAcceleration; // ジャンプの加速度を設定
		}
	}
}

void Player::AstralMove()
{
	Vector2 acc = InputMove(true, isMove, lrDirection_);
	if (isMove)
	{
		vel_ += acc;
		vel_.x = std::clamp(vel_.x, -kPlayerSpeedMax, kPlayerSpeedMax);
		vel_.y = std::clamp(vel_.y, -kPlayerSpeedMax, kPlayerSpeedMax);

		worldTransform_.translation_ += vel_;
	}
	else
	{
		vel_ = Vector2(0.0f, 0.0f);
	}

	// マップ端と本体からの距離制限
	worldTransform_.translation_.x = std::clamp(
		worldTransform_.translation_.x,
		mapChipField_->kBlockWidth,
		mapChipField_->blockCountX_ * mapChipField_->kBlockWidth - mapChipField_->kBlockWidth * 2);

	worldTransform_.translation_.y = std::clamp(
		worldTransform_.translation_.y,
		mapChipField_->kBlockHeight,
		mapChipField_->blockCountY_ * mapChipField_->kBlockHeight - mapChipField_->kBlockHeight * 2);

	worldTransform_.translation_.x = std::clamp(
		worldTransform_.translation_.x,
		tentativeWorldTransform_.translation_.x - kAstralBodyMaxDistance_,
		tentativeWorldTransform_.translation_.x + kAstralBodyMaxDistance_);

	worldTransform_.translation_.y = std::clamp(
		worldTransform_.translation_.y,
		tentativeWorldTransform_.translation_.y - kAstralBodyMaxDistance_,
		tentativeWorldTransform_.translation_.y + kAstralBodyMaxDistance_);
}

Vector2 Player::InputMove(bool allowVertical, bool& moving, DrawSprite::LRDirection& direction)
{
	Vector2 acc{ 0.0f, 0.0f };
	moving = false;

	if (Keys::IsPress(DIK_D) || Keys::IsPress(DIK_RIGHT))
	{
		if (vel_.x < 0.0f)
		{
			vel_.x = 0.0f;
		}
		if (direction != DrawSprite::LRDirection::kRight)
		{
			direction = DrawSprite::LRDirection::kRight;
		}
		acc.x += kPlayerSpeed;
		moving = true;
	}

	if (Keys::IsPress(DIK_A) || Keys::IsPress(DIK_LEFT))
	{
		if (vel_.x > 0.0f)
		{
			vel_.x = 0.0f;
		}
		if (direction != DrawSprite::LRDirection::kLeft)
		{
			direction = DrawSprite::LRDirection::kLeft;
		}
		acc.x -= kPlayerSpeed;
		moving = true;
	}

	if (allowVertical)
	{
		if (Keys::IsPress(DIK_W) || Keys::IsPress(DIK_UP))
		{
			if (vel_.y < 0.0f)
			{
				vel_.y = 0.0f;
			}
			acc.y += kPlayerSpeed;
			moving = true;
		}

		if (Keys::IsPress(DIK_S) || Keys::IsPress(DIK_DOWN))
		{
			if (vel_.y > 0.0f)
			{
				vel_.y = 0.0f;
			}
			acc.y -= kPlayerSpeed;
			moving = true;
		}
	}

	// 斜め移動の速度を調整
	if (moving)
	{
		acc = acc.normalize();
		acc.x *= kPlayerSpeed;
		acc.y *= kPlayerSpeed;
	}

	return acc;
}

void Player::MapCollision(CollisonMapInfo& info) {
	MapCollisionTop(info);
	MapCollisionBottom(info);
	MapCollisionLeft(info);
	MapCollisionRight(info);
	auto index = mapChipField_->GetMapChipIndexByPosition(worldTransform_.translation_);
	auto type = mapChipField_->GetMapChipTypeIndex(index.xIndex, index.yIndex);
	if (type == MapChipType::kClear)
	{
		isClear = true;
	}
	if (type == MapChipType::kBlock)
	{
		worldTransform_.translation_.x -= kWidth * 2.5f;
	}
	auto index2 = mapChipField_->GetMapChipIndexByPosition(worldTransform_.translation_ + Vector2(0.f, kHeight / 2));
	auto type2 = mapChipField_->GetMapChipTypeIndex(index2.xIndex, index2.yIndex);
	if (type2 == MapChipType::kThorn && nomalBodyHP != 0)
	{
		nomalBodyHP = 0;
		animationBehaviorNext_ = AnimationBehavior::kDeath;
		behaviorNext_ = Behavior::kDead;
		
	}
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
		if (type == MapChipType::kBlock && typeNext != MapChipType::kBlock) {
			index = mapChipField_->GetMapChipIndexByPosition(worldTransform_.translation_ + info.vel + Vector2(0.f, kHeight / 2));
			auto indexSetNext = mapChipField_->GetMapChipIndexByPosition(worldTransform_.translation_ + Vector2(0.f, kHeight / 2));
			if (index.yIndex != indexSetNext.yIndex) {
				//  マップチップの矩形を取得
				auto rect = mapChipField_->GetRectByIndex(index.xIndex, index.yIndex);

				info.vel.y = max(0.0f, rect.bottom - worldTransform_.translation_.y - (kHeight / 2) - kBlank);
				info.Top = true;
				break;
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
		if (type == MapChipType::kBlock && typeNext != MapChipType::kBlock) {
			index = mapChipField_->GetMapChipIndexByPosition(worldTransform_.translation_ + info.vel - Vector2(0.f, kHeight / 2));
			auto indexSetNext = mapChipField_->GetMapChipIndexByPosition(worldTransform_.translation_ - Vector2(0.f, kHeight / 2));
			if (index.yIndex != indexSetNext.yIndex) {
				auto rect = mapChipField_->GetRectByIndex(index.xIndex, index.yIndex);
				info.vel.y = min(0.0f, rect.top - worldTransform_.translation_.y + (kHeight / 2) + kBlank);
				info.Bottom = true;
				break;
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
		if (type == MapChipType::kBlock) {
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
		if (type == MapChipType::kBlock) {
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
void Player::OnCollisionNomal(const Enemies* enemies)
{
	(void)enemies;
	
	
	// ダメージ無敵時間中と死んでる時はダメージを受けない
	if (damageCooldown_ > 0 || isDead_)
	{
		return;
	}

	// ダメージ処理
	nomalBodyHP--;

	//// HPが0以下になったら死亡行動へ移行
	//if (nomalBodyHP <= 0)
	//{
	//	animationBehaviorNext_ = AnimationBehavior::kDeath;
	//	behaviorNext_ = Behavior::kDead;
	//}

	//// アニメーションが終わったら死亡フラグをオンにする
	//if (deathCount <= 0.0f)
	//{
	//	isDead_ = true;
	//}

	// ダメージ無敵時間をリセット
	damageCooldown_ = damageCooldownMax;

	// ノックバック処理
	if (enemies)
	{
		Novice::PlayAudio(bodyDamagedSFX, 0, 1.0f);

		// 点滅アニメーション切り替え
		animationBehaviorNext_ = AnimationBehavior::kDamage;

		Vector2 enemyPos = enemies->GetPos();
		//Vector2 knockDir = (worldTransform_.translation_ - enemyPos).normalize();

		if (worldTransform_.translation_.x >= enemyPos.x) {
			knockback_.x = 5.2f;
			lrDirection_ = DrawSprite::LRDirection::kLeft;
		}
		else {
			knockback_.x = -5.2f;
			lrDirection_ = DrawSprite::LRDirection::kRight;
		}

		behaviorNext_ = Behavior::kKnockback;
		BehaviorKnockbackInitialize();
	}
}

void Player::OnCollisionAstral(const Enemies* enemies)
{
	(void)enemies;
	
	if (Astralbehavior_ == AstralBehavior::kKnockback)
	{
		return; // 連続ヒット防止
	}
	Novice::PlayAudio(switchBodySFX, 0, 1.0f);
	// ノックバック方向（敵が左なら右に飛ばす）
	Vector2 enemyPos = enemies->GetPos();
	if (worldTransform_.translation_.x >= enemyPos.x) {
		knockback_.x = 3.2f;
		lrDirection_ = DrawSprite::LRDirection::kLeft;
	}
	else {
		knockback_.x = -3.2f;
		lrDirection_ = DrawSprite::LRDirection::kRight;
	}

	AstralbehaviorNext_ = AstralBehavior::kKnockback;
	animationBehaviorNext_ = AnimationBehavior::kAstralDeath;
	AstralBodyBehaviorKnockbackInitialize();

	pendingAstralDamage_ = true; // ダメージ保留
}



void Player::BehaviorRootInitialize()
{
	{
		behavior_ = Behavior::kRoot;
		vel_ = Vector2(0, 0);
		worldTransform_.scale_ = Vector2(1.0f, 1.0f);
		isAstral = false;
		astralBodyHP = maxAstralBodyHP;

		// 弾削除
		playerBullets_.ClearHoldingBullets();

	}
}
void Player::BehaviorRootUpdate()
{
	if (coolTime_ > 0.0f) {
		coolTime_ -= frameTime;
		coolTime_ = max(0.0f, coolTime_);
	}

	if ((Keys::IsTrigger(DIK_F) && onGround) || (Keys::IsTrigger(DIK_X) && onGround))
	{
		templrDirection_ = lrDirection_;
		SwitchBody();
	}

	Move();
}

void Player::BehaviorAttackInitialize()
{

}

void Player::BehaviorAttackUpdate()
{
	//behaviorNext_ = Behavior::kRoot; // 攻撃行動が終了したらルートに戻る
}

void Player::BehaviorAstralInitialize()
{
	coolTime_ = 3.f;
	astralBodyTimer_ = 0.0f;
	behavior_ = Behavior::kAstral;
	currentBullets_ = 0;
	isAstral = true;
	animationBehaviorNext_ = AnimationBehavior::kAstralRoot;

	// 弾初期化
	playerBullets_.Initialize(GetPos(), camera_);
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
		case AstralBehavior::kKnockback:
			AstralBodyBehaviorKnockbackInitialize();
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
	case AstralBehavior::kKnockback:
		AstralBodyBehaviorKnockbackUpdate();
		break;
	default:
		break;
	}
}

void Player::BehaviorKnockbackInitialize()
{

	behavior_ = Behavior::kKnockback;
	knockbackTimer_ = knockbackDuration_;
	invincibleTimer_ = maxInvincibleTimer;
	vel_ = { 0.0f, 0.0f };
}

void Player::BehaviorKnockbackUpdate()
{

	// ノックバックの移動処理
	CollisonMapInfo info;
	info.vel = knockback_;

	MapCollision(info);
	MapCollisionMove(info);
	MapAfterCollision(info);
	MapWallCollision(info);
	GroundStates(info);


	knockbackTimer_ -= frameTime;
	if (knockbackTimer_ <= 0.0f)
	{
		// HPが0以下になったら死亡行動へ移行
		if (nomalBodyHP <= 0)
		{
			animationBehaviorNext_ = AnimationBehavior::kDeath;
			behaviorNext_ = Behavior::kDead;
		}
		else
		{
			behaviorNext_ = Behavior::kRoot;
		}
	}
}

void Player::BehaviorDeadInitialize()
{
	behavior_ = Behavior::kDead;
	
}

void Player::BehaviorDeadUpdate()
{
	
}

void Player::SwitchBody()
{
	if (coolTime_ > 0.0f)
	{
		return;
	}
	Novice::PlayAudio(switchBodySFX, 0, 1.0f);
	// 霊体化する前の位置を保存
	tentativeWorldTransform_ = worldTransform_;

	behaviorNext_ = Behavior::kAstral;
}

void Player::AstralBodyBehaviorRootInitialize()
{
	Astralbehavior_ = AstralBehavior::kRoot;
	attackTimer = kAttackTime;
	animationBehaviorNext_ = AnimationBehavior::kAstralRoot;
}

void Player::AstralBodyBehaviorRootUpdate()
{
	AstralMove();

	astralBodyTimer_ += frameTime;

	if (astralBodyTimer_ >= astralBodyDuration_)
	{
		astralBodyTimer_ = 0.0f;
		behaviorNext_ = Behavior::kRoot;
		worldTransform_ = tentativeWorldTransform_;
		animationBehaviorNext_ = AnimationBehavior::kRoot;
	}

	// 本体に戻る
	if (Keys::IsTrigger(DIK_F) || Keys::IsTrigger(DIK_X))
	{

		// 早く戻りすぎた場合クールタイムを消費しない
		if (astralBodyTimer_ <= 0.5f)
		{
			coolTime_ = 0.0f;
		}

		astralBodyTimer_ = 0.0f;

		Novice::PlayAudio(switchBodySFX, 0, 1.0f);

		behaviorNext_ = Behavior::kRoot;
		worldTransform_ = tentativeWorldTransform_;
		animationBehaviorNext_ = AnimationBehavior::kRoot;
	}

	// 攻撃クールタイム更新
	attackTimer -= frameTime;

	// クールタイムが0未満にならないように制限
	if (attackTimer < 0.0f)
	{
		attackTimer = 0.0f;
	}

	// 弾発射
	if ((Keys::IsPress(DIK_SPACE) && attackTimer <= 0) || (Keys::IsTrigger(DIK_Z) && attackTimer <= 0))
	{
		Novice::PlayAudio(shootSFX, 0, 1.0f);
		attackTimer = kAttackTime; // クールタイムリセット
		AstralbehaviorNext_ = AstralBehavior::kAttack;
	}
}

void Player::AstralBodyBehaviorAttackInitialize()
{

	Astralbehavior_ = AstralBehavior::kAttack;
	animationBehaviorNext_ = AnimationBehavior::kAstralAttack;
	attackTimer = 0.0f;
	animationLagTime = 0.3f;

	// 最大数以下なら弾を発射
	if (currentBullets_ <= maxBullets_)
	{
		// 弾の速度
		Vector2 dir = GetDir();

		// 速度ベクトルを自キャラの向きに合わせて変更
		dir = TransformNormal(dir, worldTransform_.matWorld_);

		// 弾発射
		playerBullets_.Shot(dir);
	}
}

void Player::AstralBodyBehaviorAttackUpdate()
{
	if(animationLagTime <= 0.0f)
	{
		// 弾の発射カウント
		currentBullets_++;

		// 攻撃後は通常状態に戻る
		AstralbehaviorNext_ = AstralBehavior::kRoot;
	}

	animationLagTime -= frameTime;
}

void Player::AstralBodyBehaviorKnockbackInitialize()
{
	Astralbehavior_ = AstralBehavior::kKnockback;
	animationTimer = 0;
	knockbackTimer_ = 0.5f;
	vel_ = { 0.0f, 0.0f };
}

void Player::AstralBodyBehaviorKnockbackUpdate()
{
	// ノックバック移動
	worldTransform_.translation_ += knockback_;

	//// シェイク量の計算（強め＆ハイ周波数）
	//float shakeAmplitude = 5.0f; // 揺れの強さ（少し大きく）
	//float shakeFrequency = 80.0f; // 揺れの速さ
	//float elapsed = knockbackDuration_ - knockbackTimer_;
	//float shakeOffsetX = sinf(elapsed * shakeFrequency) * shakeAmplitude;
	//float shakeOffsetY = cosf(elapsed * shakeFrequency * 1.5f) * (shakeAmplitude / 2.0f);

	//// シェイクを適用（ノックバック移動の後に追加）
	//worldTransform_.translation_.x += shakeOffsetX;
	//worldTransform_.translation_.y += shakeOffsetY;

	//// 点滅処理
	//if (static_cast<int>(elapsed * 15) % 2 == 0)
	//{
	//	//sprite_.SetColor({ 1.0f, 0.2f, 0.2f, 1.0f });
	//}
	//else
	//{
	//	//sprite_.SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	//}

	// 制限範囲
	worldTransform_.translation_.x = std::clamp(
		worldTransform_.translation_.x,
		tentativeWorldTransform_.translation_.x - kAstralBodyMaxDistance_,
		tentativeWorldTransform_.translation_.x + kAstralBodyMaxDistance_);
	worldTransform_.translation_.y = std::clamp(
		worldTransform_.translation_.y,
		tentativeWorldTransform_.translation_.y - kAstralBodyMaxDistance_,
		tentativeWorldTransform_.translation_.y + kAstralBodyMaxDistance_);

	knockbackTimer_ -= frameTime;

	if (knockbackTimer_ <= 0.0f)
	{
		if (pendingAstralDamage_)
		{
			astralBodyHP--;
			pendingAstralDamage_ = false;

			if (astralBodyHP <= 0)
			{
				astralBodyTimer_ = 0.0f;
				behaviorNext_ = Behavior::kRoot;
				worldTransform_ = tentativeWorldTransform_;
				animationBehaviorNext_ = AnimationBehavior::kRoot;
				//sprite_.SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
				return;
			}
		}

		AstralbehaviorNext_ = AstralBehavior::kRoot;
		//sprite_.SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	}
}

void Player::Animation()
{
	//animationTimer++;
	if (animationBehaviorNext_ != AnimationBehavior::kUnknown)
	{
		animationBehavior_ = animationBehaviorNext_;
		animationBehaviorNext_ = AnimationBehavior::kUnknown;
		animationCount = 0;

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
		case Player::AnimationBehavior::kDamage:
			animationMax = 2;
			break;
		case Player::AnimationBehavior::kAstralBodyIdle:
			animationMax = 1;
			break;
		case Player::AnimationBehavior::kDeath:
			animationMax = 4;
			break;
		case AnimationBehavior::kAstralRoot:
			animationMax = 4;
			break;
		case AnimationBehavior::kAstralAttack:
			animationMax = 2;
			break;
		case AnimationBehavior::kAstralDeath:
			animationMax = 5;
			break;
		}
	}

	switch (animationBehavior_)
	{
	case Player::AnimationBehavior::kRoot:


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

		if (!onGround && vel_.y < 0.f)
		{
			animationBehaviorNext_ = Player::AnimationBehavior::kJumpDown;
		}
		break;
	case Player::AnimationBehavior::kJumpDown:

		if (onGround)
		{
			animationBehaviorNext_ = Player::AnimationBehavior::kRoot;
		}
		break;
	case Player::AnimationBehavior::kDamage:
		if (damageCooldown_ <= 0) {
			animationBehaviorNext_ = Player::AnimationBehavior::kRoot;
		}
		break;

	case Player::AnimationBehavior::kAstralBodyIdle:

		break;
	case AnimationBehavior::kAstralDeath:
		
		break;

	case Player::AnimationBehavior::kDeath:
		//deathCount -= frameTime;

		// 死亡アニメーション終了済みなら待機カウント
		if (deathAnimEnded_)
		{
			deathWaitTimer_++;
			if (deathWaitTimer_ >= 30)
			{
				isDead_ = true;
			}
		}

		break;
	case AnimationBehavior::kAstralRoot:
		
		break;
	case AnimationBehavior::kAstralAttack:
		
		break;
	/*case AnimationBehavior::kAstralDeath:
		
		break;*/
	default:
		break;
	}


	
	if (!deathAnimEnded_)
	{
		animationTimer++;
	}

	static int prevAnimationCount = -1;

	if (animationTimer >= 60 / (animationMax * 2)) 
	{
		animationTimer = 0;

		if (animationBehavior_ == Player::AnimationBehavior::kDeath)
		{
			// アニメーションをループさせず、最後の一枚で固定
			if (!deathAnimEnded_) 
			{
				animationCount = (animationCount + 1);

				if (animationCount >= animationMax) 
				{
					animationCount = animationMax - 1;  // 最後のフレームで止める
					deathAnimEnded_ = true;            // フラグで完了扱いに
				}
			}
		}
		else 
		{
			// 通常のループ処理
			animationCount = (animationCount + 1) % animationMax;
		}
	}
}

bool Player::isPushButton(BlockButton* button)
{
	Vector2 playerPos = worldTransform_.translation_;

	if (behavior_ == Behavior::kAstral)
	{
		// 幽体の時本体の位置を求める
		playerPos = tentativeWorldTransform_.translation_;
	}
	std::array<Vector2, kNumCorner> posNew;
	for (uint32_t i = 0; i < posNew.size(); ++i) {

		posNew[i] = CornerPos(playerPos, static_cast<Corner>(i));
	}

	std::array<Corner, 2> checkCorners = { kLeftBottom, kRightBottom };

	Vector2 btnPos = button->getPos();
	float halfSize = 32.0f;

	for (auto corner : checkCorners) {
		Vector2 c = posNew[corner];
		if (c.x >= btnPos.x - halfSize && c.x <= btnPos.x + halfSize &&
			c.y >= btnPos.y - halfSize && c.y <= btnPos.y + halfSize) {
			return true;
		}
	}

	return false;
}

void Player::Invincible()
{
	if (invincibleTimer_ > 0) {
		isInvincible_ = true;
		invincibleTimer_--;
	}
	else {
		isInvincible_ = false;
	}
}
