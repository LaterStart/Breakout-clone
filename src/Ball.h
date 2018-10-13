#pragma once
#include "ScreenObject.h"

class Brick;
class Player;
class Ball : public ScreenObject {
public:
	Ball(Size size, XY pos, RGB color);

public:
	void _Draw() const override;	
	void _UpdateLocation(std::vector<Raft*>* rafts, std::vector<std::vector<Brick*>*>* bricks);
	void _ResetPosition();
	void _IncreaseSpeed();
	void _DecreaseSpeed();
	void _SetSpeed(uint8_t speed);
	void _SetRange(uint16_t minX, uint16_t maxX, uint16_t minY, uint16_t maxY);
	bool _BrickCollision() const;
	bool _LifeLost() const;
	uint8_t _Speed() const;
	BrickID _BrickID() const;
	void _SetBrickID(BrickID brickID);
	
private:	
	void _DetectCollision(std::vector<Raft*>* rafts, std::vector<std::vector<Brick*>*>* bricks);
	void _Move();

private:
	Size size;
	XY pos;
	RGB color;
	uint16_t minX, maxX;
	uint16_t minY, maxY;
	uint8_t speed;
	int8_t xVelocity{ 1 };
	int8_t yVelocity{ 1 };
	int16_t angle{ 0 };
	bool brickCollision{};
	bool lifeLost{};
	BrickID brickID;
	XY spawnPos;
};

inline void Ball::_SetRange(uint16_t minX, uint16_t maxX, uint16_t minY, uint16_t maxY) {
	this->minX = minX; this->maxX = maxX;
	this->minY = minY; this->maxY = maxY;
}

inline uint8_t Ball::_Speed() const {
	return this->speed;
}

inline bool Ball::_BrickCollision() const {
	return this->brickCollision;
}

inline bool Ball::_LifeLost() const {
	return this->lifeLost;
}

inline BrickID Ball::_BrickID() const {
	return this->brickID;
}

inline void Ball::_IncreaseSpeed() {
	this->speed--;
}

inline void Ball::_DecreaseSpeed() {
	this->speed++;
}

inline void Ball::_SetSpeed(uint8_t speed) {
	this->speed = speed;
}

inline void Ball::_SetBrickID(BrickID brickID) {
	this->brickID = brickID;
}
