#pragma once
#include"Enemies.h"
#include <memory>
#include <list>

class Player;

// ------------------ 敵管理クラス ------------------
class EnemyManager
{
public:
    EnemyManager();
    ~EnemyManager();

    // 新しいかぼちゃ敵を生成してリストに追加
    void PushEnemyPumpkin(Vector2 pos, Camera* camera, MapChipField* mapChipField);
    void PushEnemyLamp(Vector2 pos, Camera* camera, MapChipField* mapChipField);
    void PushEnemyBat(Vector2 pos, Camera* camera, MapChipField* mapChipField);
    void PushEnemyMummy(Vector2 pos, Camera* camera, MapChipField* mapChipField);

    // 敵の更新処理（全ての敵に対してUpdateを呼ぶ）
    void Update();

    // 敵の描画処理（全ての敵に対してDrawを呼ぶ）
    void Draw();

    // プレイヤーの参照をセット（敵AIなどで利用する可能性あり）
    void setPlayer(Player* player);

    // --- ゲッター ---
    const std::list<Enemies*>& GetEnemies() const { return enemies_; }

private:
    std::list<Enemies*> enemies_;   // 敵のリスト（全ての敵を格納）
    Player* player_;                // プレイヤーの参照（敵がプレイヤーを追跡するため）
   
};


