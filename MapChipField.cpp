// MapChipField.cpp
#include "MapChipField.h"
#include <cassert>
#include <fstream>
#include <map>
#include <sstream>
namespace {
	std::map<std::string, MapChipType> mapChipTable = {
		{"-1", MapChipType::kBlank },
		{"0", MapChipType::kBlock },
		{"1", MapChipType::kButton1 },
		{"2", MapChipType::kGate1 },
		{"3", MapChipType::kButton2 },
		{"4", MapChipType::kGate2 },
		{"5", MapChipType::kButton3 },
		{"6", MapChipType::kGate3 },
		{"7", MapChipType::HiddenFloor },
		{"8", MapChipType::kThorn },
		{"9", MapChipType::EnemyPumpkin},
		{"10", MapChipType::EnemyLamp},
		{"11", MapChipType::EnemyBat},
		{"12", MapChipType::EnemyMummy},
		{"13", MapChipType::kPlayer},
		{"14", MapChipType::kClear},
		{"15", MapChipType::kButton4},
		{"16", MapChipType::kGate4},
		{"17", MapChipType::kButton5},
		{"18", MapChipType::kGate5},

	};
}

MapChipField::MapChipField() {}
MapChipField::~MapChipField() {}

void MapChipField::ResetMapChipData() {
	mapChipData_.data.clear();
	mapChipData_.data.resize(blockCountY_);
	for (auto& row : mapChipData_.data) {
		row.resize(blockCountX_, MapChipType::kBlank);
	}
}

void MapChipField::LoadMapChipCsv(const std::string& fileName) {
	std::ifstream file(fileName);
	assert(file.is_open());

	std::vector<std::string> lines;
	std::string line;
	uint32_t maxColumns = 0;

	while (std::getline(file, line)) {
		if (!line.empty()) {
			lines.push_back(line);
			std::stringstream ss(line);
			std::string cell;
			uint32_t columnCount = 0;
			while (std::getline(ss, cell, ',')) {
				++columnCount;
			}
			if (columnCount > maxColumns) {
				maxColumns = columnCount;
			}
		}
	}
	file.close();

	blockCountY_ = static_cast<uint32_t>(lines.size());
	blockCountX_ = maxColumns;

	ResetMapChipData();

	for (uint32_t y = 0; y < blockCountY_; ++y) {
		std::stringstream ss(lines[y]);
		std::string word;
		for (uint32_t x = 0; x < blockCountX_; ++x) {
			if (std::getline(ss, word, ',')) {
				if (mapChipTable.contains(word)) {
					mapChipData_.data[y][x] = mapChipTable[word];
				}
			}
		}
	}
}

MapChipType MapChipField::GetMapChipTypeIndex(uint32_t xIndex, uint32_t yIndex) {
	if (xIndex >= blockCountX_ || yIndex >= blockCountY_) {
		return MapChipType::kBlank;
	}
	return mapChipData_.data[yIndex][xIndex];
}

Vector2 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) { return Vector2(float(kBlockWidth * xIndex), float(kBlockHeight * (blockCountY_ - 1 - yIndex))); }

MapChipField::IndexSet MapChipField::GetMapChipIndexByPosition(const Vector2& pos) {
	IndexSet indexSet;
	indexSet.xIndex = static_cast<uint32_t>((pos.x + kBlockWidth / 2) / kBlockWidth);
	indexSet.yIndex = blockCountY_ - 1 - static_cast<uint32_t>((pos.y + kBlockHeight / 2) / kBlockHeight);
	return indexSet;
}

MapChipField::Rect MapChipField::GetRectByIndex(uint32_t xIndex, uint32_t yIndex) {
	Vector2 center = GetMapChipPositionByIndex(xIndex, yIndex);
	Rect rect;
	rect.left = center.x - kBlockWidth / 2;
	rect.right = center.x + kBlockWidth / 2;
	rect.bottom = center.y - kBlockHeight / 2;
	rect.top = center.y + kBlockHeight / 2;
	return rect;
}

void MapChipField::setMapChipData(const MapChipType Typ, uint32_t xIndex, uint32_t yIndex) {

	if (xIndex < blockCountX_ && yIndex < blockCountY_) {
		mapChipData_.data[yIndex][xIndex] = Typ;
	}
}
