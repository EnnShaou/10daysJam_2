#pragma once

struct Rect
{
	// 左上の座標(x, y)と幅(w), 高さ(h)
    float x, y, w, h;

	// AABBの当たり判定
    inline bool IsCollision(const Rect& other) const
    {
        if (x + w <= other.x) return false;
        if (x >= other.x + other.w) return false;
        if (y + h <= other.y) return false;
        if (y >= other.y + other.h) return false;
        return true;
    }
};


