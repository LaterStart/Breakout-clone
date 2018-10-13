#include <iostream>
#include "ScreenObject.h"

Background::Background(Size windowSize) {
	this->windowSize = windowSize;
	type = ObjectType::background;
}

void Background::_Draw() const {
	SDL_Rect rect = { 0, 0, windowSize.width, windowSize.height };
	if (texture)
		SDL_RenderCopy(renderer, texture, nullptr, &rect);
}
