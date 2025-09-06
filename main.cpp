#include <Novice.h>
#include"SceneManager.h"
#include"keys.h"
const char kWindowTitle[] = "6004_幽世の狐巫女";
const int kWindowWidth = 1280;//画面の横幅
const int kWindowHeight = 720;//画面の縦幅

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};
	Keys::Initialize(keys, preKeys);

	sceneManage* gameManager = new sceneManage();
	gameManager->Initialize();

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		
		gameManager->Update();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		
		gameManager->Draw();

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}
	// ライブラリの終了
	Novice::Finalize();
	delete gameManager;
	gameManager = nullptr;
	return 0;
}
