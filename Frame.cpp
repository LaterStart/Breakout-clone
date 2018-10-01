#include "ScreenObject.h"

Frame::Frame(Size size, XY pos, RGB color) :
	size(size), pos(pos), color(color) {
	type = ObjectType::frame;
}

Frame::Frame(Size size, XY pos) :
	size(size), pos(pos) {
	type = ObjectType::frame;
}

void Frame::_Draw() const { 
	SDL_Rect rect;

	rect.w = size.width;
	rect.h = size.height;
	rect.x = pos.x;
	rect.y = pos.y;

	if(texture)
		SDL_RenderCopy(renderer, texture, nullptr, &rect);
	else {
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(renderer, &rect);
	}
}