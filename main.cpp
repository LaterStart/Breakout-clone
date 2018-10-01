#include "Window.h"
#include "ScreenObject.h"
#include "Level.h"
#include "Player.h"
#include "Utility.h"
using namespace std;

void _UpdateScoreView(int playerScore, vector<Text*>* digits, vector<const char*>* numbers) {
	size_t z{ digits->size() };
	while (playerScore > 0) {
		uint8_t n = playerScore % 10;
		playerScore /= 10;
		digits->at(--z)->_SetTexture(numbers->at(n));
	}
}

int main(int argc, char* argv[]) {
	// create window
	Window window{ "Breakout - by Mateo" };
		
	// load XML level configuration files
	vector<const char*>* numbers{ Level::_LoadXMLNumbers("Configuration/numbers.xml") };
	vector<Level*>* levels{ Level::_LoadXMLLevels("Configuration/levels.xml") };

	Player player(4);
	bool gameLost{};
	bool victory{};
	int lvlNum{};
	Text* gameOverTxt{};



	// iterate through levels
	for (auto &level : *levels) {
		// initialize level
		level->_LoadLevel(window);		
		vector<vector<Brick*>*>* brickContainer{ level->bricksContainer };
		vector<ScreenObject*>* objectContainer{ level->objectsContainer };		
		vector<Text*>* scoreDigits{ level->scoreDigits };

		// point out dynamic objects from screen objects container
		Ball* ball{};
		Text* livesNumber{};
		Text* clickStart{};
		
		vector<Raft*>* raftContainer{ new vector<Raft*>() };		
		for (auto &object : *objectContainer) {
			if (object->_Type() == ObjectType::raft)
				raftContainer->push_back(dynamic_cast<Raft*>(object));
			else if (object->_Type() == ObjectType::ball)
				ball = dynamic_cast<Ball*>(object);
			else if (object->_Type() == ObjectType::text) {
				if (strcmp(dynamic_cast<Text*>(object)->ID, "liveNum") == 0) 
					livesNumber = dynamic_cast<Text*>(object);	
				else if (strcmp(dynamic_cast<Text*>(object)->ID, "gameOver") == 0)
					gameOverTxt = dynamic_cast<Text*>(object);
				else if (strcmp(dynamic_cast<Text*>(object)->ID, "clickStart") == 0)
					clickStart = dynamic_cast<Text*>(object);
			}
		}
		livesNumber->_SetTexture(numbers->at(player._Lives()));	
		for (auto &digit : *scoreDigits) 
			digit->_SetTexture(numbers->at(0));

		// set ball movement speed
		ball->_SetSpeed(level->_BallSpeed());

		// configure bricks drop down time	
		uint16_t levelDropTime{ level->_LevelDropTime() };
		uint32_t dropTime{ SDL_GetTicks() };
		
		// time render comparision variables
		uint32_t lastTime{}, currentTime{};

		// ask user to confirm level start
		window._ResetClick();
		window._RenderObjects(objectContainer);
		clickStart->_Draw();
		window._Clear();
		clickStart->_SetVisible(false);
		while (!window._Click() && window._IsOpen()) 
			window._PollEvents();

		if (!window._IsOpen())
			gameLost = true;

		// game loop
		while (window._IsOpen()) {
			currentTime = SDL_GetTicks();	

			if (currentTime > lastTime + ball->_Speed()) {		
				// update ball location while checking for collisions
				ball->_UpdateLocation(raftContainer, brickContainer);
				
				if (ball->_BrickCollision()) {
					// if collision occured
					BrickID b{ ball->_BrickID() };
					Brick* brick{ brickContainer->at(b.rowID)->at(b.columnID) };
					brick->_Hit();						

					if (brick->_Destroyed()) {	
						// if brick is destroyed
						player._AddScore(brick->_BreakScore());
						_UpdateScoreView(player._Score(), scoreDigits, numbers);
						level->_DescreaseBrickCount();						

						// remove brick from bricks container
						brickContainer->at(b.rowID)->erase(brickContainer->at(b.rowID)->begin() + b.columnID);
						delete brick;
						if (brickContainer->at(b.rowID)->size() == 0) {
							vector<Brick*>* brickRow{ brickContainer->at(b.rowID) };
							brickContainer->erase(brickContainer->begin() + b.rowID);
							delete brickRow;
						}						

						// check if it was last brick
						if (level->_Finished()) {
							lvlNum++;
							if (lvlNum == levels->size())
								victory = true;
							_Pause(2);
							// switch to next level
							break;
						}
					}
				}
				else if (ball->_LifeLost()) {
					// if player lost life
					if (player._Lives() == 0) {
						gameLost = true;
						gameOverTxt->_SetVisible(true);
						window._RenderObjects(objectContainer);
						window._RenderObjects((vector<ScreenObject*>*)scoreDigits);
						window._Clear();
						_Pause(5);
						break;
					}
					else {
						player._TakeLife();
						_Pause(2);
						livesNumber->_SetTexture(numbers->at(player._Lives()));
						ball->_ResetPosition();
					}
				}

				lastTime = currentTime;

				// render screen objects
				window._RenderObjects(objectContainer);

				// render bricks row by row
				for (auto &brickRow : *brickContainer)
					window._RenderObjects((vector<ScreenObject*>*)brickRow);

				// render player score digits	
				window._RenderObjects((vector<ScreenObject*>*)scoreDigits);

				// refresh screen
				window._Clear();

				// check for user interaction
				window._PollEvents(objectContainer);
				if (!window._IsOpen())
					gameLost = true;

				// check if bricks are about to be moved one step down
				if (currentTime > dropTime + levelDropTime) {
					dropTime = currentTime;
					level->_DropBricks();
				}
			}
		}
		if (gameLost)
			break;
	}
	if (victory) {
		gameOverTxt->_SetVisible(true);
		gameOverTxt->_Draw();
		window._Clear();
		_Pause(5);
	}

	// free resources
	for (size_t i{}; i < numbers->size(); i++) {
		delete[] numbers->at(i);
	}
	delete numbers;

	for (size_t i{}; i < levels->size(); i++) {
		delete levels->at(i);
	}
	delete levels;

	return 0;
}