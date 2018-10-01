#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "Utility.h"

const enum class ObjectType {
	none, background, frame, raft, ball, brick, text
};

class ScreenObject {
public:
	virtual ~ScreenObject() = 0;

public:
	virtual void _Draw() const = 0;
	virtual void _SetTexture(const char* imagePath);
	virtual void _SetRenderer(SDL_Renderer* renderer);
	virtual void _PollEvents(SDL_Event& event);
	ObjectType _Type() const;
	bool _Visible() const;
	void _SetVisible(bool status);
		
protected:
	ObjectType type{ ObjectType::none };
	SDL_Renderer * renderer{};
	SDL_Texture* texture{};

private:
	const char* imagePath{};
	bool visible{ true };
};

inline void ScreenObject::_SetRenderer(SDL_Renderer* renderer) {
	this->renderer = renderer;
}

inline void ScreenObject::_PollEvents(SDL_Event& event) {
	return;
}

inline ObjectType ScreenObject::_Type() const {
	return this->type;
}

inline bool ScreenObject::_Visible() const {
	return this->visible;
}

inline void ScreenObject::_SetVisible(bool status) {
	this->visible = status;
}

#include "Brick.h"
#include "Raft.h"
#include "Frame.h"
#include "Ball.h"
#include "Background.h"
#include "Text.h"
