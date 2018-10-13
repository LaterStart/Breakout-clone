#pragma once
#include "ScreenObject.h"

class BrickConfig {
	friend class Level;
	friend class Brick;

private:
	char id;
	int hitPoints{};
	int breakScore{};
	const char* texturePath{};
	const char* hitSoundPath{};
	const char* breakSoundPath{};
};

class Brick : public ScreenObject {
public:
	void _Draw() const override;
	void _Hit();
	void _SetSize(Size size);
	bool _Infinite() const;
	bool _Destroyed() const;
	int _BreakScore() const;
	int _HitPoints() const;		
	Size _Size() const;	
	XY& _Location();

	static Brick* _SpawnBrick(BrickConfig* cfg, SDL_Renderer* renderer);

protected:
	int hitPoints;
	int breakScore;
	bool destroyed{};
	bool infinite{};
	const char* breakSoundPath{};
	const char* hitSoundPath{};
	Size size;
	XY pos;
	BrickID brickID;

};

inline int Brick::_BreakScore() const {
	return this->breakScore;
}

inline XY& Brick::_Location() {
	return this->pos;
}

inline bool Brick::_Destroyed() const {
	return this->destroyed;
}

inline void Brick::_SetSize(Size size) {
	this->size = size;
}

inline Size Brick::_Size() const {
	return this->size;
}

inline int Brick::_HitPoints() const {
	return this->hitPoints;
}

inline bool Brick::_Infinite() const {
	return this->infinite;
}
