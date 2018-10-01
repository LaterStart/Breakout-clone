#include "ScreenObject.h"

void Brick::_Draw() const {
	if (destroyed)
		return;
	SDL_Rect rect;

	rect.w = size.width;
	rect.h = size.height;
	rect.x = pos.x;
	rect.y = pos.y;

	if (texture)
		SDL_RenderCopy(renderer, texture, nullptr, &rect);
}

void Brick::_Hit() {
	hitPoints--;
	if (hitPoints == 0)
		destroyed = true;
}

Brick* Brick::_SpawnBrick(BrickConfig* cfg, SDL_Renderer* renderer) {
	Brick* brick{ new Brick() };
	if (cfg) {
		brick->type = ObjectType::brick;
		brick->hitPoints = cfg->hitPoints;
		brick->breakScore = cfg->breakScore;
		brick->breakSoundPath = cfg->breakSoundPath;
		brick->hitSoundPath = cfg->hitSoundPath;		
		brick->renderer = renderer;
		brick->_SetTexture(cfg->texturePath);
		if (brick->hitPoints == 0)
			brick->infinite = true;
	}
	else brick->destroyed = true;
	
	return brick;
}
