#pragma once

class Player {
public:
	Player(int lifeNum);

public:
	int _Score() const;
	int _Lives() const;
	void _AddLife();
	void _TakeLife();
	void _AddScore(int score);

private:
	int score = 0;
	int lives = 3;
	int maxLevel = 0;
};

inline int Player::_Score() const {
	return this->score;
}

inline int Player::_Lives() const {
	return this->lives;
}

inline void Player::_AddLife() {
	this->lives++;
}

inline void Player::_TakeLife() {
	this->lives--;
}

inline void Player::_AddScore(int score) {
	this->score += score;
}