#pragma once
#include "ScreenObject.h"

class Raft : public ScreenObject {
public:
	Raft(Size size, XY pos, RGB color);
	Raft(Size size, XY pos);

public:
	void _Draw() const override;
	void _PollEvents(SDL_Event& event) override;
	void _SetRange(uint16_t minX, uint16_t maxX);
	void _SetWindow(SDL_Window* window);
	XY _Location() const;
	Size _Size() const;
	uint16_t _Speed() const;

private:
	Size size;
	XY pos;
	RGB color;
	uint16_t minX;
	uint16_t maxX;
	uint16_t speed;
	SDL_Window* window{};
};

inline void Raft::_SetRange(uint16_t minX, uint16_t maxX) {
	this->minX = minX, this->maxX = maxX;
}

inline void Raft::_SetWindow(SDL_Window* window) {
	this->window = window;
}

inline XY Raft::_Location() const {
	return this->pos;
}

inline Size Raft::_Size() const {
	return this->size;
}

inline uint16_t Raft::_Speed() const {
	return this->speed;
}