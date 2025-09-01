#pragma once
class Fade {
public:
	Fade();
	~Fade();
	void Initialize();
	void Update();
	void Draw();
	enum class Status {
		None,   // フェードなし
		FadeIn, // フェードイン
		FadeOut // フェードアウト
	};
	void Start(Status status, float duration);
	void Stop();
	bool IsFinished() const;

private:
	int TextureHandle_ = 0;                 // テクスチャハンドル
	Status status_ = Status::None;               // フェードの状態
	float duration_ = 0.0f;                      // フェードの持続時間
	float count_ = 0.0f;                         // フェードのカウント
};