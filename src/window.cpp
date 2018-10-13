#include <iostream>
#include "Window.h"
#include "ScreenObject.h"

Window::Window(const char* title) : title(title) {
	opened = _Initialize();
}

bool Window::_Initialize() {
	if (SDL_Init(SDL_INIT_VIDEO) + SDL_Init(SDL_INIT_AUDIO) != 0) {
		std::cerr << "SDL failed to initialize.\n";
		return false;
	}

	SDL_DisplayMode dm;	
	if (SDL_GetCurrentDisplayMode(0, &dm) != 0) {
		std::cerr << "Failed to get display mode.\n";
		return false;
	}

	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == 0) {
		std::cerr << "Failed to initialize SDL image\n";
		return false;
	}

	// max supported resolution
	if(dm.w > 1920)
		dm.w = 1920;
	if(dm.h > 1080)
		dm.h = 1080;

	window = SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		dm.w, dm.h,
		0
	);

	if (window == nullptr) {
		std::cerr << "Failed to create window.\n";
		return false;
	}
	size.width = dm.w;
	size.height = dm.h;
		
	SDL_ShowCursor(SDL_DISABLE);

	renderer = SDL_CreateRenderer(
		window, -1, SDL_RENDERER_ACCELERATED);

	if (renderer == nullptr) {
		std::cerr << "Failed to create renderer.\n";
		return false;
	}

	return true;
}

void Window::_RenderObjects(std::vector<ScreenObject*>* objects) {
	for (auto object : *objects) {
		if(object->_Visible())
			object->_Draw();
	}
}

void Window::_PollEvents(std::vector<ScreenObject*>* objects) {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			opened = false;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				opened = false;
				break;
			}
		case SDL_MOUSEBUTTONDOWN:
			click = true;
			break;
		default:
			break;
		}

		for (auto object : *objects)
			object->_PollEvents(event);
	}
}

void Window::_PollEvents() {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			opened = false;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				opened = false;
				break;
			}
		case SDL_MOUSEBUTTONDOWN:
			click = true;
			break;
		default:
			break;
		}
	}
}

void Window::_Clear() const {
	SDL_RenderPresent(renderer);	
	SDL_RenderClear(renderer);
}

Window::~Window() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	IMG_Quit();
	SDL_Quit();
}
