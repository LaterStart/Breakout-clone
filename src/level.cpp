#include <iostream>
#include "Level.h"
#include "Brick.h"
#include "Window.h"
#include "tinyxml2.h"
#include "Utility.h"
using namespace tinyxml2;
using namespace std;

// load level configuration XML
vector<Level*>* Level::_LoadXMLLevels(const char* configurationPath) {		
	XMLDocument doc;
	doc.LoadFile(configurationPath);
	vector<Level*>* levels{};

	if (doc.ErrorID() == 0) {
		XMLElement* root { doc.RootElement() };

		if (root) {
			levels = new vector<Level*>();
			XMLElement* levelXml{ root->FirstChildElement() };

			while (levelXml != nullptr) {		
				Level* level{ new Level };
				level->brickTypes = new vector<BrickConfig>();

				XMLElement* element{ levelXml->FirstChildElement("RowCount") };
				level->rowCount = atoi(element->GetText());
				element = levelXml->FirstChildElement("ColumnCount");
				level->columnCount = atoi(element->GetText());
				element = levelXml->FirstChildElement("RowSpacing");
				level->rowSpacing = atoi(element->GetText());
				element = levelXml->FirstChildElement("ColumnSpacing");
				level->columnSpacing = atoi(element->GetText());
				element = levelXml->FirstChildElement("TitleTexture");
				level->titleTexture = _CopyChar(element->GetText());
				element = levelXml->FirstChildElement("BackgroundTexture");
				level->backgroundTexture = _CopyChar(element->GetText());
				element = levelXml->FirstChildElement("LeftFrameTexture");
				level->leftFrameTexture = _CopyChar(element->GetText());
				element = levelXml->FirstChildElement("RightFrameTexture");
				level->rightFrameTexture = _CopyChar(element->GetText());
				element = levelXml->FirstChildElement("TopFrameTexture");
				level->topFrameTexture = _CopyChar(element->GetText());
				element = levelXml->FirstChildElement("RaftTexture");
				level->raftTexture = _CopyChar(element->GetText());
				element = levelXml->FirstChildElement("DropdownTime");
				level->msLevelDrop = atoi(element->GetText()) * 1000;
				element = levelXml->FirstChildElement("BallSpeed");
				level->ballSpeed = atoi(element->GetText());	
				element = levelXml->FirstChildElement("RaftMode");
				level->raftMode = _CopyChar(element->GetText());

				XMLElement* brickTypeXML{ levelXml->FirstChildElement("BrickTypes")->FirstChildElement("BrickType") };

				while (brickTypeXML != nullptr) {
					BrickConfig brickCfg;
					element = brickTypeXML->FirstChildElement("Id");
					brickCfg.id = element->GetText()[0];
					element = brickTypeXML->FirstChildElement("Texture");
					brickCfg.texturePath = _CopyChar(element->GetText());
					element = brickTypeXML->FirstChildElement("HitPoints");
					brickCfg.hitPoints = atoi(element->GetText());
					element = brickTypeXML->FirstChildElement("HitSound");
					brickCfg.hitSoundPath = _CopyChar(element->GetText());
					element = brickTypeXML->FirstChildElement("BreakSound");
					if (element) 
						brickCfg.breakSoundPath = _CopyChar(element->GetText());
					element = brickTypeXML->FirstChildElement("BreakScore");
					if(element)
						brickCfg.breakScore = atoi(element->GetText());	

					level->brickTypes->push_back(brickCfg);
					brickTypeXML = brickTypeXML->NextSiblingElement();
				}

				XMLElement* bricksXML{ levelXml->FirstChildElement("Bricks") };
				const char* buffer{ bricksXML->GetText() };
				level->bricksConfiguration = new char*[level->rowCount];
				int z{};
				for (int i{}; i < level->rowCount; i++) {
					level->bricksConfiguration[i] = new char[level->columnCount];
					for (int j{}; j < level->columnCount; j++) {
						if ((buffer[z] >= 65 && buffer[z] <= 90) || buffer[z] == 95) 
							level->bricksConfiguration[i][j] = buffer[z++];		
						else {
							z++;
							j--;
						}
					}
				}

				levels->push_back(level);
				levelXml = levelXml->NextSiblingElement();
			}
		}
	}
	else cerr << "Failed to load level configuration\n";
	return levels;
}

vector<const char*>* Level::_LoadXMLNumbers(const char* configurationPath) {
	XMLDocument doc;
	doc.LoadFile(configurationPath);
	vector<const char*>* numbers{};

	if (doc.ErrorID() == 0) {
		XMLElement* root{ doc.RootElement() };

		if (root) {
			numbers = new vector<const char*>(10);
			XMLElement* numberXml{ root->FirstChildElement() };

			while (numberXml != nullptr) {
				XMLElement* element{ numberXml->FirstChildElement("Value") };
				int index = atoi(element->GetText());
				element = numberXml->FirstChildElement("Texture");
				const char* path = element->GetText();

				numbers->at(index) = _CopyChar(path);
				numberXml = numberXml->NextSiblingElement();
			}
		}
	}
	return numbers;
}

void Level::_LoadLevel(Window& window) {
	XY position;
	uint16_t w, h, tmp;
	Size ws{ window._Size() };

	// configure left frame
	w = _CalculatePercent(2, ws.width) + 1;
	h = _CalculatePercent(70, ws.height);
	position.x = _CalculatePercent(25, ws.width);
	position.y = _CalculatePercent(13, ws.height);
	Frame* leftFrame{ new Frame{ Size{ w, h }, position} };
	uint16_t minX(position.x + w);

	// configure right frame
	position.x = ws.width - position.x - w;
	Frame* rightFrame{ new Frame{ Size{ w, h }, position} };
	uint16_t maxX{ position.x };
	uint16_t maxY(position.y + h);

	// configure rafts
	if (strcmp("single", raftMode) == 0)
		raftNumber = 1;
	else raftNumber = 2;
	Raft** rafts{ new Raft*[raftNumber] };
	tmp = h;
	w = _CalculatePercent(5, ws.width);
	h = _CalculatePercent(2, ws.height);
	position.x = _CalculatePercent(47.5, ws.width);
	position.y = tmp + position.y - h;
	for (int i{ 0 }; i < raftNumber; i++) {
		rafts[i] = new Raft{ Size{ w, h }, position };
		rafts[i]->_SetRange(minX, maxX - w);
		position.y -= _CalculatePercent(8, ws.height);
	}

	// configure top frame
	w = maxX - minX;
	h = _CalculatePercent(3, ws.height);
	position.x = minX;
	position.y = _CalculatePercent(12, ws.height);
	Frame* topFrame{ new Frame{ Size{ w, h }, position} };
	uint16_t minY(position.y + h);

	// configure ball
	w = _CalculatePercent(0.5, ws.width);
	h = _CalculatePercent(0.5, ws.height);
	position.x = _CalculatePercent(50, ws.width);
	position.y = _CalculatePercent(50, ws.height);
	Ball* ball{ new Ball{ Size{ w, h }, position, RGB{ 0, 255, 222, 255 } } };
	ball->_SetRange(minX, maxX - w, minY, maxY);

	// configure background
	Background* background{ new Background{ window._Size() } };

	// configure level title
	w = _CalculatePercent(6.5, ws.width);
	h = _CalculatePercent(5, ws.height);
	position.x = _CalculatePercent(26.1, ws.width);
	position.y = _CalculatePercent(4, ws.width);
	Text* levelTitle{ new Text{ Size{ w, h }, position} };
	levelTitle->ID = "levelTitle";

	// configure lives number
	w = _CalculatePercent(1, ws.width);
	h = _CalculatePercent(3.5, ws.height);
	position.x = _CalculatePercent(26.1, ws.width);
	position.y = _CalculatePercent(83.5, ws.height);
	Text* livesNumber{ new Text{ Size{ w, h }, position } };
	livesNumber->ID = "liveNum";

	// configure score digits
	scoreDigits = new vector<Text*>(5);
	uint16_t z{};
	uint16_t xp{ _CalculatePercent(69, ws.width) };
	for (auto &digit : *scoreDigits) {
		w = _CalculatePercent(1, ws.width);
		h = _CalculatePercent(3.5, ws.height);		
		position.x = xp + z;
		position.y = _CalculatePercent(9, ws.height);
		digit =  new Text{ Size{ w, h }, position };
		digit->ID = "scoreDigit";
		digit->_SetRenderer(window._Renderer());
		z += w;
	}

	// configure game over label
	w = _CalculatePercent(20, ws.width);
	h = _CalculatePercent(10, ws.height);
	position.x = _CalculatePercent(40, ws.width);
	position.y = _CalculatePercent(40, ws.height);
	Text* gameOver{ new Text{ Size{ w, h }, position } };
	gameOver->_SetVisible(false);
	gameOver->ID = "gameOver";	

	// configure click to start label
	w = _CalculatePercent(20, ws.width);
	h = _CalculatePercent(10, ws.height);
	position.x = _CalculatePercent(40, ws.width);
	position.y = _CalculatePercent(40, ws.height);
	Text* clickStart{ new Text{ Size{ w, h }, position } };
	clickStart->ID = "clickStart";

	// initialize container
	objectsContainer = new vector<ScreenObject*>{
	   background , leftFrame, rightFrame, topFrame, levelTitle, livesNumber, ball, gameOver, clickStart
	};
	for (int i{}; i < raftNumber; i++)
		objectsContainer->push_back(rafts[i]);

	// set renderers
	for (size_t i{}; i < objectsContainer->size(); i++)
		objectsContainer->at(i)->_SetRenderer(window._Renderer());

	// set textures
	background->_SetTexture(backgroundTexture);
	leftFrame->_SetTexture(leftFrameTexture);
	rightFrame->_SetTexture(rightFrameTexture);
	topFrame->_SetTexture(topFrameTexture);	
	levelTitle->_SetTexture(titleTexture);
	gameOver->_SetTexture("Textures/game_over.png");
	clickStart->_SetTexture("Textures/click_to_start.png");
	for (int i{}; i < raftNumber; i++)
		rafts[i]->_SetTexture(raftTexture);

	// spawn bricks
	uint16_t width (maxX - minX);
	uint16_t spaceX{ _CalculatePercent(0.4 * columnSpacing, width) };
	uint16_t spaceY{ _CalculatePercent(0.4 * rowSpacing, ws.height) };

	uint16_t brickWidth{ _CalculatePercent((100 / columnCount), width) };
	uint16_t brickHeight{ _CalculatePercent(1.3, ws.height) };	
	brickWidth -= spaceX;		
	uint16_t frameSpaceX ((width - ((columnCount*brickWidth) + ((columnCount-1) *spaceX)))/2);
	if (frameSpaceX > spaceX) {		
		uint16_t xPlus(width - ((brickWidth*columnCount) + (spaceX*(columnCount-1))));	
		brickWidth += xPlus/columnCount;
		frameSpaceX -= xPlus / 2;
		frameSpaceX = (frameSpaceX < 2) ? 2 : frameSpaceX;
	}
	Size brickSize{ brickWidth, brickHeight };

	uint8_t index_i{}, index_j{};
	bricksContainer = new vector<vector<Brick*>*>(rowCount);
	for (auto &row : *bricksContainer) {
		row = new vector<Brick*>(columnCount);
		index_j = 0;
		for (auto &brick : *row) {			
			BrickConfig* cfg{};
			for (auto &type : *brickTypes) {
				if (type.id == bricksConfiguration[index_i][index_j]) {
					cfg = &type;
					break;
				}
				cfg = nullptr;
			}
			brick = Brick::_SpawnBrick(cfg, window._Renderer());
			brick->_SetSize(brickSize);
			brick->_Location().x = minX + frameSpaceX + ((brickSize.width + spaceX) * index_j++);
			brick->_Location().y = minY + spaceY + ((brickSize.height + spaceY)* index_i);			
			if (!brick->_Infinite() && !brick->_Destroyed())
				activeBrickCount++;
		}
		index_i++;
	}

	// remove invalid bricks from container
	for (size_t i{}; i < bricksContainer->size(); i++) {
		for (size_t j{}; j < bricksContainer->at(i)->size(); j++) {
			Brick* brick{ bricksContainer->at(i)->at(j) };
			if (brick->_Destroyed()) {
				bricksContainer->at(i)->erase(bricksContainer->at(i)->begin() + j);			
				delete brick;
				j--;
				if (bricksContainer->at(i)->size() == 0) {
					vector<Brick*>* brickRow{ bricksContainer->at(i) };
					bricksContainer->erase(bricksContainer->begin() + i);
					delete brickRow;
					i--;
				}
			}
		}
	}
}

void Level::_DropBricks() {	
	for (auto &brickRow : *bricksContainer) {
		for (auto &brick : *brickRow) {
			uint16_t brickHeight{ brick->_Size().height };
			uint16_t y{ brick->_Location().y };
			brick->_Location().y = y + brickHeight;
		}
	}
}

Level::~Level() {
	if (bricksContainer) {
		for (size_t i{}; i < bricksContainer->size(); i++) {
			for (size_t j{}; j < bricksContainer->at(i)->size(); j++) {
				delete bricksContainer->at(i)->at(j);
			}
			delete bricksContainer->at(i);
		}
		delete bricksContainer;
	}
	
	if (objectsContainer) {
		for (size_t i{}; i < objectsContainer->size(); i++) {
			delete objectsContainer->at(i);
		}
		delete objectsContainer;
	}

	if (scoreDigits) {
		for (size_t i{}; i < scoreDigits->size(); i++) {
			delete scoreDigits->at(i);
		}
		delete scoreDigits;
	}

	delete brickTypes;

	for (int i = 0; i < rowCount; i++) {
		delete[]bricksConfiguration[i];
	}
	delete[]bricksConfiguration;

	delete[] backgroundTexture;
	delete[] titleTexture;
	delete[] leftFrameTexture;
	delete[] rightFrameTexture;
	delete[] topFrameTexture;
	delete[] raftTexture;
	delete[] raftMode;
}
