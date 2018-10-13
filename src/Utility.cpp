#include <SDL.h>
#include "Utility.h"

RGB::RGB(uint8_t r, uint8_t g, uint8_t b, uint8_t a) :
	r(r), g(g), b(b), a(a) {
}

XY::XY(uint16_t x, uint16_t y) :
	x(x), y(y) {
}

Size::Size(uint16_t width, uint16_t height) :
	width(width), height(height) {
}

Range::Range(uint16_t a, uint16_t b) :
	a(a), b(b) {
}

PixelArea::PixelArea(Range x, Range y) {
	// define pixel area
	uint16_t xs( x.b - x.a );
	uint16_t ys( y.b - y.a );
	pixels = new std::vector<XY>(xs * ys);

	// define pixel positions
	size_t z{};
	for (uint16_t i{}; i < ys; i++) {
		for (uint16_t j{}; j < xs; j++) {
			pixels->at(z).x = x.a + j;
			pixels->at(z++).y = y.a + i;
		}
	}
}

bool PixelArea::operator==(const PixelArea& second) {
	for (auto pixelA : *pixels) {
		for (auto pixelB : *second.pixels) {			
			if (pixelA.x == pixelB.x && pixelA.y == pixelB.y)
				return true;
		}
	}
	return false;
}

uint16_t _CalculatePercent(double percent, uint16_t originalAmount) {
	return uint16_t(originalAmount * percent / 100);
}

char* _CopyChar(const char* text) {
	int z{};
	while (*(text+z) != '\0') 
		z++;
	char* copy{ new char[z + 1] };
	while (z >= 0) 
		*(copy + z) = *(text+z--);
	return copy;
}

void _Pause(uint16_t seconds) {
	uint32_t currentTime{ SDL_GetTicks() };
	uint32_t wantedTime(currentTime + (1000 * seconds));
	while (currentTime<wantedTime) {
		currentTime = SDL_GetTicks();
	}
}
