#include "Enemies.h"
#include <array>
#include <algorithm>



Enemies::~Enemies()
{
}

void Enemies::AstralBehavior() {
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

void Enemies::AstralBehaviorInitialize() {
	// プレイヤーが幽霊状態の時の処理
	behavior_ = Behavior::kAstral;

}

void Enemies::StopBehavior() {
	if (player_->GetBehavior() == Player::Behavior::kAstral) {
		behaviorNext_ = Behavior::kAstral;
	}
}

void Enemies::StopBehaviorInitialize() {
	// プレイヤーが本体状態の時の処理

	behavior_ = Behavior::kStop;
	vel_ = { 0.0f, 0.0f };
}

void Enemies::Animation() {
}


///////////////////////////////////////////////////////////////////////////////
//    																		 //
//    					マップチップの当たり判定生成クラスの関数	 			     //
//    					                                         		     //
///////////////////////////////////////////////////////////////////////////////
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

/// <summary>
/// Enemy Pumpkin
/// </summary>
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
	sprite = new DrawSprite(Novice::LoadTexture("./Resources/Enemies/pumpkin.png"), { 64,64 });
	sprite->SetColor(0xffffffff);
	camera_ = camera;
	wtf.Initialize();
	wtf.translation_ = pos;
	mapChipField_ = mapChipField;

	kSpeed = { 2.0f, 2.0f };  //かぼちゃの速さ
	kAtkRange = 240.0f;         //プレイヤーがこの範囲にいると動く

	kWidth = 60;              // 当たり判定の幅
	kHeight = 30;             // 当たり判定の高さ

	// アニメーション用の幅と縦サイズ
	imageWidth_ = 64;
	imageHeight_ = 64;
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

void EnemyPumpkin::Animation() {

	animationTimer_++;
	switch (behavior_) {
	case Behavior::kStop:
		animePosY_ = imageHeight_;
		animePosX_ = 0;
		animationMax_ = 1;

		break;
	case Behavior::kAstral:

		animePosY_ = 0;
		animationMax_ = 4;
		if (animationTimer_ % 12 == 0) {
			animePosX_ += imageWidth_;

		}
		if (animePosX_ >= imageWidth_ * animationMax_) {
			animePosX_ = 0;
		}

		break;
	default:
		break;

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

	Animation();

	// 移動更新
	wtf.translation_ += info.vel;

	wtf.Update();
}

void EnemyPumpkin::Draw()
{
	sprite->Draw(wtf, camera_, animePosX_, animePosY_, imageWidth_, imageHeight_);
}

/// <summary>
/// Enemy Lamp
/// </summary>
EnemyLamp::EnemyLamp()
{
}

EnemyLamp::~EnemyLamp()
{
	delete sprite;
	sprite = nullptr;

	delete camera_;
	camera_ = nullptr;
}

void EnemyLamp::Initialize(Camera* camera, Vector2& pos, MapChipField* mapChipField)
{
	sprite = new DrawSprite(Novice::LoadTexture("white1x1.png"), { 64,64 });
	sprite->SetColor(0xdfeb3dff);
	camera_ = camera;
	wtf.Initialize();
	wtf.translation_ = pos;
	mapChipField_ = mapChipField;

	kSpeed = { 1.2f, 1.2f };
	kAtkRange = 320.0f;
}

void EnemyLamp::Update()
{
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

	wtf.translation_ += vel_;
	wtf.Update();
}

void EnemyLamp::Draw()
{
	DrawCircle(wtf, camera_, int(lightRadius_), RED);
	sprite->Draw(wtf, camera_, 0, 0, 64, 64);
	
}

/// <summary>
/// Enemy Bat
/// </summary>
EnemyBat::EnemyBat() {

}

EnemyBat::~EnemyBat()
{
	delete sprite;
	sprite = nullptr;

	delete camera_;
	camera_ = nullptr;
}

void EnemyBat::Initialize(Camera* camera, Vector2& pos, MapChipField* mapChipField)
{
	sprite = new DrawSprite(Novice::LoadTexture("./Resources/Enemies/bat.png"), { 32,32 });
	sprite->SetColor(0xffffffff);
	camera_ = camera;
	wtf.Initialize();
	spawnPos_ = pos;
	wtf.translation_ = spawnPos_;
	mapChipField_ = mapChipField;

	kSpeed = { 1.6f, 1.0f };
	kAtkRange = 480.0f; //攻撃範囲 500

	// 当たり判定
	kWidth = 24.0f;
	kHeight = 24.0f;

	// 画像サイズ
	imageWidth_ = 32;
	imageHeight_ = 32;
}

void EnemyBat::Update()
{
	BehaviorNormal();
	
	Animation();
	// 移動更新
	wtf.translation_ += vel_;
	wtf.Update();
}

void EnemyBat::Draw()
{
	sprite->Draw(wtf, camera_, animePosX_, animePosY_, imageWidth_, imageHeight_, lrDirection_);
}

void EnemyBat::BehaviorNormal(){
	// 左右の動き
	if (wtf.translation_.x >= spawnPos_.x + maxMovementX_ ||
		wtf.translation_.x <= spawnPos_.x - maxMovementX_) {
		kSpeed.x = -kSpeed.x;
	}
	vel_.x = +kSpeed.x;

}

void EnemyBat::BehaviorAttack(){

	Vector2 playerPos;
	// コウモリをプレイヤーの本体のみに動かす
	if (player_->GetBehavior() == Player::Behavior::kAstral) {
		playerPos = player_->GetTentativePos();
	} else {
		playerPos = player_->GetPos();
	}

	//　プレイヤーが敵の攻撃範囲にいるかどうかを計算
	Vector2 pumpkinPos = wtf.translation_;
	Vector2 enemyToPlayer = { playerPos.x - pumpkinPos.x, playerPos.y - pumpkinPos.y };
	float distanceToPlayer = sqrtf(enemyToPlayer.x * enemyToPlayer.x + enemyToPlayer.y * enemyToPlayer.y);

	// プレイヤーの方向に動く
	if (distanceToPlayer <= kAtkRange) {
		behavior_ = BatBehavior::kAttack;
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
	} else {
		BehaviorReturn();
	}
}

void EnemyBat::BehaviorReturn(){
	//// vector from enemy to spawn
	//Vector2 enemyToSpawn = { spawnPos_.x - wtf.translation_.x,
	//						 spawnPos_.y - wtf.translation_.y };

	//// distance to spawn
	//float distanceToSpawn = sqrtf(enemyToSpawn.x * enemyToSpawn.x +
	//	enemyToSpawn.y * enemyToSpawn.y);

	//// if close enough, stop and switch back to Normal
	//if (distanceToSpawn < 1.0f) { // tolerance so it doesn’t jitter
	//	vel_ = { 0.0f, 0.0f };

	//	behavior_ = BatBehavior::kNormal;
	//	return;
	//}

	//// normalize direction
	//Vector2 dir = { enemyToSpawn.x / distanceToSpawn,
	//				enemyToSpawn.y / distanceToSpawn };

	//// move toward spawn with some speed
	//vel_.x = dir.x * kSpeed.x;
	//vel_.y = dir.y * kSpeed.y;
}

void EnemyBat::Animation(){
	
	//　アニメーションの更新
	animationTimer_++;
	switch (behavior_) {
	case BatBehavior::kNormal:
	case BatBehavior::kAttack:

		animePosY_ = 0;
		animationMax_ = 3;
		if (animationTimer_ % 12 == 0) {
			animePosX_ += imageWidth_;

		}
		if (animePosX_ >= imageWidth_ * animationMax_) {
			animePosX_ = 0;
		}

		break;
	case BatBehavior::kDead:

		animePosY_ = imageHeight_;
		animePosX_ = 0;
		animationMax_ = 1;

		break;
	default:
		break;
	}

	// ミイラの方向を取得
	if (kSpeed.x >= 0.0f) {
		lrDirection_ = DrawSprite::LRDirection::kRight;
	}
	else {
		lrDirection_ = DrawSprite::LRDirection::kLeft;
	}
}

/// <summary>
/// Enemy Mummy
/// </summary>
EnemyMummy::EnemyMummy()
{
}

EnemyMummy::~EnemyMummy() {
	delete sprite;
	sprite = nullptr;

	delete camera_;
	camera_ = nullptr;
}

void EnemyMummy::Initialize(Camera* camera, Vector2& pos, MapChipField* mapChipField)
{
	sprite = new DrawSprite(Novice::LoadTexture("./Resources/Enemies/mummy.png"), { 58,72 });
	sprite->SetColor(0xffffffff);
	camera_ = camera;
	wtf.Initialize();
	wtf.translation_ = pos;
	mapChipField_ = mapChipField;

	kSpeed = { 0.52f, 0.0f };
	kAtkRange = 0.0f; //攻撃範囲

	// 当たり判定
	kWidth = 32.0f;
	kHeight = 64.0f;

	// アニメーション用の幅と縦サイズ
	imageWidth_ = 58;
	imageHeight_ = 72;
}

void EnemyMummy::Update()
{

	vel_.x = +kSpeed.x;

	// マップチップの当たり判定 (上下当たり判定なし)
	CollisonMapInfo info;
	info.vel = vel_;
	MapCollisionRight(info);
	MapCollisionLeft(info);
	MapWallCollision(info);

	Animation();

	// 移動更新
	wtf.translation_.x +=  info.vel.x;
	wtf.Update();
}

void EnemyMummy::Draw()
{
	sprite->Draw(wtf, camera_, animePosX_, animePosY_, imageWidth_, imageHeight_, lrDirection_);
}

void EnemyMummy::MapWallCollision(CollisonMapInfo& info)
{
	// 左右の壁に当たると反射
	if (info.LR) {
		kSpeed.x = -kSpeed.x;
		
	}
}

void EnemyMummy::MapCollisionRight(CollisonMapInfo& info)
{
	if (info.vel.x <= 0) {
		return;
	}
	std::array<Vector2, kNumCorner> posNew;
	for (uint32_t i = 0; i < posNew.size(); ++i) {
		posNew[i] = CornerPos(wtf.translation_ + info.vel, static_cast<Corner>(i));
	}

	auto indexRT = mapChipField_->GetMapChipIndexByPosition(posNew[kRightTop]);
	auto indexRB = mapChipField_->GetMapChipIndexByPosition(posNew[kRightBottom]);

	auto typeRT = mapChipField_->GetMapChipTypeIndex(indexRT.xIndex, indexRT.yIndex);
	auto typeRB = mapChipField_->GetMapChipTypeIndex(indexRB.xIndex, indexRB.yIndex + 1);

	// --- 右上にブロックがあったら
	if (typeRT == MapChipType::kBlock) {
		auto index = mapChipField_->GetMapChipIndexByPosition(
			wtf.translation_ + info.vel + Vector2(kWidth / 2, 0.f));
		auto indexSetNext = mapChipField_->GetMapChipIndexByPosition(
			wtf.translation_ + Vector2(kWidth / 2, 0.f));

		if (index.xIndex != indexSetNext.xIndex) {
			auto rect = mapChipField_->GetRectByIndex(index.xIndex, index.yIndex);
			info.vel.x = min(0.0f, rect.left - wtf.translation_.x + (kWidth / 2) + kBlank);
			info.LR = true; 
		}
	}
	// --- 右下にブロックがなかったら
	else if (typeRB != MapChipType::kBlock) {
		info.LR = true; 
	}

	
}

void EnemyMummy::MapCollisionLeft(CollisonMapInfo& info)
{
	if (info.vel.x >= 0) {
		return;
	}
	std::array<Vector2, kNumCorner> posNew;
	for (uint32_t i = 0; i < posNew.size(); ++i) {
		posNew[i] = CornerPos(wtf.translation_ + info.vel, static_cast<Corner>(i));
	}
	
	auto indexLT = mapChipField_->GetMapChipIndexByPosition(posNew[kLeftTop]);
	auto indexLB = mapChipField_->GetMapChipIndexByPosition(posNew[kLeftBottom]);

	auto typeLT = mapChipField_->GetMapChipTypeIndex(indexLT.xIndex, indexLT.yIndex);
	auto typeLB = mapChipField_->GetMapChipTypeIndex(indexLB.xIndex, indexLB.yIndex + 1);

	// --- 左上にブロックがあったら
	if (typeLT == MapChipType::kBlock) {
		auto index = mapChipField_->GetMapChipIndexByPosition(wtf.translation_ + info.vel - Vector2(kWidth / 2, 0.f));
		auto indexSetNext = mapChipField_->GetMapChipIndexByPosition(wtf.translation_ - Vector2(kWidth / 2, 0.f));

		if (index.xIndex != indexSetNext.xIndex) {
			auto rect = mapChipField_->GetRectByIndex(index.xIndex, index.yIndex);
			info.vel.x = max(0.0f, rect.right - wtf.translation_.x - (kWidth / 2) + kBlank);
			info.LR = true;
		}
	}
	// --- 左下にブロックがなかったら
	else if (typeLB != MapChipType::kBlock) {
		info.LR = true;
	}
}

void EnemyMummy::Animation()
{
	//　アニメーションの更新
	animationTimer_++;
	switch (animationBehavior_) {
	case AnimationBehavior::kMove:
		
		animePosY_ = 0;
		animationMax_ = 3;
		if (animationTimer_ % 24 == 0) {
			animePosX_ += imageWidth_;

		}
		if (animePosX_ >= imageWidth_ * animationMax_) {
			animePosX_ = 0;
		}

		break;
	case AnimationBehavior::kStop:

		animePosY_ = imageHeight_;
		animePosX_ = 0;
		animationMax_ = 1;

		break;
	default:
		break;
	}

	// ミイラの方向を取得
	if (kSpeed.x >= 0.0f) {		
		lrDirection_ = DrawSprite::LRDirection::kRight;
	} else {
		lrDirection_ = DrawSprite::LRDirection::kLeft;
	}
	
}


