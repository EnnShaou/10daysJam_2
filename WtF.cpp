#include "WtF.h"
#include "vector2.h"
WtF::WtF() {
    matWorld_ = Identity();
}

WtF::~WtF() {}

void WtF::Initialize() {
    translation_ = { 0.0f, 0.0f };
    scale_ = { 1.0f, 1.0f };
    rotation_ = 0.0f;
    matWorld_ =Identity();
}

void WtF::Update() {
    // ローカル → ワールド変換行列 = S * R * T
    Matrix3x3 matS = MakeScaleMatrix(scale_);
    Matrix3x3 matR = MakeRotateMatrix(rotation_);
    Matrix3x3 matT = MakeTranslateMatrix(translation_);

    matWorld_ = matS * matR * matT;
}
