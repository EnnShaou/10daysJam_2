#include "BGM.h"

#include <Novice.h>

BGM::BGM() {
	
}

void BGM::Initialize() {
	title_ = Novice::LoadAudio("./Resources/Audio/bgm/title.mp3");
	stage1_ = Novice::LoadAudio("./Resources/Audio/bgm/stage1.mp3");
	stage2_ = Novice::LoadAudio("./Resources/Audio/bgm/stage2.mp3");
	stage3_ = Novice::LoadAudio("./Resources/Audio/bgm/stage2.mp3");

	playTitle_ = -1;
	playStage1_ = -1;
	playStage2_ = -1;
	playStage3_ = -1;

	volume_ = 0.15f;
}

void BGM::SetVolume(const float& volume) {
	if (Novice::IsPlayingAudio(playTitle_)) {
		Novice::SetAudioVolume(playTitle_, volume);
	}
	if (Novice::IsPlayingAudio(playStage1_)) {
		Novice::SetAudioVolume(playStage1_, volume);
	}
	if (Novice::IsPlayingAudio(playStage2_)) {
		Novice::SetAudioVolume(playStage2_, volume);
	}
	if (Novice::IsPlayingAudio(playStage3_)) {
		Novice::SetAudioVolume(playStage3_, volume);
	}
}

void BGM::Stop() {
	if (Novice::IsPlayingAudio(playTitle_)) {
		Novice::StopAudio(playTitle_);
	}
	if (Novice::IsPlayingAudio(playStage1_)) {
		Novice::StopAudio(playStage1_);
	}
	if (Novice::IsPlayingAudio(playStage2_)) {
		Novice::StopAudio(playStage2_);
	}
	if (Novice::IsPlayingAudio(playStage3_)) {
		Novice::StopAudio(playStage3_);
	}
}

void BGM::PlayBGM(int const& num) {
	switch (num) {
	case 0:
		if (!Novice::IsPlayingAudio(playTitle_)) {
			playTitle_ = Novice::PlayAudio(title_, 1, volume_);
		}
		break;
	case 1:
		if (!Novice::IsPlayingAudio(playStage1_)) {
			playStage1_ = Novice::PlayAudio(stage1_, 1, volume_);
		}
		break;
	case 2:
		if (!Novice::IsPlayingAudio(playStage2_)) {
			playStage2_ = Novice::PlayAudio(stage2_, 1, volume_);
		}
		break;
	case 3:
		if (!Novice::IsPlayingAudio(playStage3_)) {
			playStage3_ = Novice::PlayAudio(stage3_, 1, volume_);
		}
		break;
	default:
		if (!Novice::IsPlayingAudio(playTitle_)) {
			playTitle_ = Novice::PlayAudio(title_, 1, volume_);
		}
		break;
	}
}