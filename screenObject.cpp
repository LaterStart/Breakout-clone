#include <iostream>
#include "ScreenObject.h"

void ScreenObject::_SetTexture(const char* imagePath) {
	this->imagePath = imagePath;
	SDL_Surface* surface{ IMG_Load(imagePath) };

	if (!surface) {
		std::cerr << "Failed to create surface\n";
		return;
	}

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	if (!texture) {
		std::cerr << "Failed to create texture\n";
		return;
	}

	SDL_FreeSurface(surface);
}

ScreenObject::~ScreenObject() {
	SDL_DestroyTexture(texture);
}