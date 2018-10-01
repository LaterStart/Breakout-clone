#pragma once
#include <cstdint>
#include <vector>

struct RGB {
	uint8_t r, g, b, a;

	RGB(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	RGB() = default;
};

struct XY {
	uint16_t x;
	uint16_t y;

	XY(uint16_t x, uint16_t y);
	XY() = default;
};

struct Size {
	uint16_t width;
	uint16_t height;

	Size(uint16_t width, uint16_t height);
	Size() = default;
};

struct Range {
	uint16_t a, b;

	Range(uint16_t a, uint16_t b);
	Range() = default;
};

class PixelArea {
public:
	PixelArea(Range x, Range y);

public:
	bool operator==(const PixelArea& second);

private:
	std::vector<XY>* pixels{};
};

struct BrickID {
	uint8_t rowID;
	uint8_t columnID;
};

uint16_t _CalculatePercent(double percent, uint16_t originalAmount);
char* _CopyChar(const char* text);
void _Pause(uint16_t seconds);