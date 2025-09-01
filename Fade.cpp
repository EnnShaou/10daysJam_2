#include "Fade.h"
#include <algorithm>
Fade::Fade() {}

Fade::~Fade() {

}

void Fade::Initialize() {
	//TextureHandle_ = TextureManager::Load("white1x1.png"); // テクスチャの読み込み
}

void Fade::Update() {

	switch (status_) {
	case Fade::Status::None:
		break;

	case Fade::Status::FadeIn:
		count_ += 1.0f / 60.0f;
		if (count_ >= duration_) {
			count_ = duration_;
		}
		//fadeSprite_->SetColor(Vector4(0, 0, 0, 1.0f - std::clamp(count_ / duration_, 0.0f, 1.0f)));
		break;

	case Fade::Status::FadeOut:
		count_ += 1.f / 60.f;
		if (count_ >= duration_) {
			count_ = duration_;
		}
		//fadeSprite_->SetColor(Vector4(0, 0, 0, std::clamp(count_ / duration_, 0.0f, 1.0f)));
		break;
	}
}

void Fade::Draw() {

	if (status_ == Status::None) {
	}
}

void Fade::Start(Status status, float duration) {

	status_ = status;     // フェードの状態を設定
	duration_ = duration; // フェードの持続時間を設定
	count_ = 0.0f;        // カウントをリセット
}

void Fade::Stop() {

	status_ = Status::None;              // フェードの状態を「なし」に設定
	count_ = 0.0f;                       // カウントをリセット
	//fadeSprite_->SetColor({ 0, 0, 0, 1 }); // スプライトの色を完全に不透明に設定
}

bool Fade::IsFinished() const {

	switch (status_) {
	case Fade::Status::FadeIn:
		if (count_ >= duration_) {
			return true; // フェードインが完了した場合はtrueを返す
		}
		else {
			return false; // フェードインが完了していない場合はfalseを返す
		}
		break;
	case Fade::Status::FadeOut:
		if (count_ >= duration_) {
			return true; // フェードアウトが完了した場合はtrueを返す
		}
		else {
			return false; // フェードアウトが完了していない場合はfalseを返す
		}
		break;
	default:
		break;
	}
	return true; // フェードが「なし」の場合はfalseを返す
}
