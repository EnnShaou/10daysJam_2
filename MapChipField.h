#pragma once
#include <string>
#include <vector>
#include "Vector2.h"

enum class MapChipType { kBlank = -1, kBlock, EnemyPumpkin = 9, EnemyLamp = 10 };

struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};

class MapChipField {
public:
	MapChipField();
	~MapChipField();

	struct IndexSet {
		uint32_t xIndex;
		uint32_t yIndex;
	};

	struct Rect {
		float left;
		float right;
		float bottom;
		float top;
	};

	static inline const float kBlockWidth = 64;
	static inline const float kBlockHeight = 64;
	uint32_t blockCountX_ = 0;
	uint32_t blockCountY_ = 0;

	void ResetMapChipData();
	void LoadMapChipCsv(const std::string& fileName);
	MapChipType GetMapChipTypeIndex(uint32_t xIndex, uint32_t yIndex);
	Vector2 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);
	MapChipField::IndexSet GetMapChipIndexByPosition(const Vector2& pos);
	MapChipField::Rect GetRectByIndex(uint32_t xIndex, uint32_t yIndex);
	void setMapChipData(const MapChipType Typ, uint32_t xIndex, uint32_t yIndex);

private:
	MapChipData mapChipData_;
};
