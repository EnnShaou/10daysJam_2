#include "Game.h"
#include "ImGui.h"
#include"keys.h"

Game::Game() : mapChipField_(), camera_() {

}

Game::~Game() {

	delete mapChipField_;
	mapChipField_ = nullptr;
	// fade_の解放
	delete fade_;
	fade_ = nullptr;

	delete blockManger;
	blockManger = nullptr;
	delete player_;
	player_ = nullptr;
	delete camera_;
	camera_ = nullptr;
}

void Game::Initialize() {

	// マップチップフィールドの初期化
	mapChipField_ = new MapChipField();
	mapChipField_->LoadMapChipCsv("Resources/MapData/map_data1.csv");
	camera_ = new Camera({ 0,0 });
	camera_->Initialize(1280, 720);




	blockManger = new MapBlockManager();
	blockManger->setCamera(camera_);
	blockManger->setMapChipField(mapChipField_);
	// プレイヤーの初期化
	player_ = new Player();
	Vector2 playerPos = mapChipField_->GetMapChipPositionByIndex(2, 20);
	player_->Initialize(camera_, playerPos);
	player_->SetMapChipField(mapChipField_);
	blockManger->setPlayer(player_);
	camera_->setTarget(player_);
	// フェードの初期化
	fade_ = new Fade();
	fade_->Initialize();
	enemyManager.setPlayer(player_);
	blockManger->setEnemies(&enemyManager);
	GenerateBlocks();
#ifdef _DEBUG



#endif // _DEBUG
}

void Game::Update() {

	// デバッグカメラの更新
#ifdef _DEBUG


#endif

	switch (phase_) {
	case Game::Phase::kPlay:
		camera_->Update();

		player_->Update();
		enemyManager.UpDate(player_);
		// 全ての当たり判定をチェック
		CheckAllCollisions();
		blockManger->Update();
		break;
	case Game::Phase::kDeath:

		break;
	default:
		break;
	}

	ChangePhase(); // フェーズを変更する
}

void Game::Draw() {



	blockManger->Draw();
	player_->Draw();
	enemyManager.Draw();

	fade_->Draw(); // フェードの描画
}

void Game::GenerateBlocks() {
	// 要素数
	uint32_t numBlockX = mapChipField_->blockCountX_;
	uint32_t numBlockY = mapChipField_->blockCountY_;

	// 要素数を変更
	// 列数を設定(縦方向ブロック数)
	blockManger->setSize(numBlockX, numBlockY);

	// ブロックの生成
	for (uint32_t y = 0; y < numBlockY; y++) {
		for (uint32_t x = 0; x < numBlockX; x++) {

			MapChipType mapChipType = mapChipField_->GetMapChipTypeIndex(x, y);
			Vector2 pos = mapChipField_->GetMapChipPositionByIndex(x, y);
			if (mapChipType == MapChipType::kBlock) {

				blockManger->pushBlock(new Block(), pos, x, y);
			}
			if (mapChipType == MapChipType::kButton1)
			{
				auto* button = new BlockButtonAndGate();
				button->setBindID(1);  
				blockManger->pushBlock(button, pos, x, y);
			}
			if (mapChipType == MapChipType::kGate1)
			{
				auto* gate = new Gate();
				gate->setBindID(1);  
				blockManger->pushBlock(gate, pos, x, y);
			}
			if (mapChipType == MapChipType::kButton2)
			{
				auto* button = new BlockButtonAndGate();
				button->setBindID(2);  
				blockManger->pushBlock(button, pos, x, y);
			}
			if (mapChipType == MapChipType::kGate2)
			{
				auto* gate = new Gate();
				gate->setBindID(2);  
				blockManger->pushBlock(gate, pos, x, y);
			}
			if (mapChipType == MapChipType::kButton3)
			{
				auto* button = new BlockButtonAndGate();
				button->setBindID(3);  
				blockManger->pushBlock(button, pos, x, y);
			}
			if (mapChipType == MapChipType::kGate3)
			{
				auto* gate = new Gate();
				gate->setBindID(3);  
				blockManger->pushBlock(gate, pos, x, y);
			}
		}
	}
	//enemyの生成
	for (uint32_t y = 0; y < numBlockY; y++) {
		for (uint32_t x = 0; x < numBlockX; x++) {

			MapChipType mapChipType = mapChipField_->GetMapChipTypeIndex(x, y);

			if (mapChipType == MapChipType::EnemyPumpkin) {

				enemyManager.PushEnemyPumpkin(mapChipField_->GetMapChipPositionByIndex(x, y), camera_, mapChipField_);
				mapChipField_->setMapChipData(MapChipType::kBlank, x, y);

			}
		}
	}

	blockManger->BindButtonAndGates();

}

void Game::CheckAllCollisions() {
#pragma region "playerとenemiesの当たり判定"

#pragma endregion
}

void Game::ChangePhase() {
	switch (phase_) {
	case Game::Phase::FadeIn:
		fade_->Start(Fade::Status::FadeIn, 1.0f); // フェードインを開始
		phase_ = Game::Phase::kPlay;
		break;
	case Game::Phase::kPlay:
		fade_->Update(); // フェードの更新
		//if (player_->IsDead()) {
		//	// プレイヤーが死亡した場合、フェーズを変更
		//	phase_ = Game::Phase::kDeath;
		//}
		//if (Input::GetInstance()->TriggerKey(DIK_R)) {
		//	fade_->Start(Fade::Status::FadeOut, 1.0f); // フェードアウトを開始
		//	phase_ = GameScene::Phase::kFadeOut;       // フェーズをフェードアウトに変更
		//}
		//if (player_->IsClear()) {
		//	fade_->Start(Fade::Status::FadeOut, 1.0f); // フェードアウトを開始
		//	phase_ = GameScene::Phase::kFadeOut;       // フェーズをフェードアウトに変更
		//}
		break;
	case Game::Phase::kDeath:
		//// タイマーを初期化
		//if (player_deathEffects_->isFinished()) {      // 死亡エフェクトが終了したらシーンを終了
		//	fade_->Start(Fade::Status::FadeOut, 1.0f); // フェードアウトを開始
		//	phase_ = GameScene::Phase::kFadeOut;       // フェーズをフェードアウトに変更
		//}
		break;
	case Game::Phase::kFadeOut:
		fade_->Update(); // フェードの更新
		//if (player_->IsClear()) {
		//	if (fade_->IsFinished()) {   // フェードアウトが完了したらシーンを終了
		//		SceneNo = Scene::kClear; // シーンを終了
		//	}
		//}
		//else if (player_->IsDead()) {
		//	if (fade_->IsFinished()) {  // フェードアウトが完了したらシーンを終了
		//		SceneNo = Scene::kOver; // シーンを終了
		//	}
		//}
		//else {
		//	if (fade_->IsFinished()) {   // フェードアウトが完了したらシーンを終了
		//		SceneNo = Scene::kTitle; // シーンを終了
		//	}
		//}

		break;
	default:
		break;
	}
}


