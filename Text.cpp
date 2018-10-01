#include "Text.h"

Text::Text(Size size, XY pos) {	
	type = ObjectType::text;
	this->size = size;
	this->pos = pos;
}

void Text::_Draw() const {
	SDL_Rect rect;

	rect.w = size.width;
	rect.h = size.height;
	rect.x = pos.x;
	rect.y = pos.y;

	if (texture)
		SDL_RenderCopy(renderer, texture, nullptr, &rect);
}