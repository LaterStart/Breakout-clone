#pragma once
#include <vector>

class BrickConfig;
class ScreenObject;
class Window;
class Brick;
class Text;
class Level {
public:
	~Level();

public:
	static std::vector<Level*>* _LoadXMLLevels(const char* configurationPath);
	static std::vector<const char*>* _LoadXMLNumbers(const char* configurationPath);
	void _LoadLevel(Window& window);
	void _DropBricks();
	void _DescreaseBrickCount();
	bool _Finished() const;	
	uint8_t _BallSpeed() const;
	uint8_t _RaftNumber() const;
	uint16_t _LevelDropTime() const;

public:
	std::vector<std::vector<Brick*>*>* bricksContainer{};
	std::vector<ScreenObject*>* objectsContainer{};
	std::vector<Text*>* scoreDigits{};

private:
	int rowCount;
	int columnCount;
	int rowSpacing;
	int columnSpacing;
	char** bricksConfiguration{};
	const char* backgroundTexture{};
	const char* titleTexture{};
	const char* leftFrameTexture{};
	const char* rightFrameTexture{};
	const char* topFrameTexture{};
	const char* raftTexture{};
	const char* raftMode{};
	std::vector<BrickConfig>* brickTypes{};
	uint16_t msLevelDrop;
	uint8_t ballSpeed;
	uint8_t raftNumber;
	int activeBrickCount{};
};

inline uint16_t Level::_LevelDropTime() const {
	return this->msLevelDrop;
}

inline void Level::_DescreaseBrickCount() {
	activeBrickCount--;
}

inline bool Level::_Finished() const {
	if (activeBrickCount > 0)
		return false;
	else return true;
}

inline uint8_t Level::_BallSpeed() const {
	return this->ballSpeed;
}

inline uint8_t Level::_RaftNumber() const {
	return this->raftNumber;
}
