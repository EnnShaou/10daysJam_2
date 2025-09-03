#pragma once
#include"Vector2.h"
#include"DrawFunc.h"
#include"WtF.h"
#include"Camera.h"
#include"MapChipField.h"

// ------------------ 敵の基底クラス ------------------
class Enemies
{
public:
    Enemies() = default;
    ~Enemies();

    // 矩形の四隅を示す列挙型（当たり判定用）
    enum Corner
    {
        kRightBottom,   // 右下
        kLeftBottom,    // 左下
        kLeftTop,       // 左上
        kRightTop,      // 右上
        kNumCorner      // 隅の数
    };

    // マップ衝突情報
    struct CollisonMapInfo
    {
        bool Top = false;      // 上方向との衝突
        bool Bottom = false;   // 下方向との衝突（地面）
        bool LR = false;       // 左右方向との衝突
        Vector2 vel;           // 移動量（速度ベクトル）
    };

    // 初期化（カメラ、初期位置）
    virtual void Initialize(Camera* camera, Vector2& pos) = 0;

    // 更新処理
    virtual void Update() = 0;

    // 描画処理
    virtual void Draw() = 0;

    // --- 衝突判定関連 ---
    void MapColdlision(CollisonMapInfo& info);   // マップ全体との衝突判定

protected:
    DrawSprite* sprite = nullptr;   // 敵スプライト
    WtF wtf;                        // 変換行列？（詳細不明）
    Camera* camera_ = nullptr;      // カメラ参照

    // 当たり判定のサイズ
    float kWidth = 60;              // 当たり判定の幅
    float kHeight = 60;             // 当たり判定の高さ
    float kBlank = 2.f;             // 当たり判定の余白（境界ずれ防止）

private:
    // 以下は詳細な方向別の衝突判定処理（現状コメントアウト）
    //void MapCollisionTop(CollisonMapInfo& info);         // 上方向の衝突判定
    //void MapCollisionBottom(CollisonMapInfo& info);      // 下方向の衝突判定
    //void MapCollisionLeft(CollisonMapInfo& info);        // 左方向の衝突判定
    //void MapCollisionRight(CollisonMapInfo& info);       // 右方向の衝突判定
    //void MapWallCollision(CollisonMapInfo& info);        // 壁との衝突判定
    //void MapCollisionMove(const CollisonMapInfo& info);  // 衝突後の移動処理
    //void MapAfterCollision(const CollisonMapInfo& info); // 衝突後の状態処理
    //void GroundStates(const CollisonMapInfo& info);      // 地面との接地状態更新
};


// ------------------ かぼちゃ敵クラス（派生） ------------------
class EnemyPumpkin : public Enemies
{
public:
    EnemyPumpkin();
    ~EnemyPumpkin();

    // 初期化（カメラ、初期位置）
    void Initialize(Camera* camera, Vector2& pos) override;

    // 更新処理
    void Update() override;

    // 描画処理
    void Draw() override;

private:
    // ここにかぼちゃ敵固有の変数や処理を追加できる
};
