#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "Utility.h"

class ScreenObject;
class Window {
public:
	Window(const char* title);
	~Window();

public:
	void _RenderObjects(std::vector<ScreenObject*>* objects);
	void _PollEvents(std::vector<ScreenObject*>* objects);	
	void _PollEvents();
	void _Clear() const;
	bool _IsOpen() const;
	Size _Size() const;
	bool _Click() const;
	inline SDL_Renderer* _Renderer() const;
	void _ResetClick();

private:
	bool _Initialize();

private:
	const char* title;
	bool opened;
	bool click{};
	Size size;
	SDL_Window * window{};
	SDL_Renderer * renderer{};
};

inline bool Window::_IsOpen() const {
	return opened;
};

inline Size Window::_Size() const {
	return this->size;
}

inline SDL_Renderer* Window::_Renderer() const {
	return this->renderer;
}

inline bool Window::_Click() const {
	return this->click;
}

inline void Window::_ResetClick() {
	this->click = false;
}