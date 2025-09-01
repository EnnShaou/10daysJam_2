#pragma once
class Keys {
public:
	inline static char* keys_;
	inline static char* preKeys_;

	static void Initialize(char* keys, char* preKeys) {
		keys_ = keys;
		preKeys_ = preKeys;
	}

	// 判定関数
	static bool IsPress(int keyCode) {
		return keys_[keyCode];
	}

	static bool IsTrigger(int keyCode) {
		return keys_[keyCode] && !preKeys_[keyCode];
	}

	static bool IsRelease(int keyCode) {
		return !keys_[keyCode] && preKeys_[keyCode];
	}
};


