#pragma once
class BGM {
public:
	BGM();
	~BGM() = default;
	void Initialize();
	void SetVolume(const float& volume);
	void Stop();
	void PlayBGM(int const& num);
private:
	int title_;
	int stage1_;
	int stage2_;
	int stage3_;

	int playTitle_;
	int playStage1_;
	int playStage2_;
	int playStage3_;

	float volume_;
};