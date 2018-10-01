#include "ScreenObject.h"

Raft::Raft(Size size, XY pos, RGB color) :
	size(size), pos(pos), color(color) {
	type = ObjectType::raft;
}

Raft::Raft(Size size, XY pos) :
	size(size), pos(pos), color(color){
	type = ObjectType::raft;
}

void Raft::_Draw() const {
	SDL_Rect rect;

	rect.w = size.width;
	rect.h = size.height;
	rect.x = pos.x;
	rect.y = pos.y;

	if (texture)
		SDL_RenderCopy(renderer, texture, nullptr, &rect);
	else {
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(renderer, &rect);
	}
}

void Raft::_PollEvents(SDL_Event& event) {
	uint16_t x{ pos.x };
	switch (event.type) {
	case SDL_MOUSEMOTION:
		if (event.motion.x >= minX && event.motion.x <= maxX)
			pos.x = event.motion.x;
		else if (event.motion.x < minX) {
			pos.x = minX;
			SDL_WarpMouseInWindow(window, minX, event.motion.y);
		}
		else if (event.motion.x > maxX) {
			pos.x = maxX;
			SDL_WarpMouseInWindow(window, maxX, event.motion.y);
		}
		break;
	default:
		break;
	}
	speed = abs(x - pos.x);
}