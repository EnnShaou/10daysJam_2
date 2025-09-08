#include "Fade.h"
#include <algorithm>

Fade::Fade() {}

Fade::~Fade() {

}

void Fade::Initialize() {
	TextureHandle_ = Novice::LoadTexture("white1x1.png"); // テクスチャの読み込み
	color_ = RGBA(0, 0, 0, 255);
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
		color_.A = static_cast<int>(
			255 - std::clamp(static_cast<float>(count_) / duration_, 0.0f, 1.0f) * 255
			);
		break;

	case Fade::Status::FadeOut:
		count_ += 1.f / 60.f;
		if (count_ >= duration_) {
			count_ = duration_;
		}
		color_.A = static_cast<int>(
			std::clamp(static_cast<float>(count_) / duration_, 0.0f, 1.0f) * 255
			);
		break;
	}
}

void Fade::Draw() {

	Novice::DrawSprite(0, 0, TextureHandle_, 1280, 720, 0.f, color_);

}

void Fade::Start(Status status, float duration) {

	status_ = status;     // フェードの状態を設定
	duration_ = duration; // フェードの持続時間を設定
	count_ = 0.0f;        // カウントをリセット
}

void Fade::Stop() {

	status_ = Status::None;              // フェードの状態を「なし」に設定
	count_ = 0.0f;                       // カウントをリセット
	color_.A = 255;
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
