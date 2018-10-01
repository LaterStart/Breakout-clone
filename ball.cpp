#include "ScreenObject.h"
using namespace std;

Ball::Ball(Size size, XY pos, RGB color) :
	size(size), pos(pos), color(color){
	type = ObjectType::ball;
	spawnPos = pos;
}

void Ball::_Draw() const {
	SDL_Rect rect;

	rect.w = size.width;
	rect.h = size.height;
	rect.x = pos.x;
	rect.y = pos.y;

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer, &rect);
}

void Ball::_DetectCollision(std::vector<Raft*>* rafts, std::vector<std::vector<Brick*>*>* bricks) {
	// reset state
	lifeLost = false;
	brickCollision = false;

	// next ball position
	uint16_t xb(pos.x + xVelocity);
	uint16_t yb(pos.y + yVelocity);

	// check if ball will hit frames or life is lost
	if (xb <= minX || xb >= maxX) {
		// side frame is hit - change x velocity
		xVelocity *= -1;
		return;
	}
	if (yb <= minY) {
		// top frame is hit - change y velocity
		yVelocity *= -1;
		return;
	}
	if (yb >= maxY) {
		// life is lost
		lifeLost = true;
		return;
	}

	PixelArea ballBotPixels{ Range(xb, xb + size.width), Range(yb + size.height - (size.height / 4), yb + size.height) };

	// check if ball will hit one of the rafts
	if (yb > pos.y) {
		for (auto raft : *rafts) {
			XY rxy = raft->_Location();
			Size rs = raft->_Size();

			if (yb + size.height >= rxy.y) {
				// compare object pixels - allows side hit
				PixelArea raftPixels{ Range(rxy.x, rxy.x + rs.width), Range(rxy.y, rxy.y + (rs.height / 3)) };
				if (ballBotPixels == raftPixels) {
					yVelocity *= -1;
					angle = 0;

					// change x velocity if is over the center
					uint16_t centerX(rxy.x + (rs.width / 2));
					if ((xb + size.height < centerX && xb > pos.x) || (xb > centerX && xb < pos.x))
						xVelocity *= -1;

					// check corner hit
					PixelArea leftCorner{ Range(rxy.x, rxy.x + (size.width / 4)), Range(rxy.y, rxy.y + (rs.height / 3)) };
					PixelArea rightCorner{ Range(rxy.x + rs.width - (size.width / 4), rxy.x + rs.width), Range(rxy.y, rxy.y + (rs.height / 3)) };
					if ((xb > pos.x && ballBotPixels == leftCorner) || (xb < pos.x && ballBotPixels == rightCorner))
						angle = 2;

					angle += uint16_t((raft->_Speed() * 0.25));
					return;
				}
			}
		}
	}

	// check if brick hit
	Brick* firstRowBrick{ bricks->front()->at(0) };
	Brick* lastRowBrick{ bricks->back()->at(0) };
	uint16_t y1{ firstRowBrick->_Location().y };
	uint16_t y2( lastRowBrick->_Location().y + lastRowBrick->_Size().height);
	
	if (yb + size.height >= y1 && yb <= y2) {
		uint8_t index_i{}, index_j{};
		for (auto brickRow : *bricks) {
			index_j = 0;
			for (auto brick : *brickRow) {
				XY* bPos{ &brick->_Location() };
				Size bSize{ brick->_Size() };

				// if top or bottom brick hit
				if ((yb < pos.y && yb == (bPos->y + bSize.height)) || (yb > pos.y && yb + size.height == bPos->y)) {
					if ((xb >= bPos->x && xb <= bPos->x + bSize.width) || (xb + size.width >= bPos->x && xb + size.width <= bPos->x + bSize.width)) {
						_SetBrickID(BrickID{ index_i, index_j });
						brickCollision = true;
						yVelocity *= -1;
						return;
					}
				}

				// if side hit
				if ((yb < pos.y && (yb < bPos->y + bSize.height && yb >= bPos->y)) || (yb > pos.y && (yb + size.height > bPos->y && yb + size.height <= bPos->y + bSize.height))) {
					if ((xb > pos.x && (xb + size.width >= bPos->x && xb + size.width <= bPos->x + bSize.width)) || (xb < pos.x && (xb <= bPos->x + bSize.width && xb >= bPos->x))) {
						_SetBrickID(BrickID{ index_i, index_j });
						brickCollision = true;
						xVelocity *= -1;
						return;
					}
				}

				index_j++;
			}
			index_i++;
		}
	}
}

void Ball::_UpdateLocation(std::vector<Raft*>* rafts, std::vector<std::vector<Brick*>*>* bricks) {
	_DetectCollision(rafts, bricks);
	_Move();
}

void Ball::_Move() {
	angle = ((xVelocity < 0 && angle > 0) || (xVelocity > 0 && angle < 0)) ? angle * -1 : angle;
	angle = (angle < 0 && angle < -5) ? -5 : angle;
	angle = (angle > 0 && angle > 5) ? 5 : angle;
	pos.x += (xVelocity + angle);
	pos.y += yVelocity;
}

void Ball::_ResetPosition() {
	angle = 0;
	pos.x = spawnPos.x;
	pos.y = spawnPos.y;
	xVelocity *= -1;
}