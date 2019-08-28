#pragma once
#include "Entity.h"
#include "Cell.h"
#include "SFML/Audio.hpp"

class Player;
class Brick;
class Ball;
class Button;
class ChatBox;
class PickupItem;

class BrickPongApp
{
public:

	BrickPongApp();

	void run();

	~BrickPongApp();
private:

	void loadTextures();
	void loadSoundBuffers();
	void loadFonts();

	enum class State
	{
		Menu, Game, Edit, Pause, Dead, Level, LevelNaming, Win
	};

	State fCurrentState;
	void update(sf::Time dt);
	void handleEvent();

	void handleGameEvent(const sf::Event& e);
	void handleEditEvent(const sf::Event& e);
	void handleMenuEvent(const sf::Event& e);
	void handlePauseEvent(const sf::Event& e);
	void handleDeadEvent(const sf::Event& e);
	void handleLevelEvent(const sf::Event& e);
	void handleLevelNamingEvent(const sf::Event& e);
	void handleWinStateEvent(const sf::Event& e);

	void updateGameState(sf::Time dt);
	void updateMenuState(sf::Time dt);
	void updateEditState(sf::Time dt);
	void updateDeadState(sf::Time dt);
	void updatePauseState(sf::Time dt);
	void updateLevelState(sf::Time dt);
	void updateLevelNaming(sf::Time dt);
	void updateWinState(sf::Time dt);

	sf::Text fWinText;

	void render();

	void checkCollisions();
	void checkBallPlayerCollision();
	void checkBallBricksCollision();
	void playerLose(Ball& bll);

	sf::Vector2i getGridPosition(const Cell& cell) const;
	sf::Vector2i getGridCellPosition(const sf::Vector2f& pos) const;

	enum class InWorldEntity
	{
		player, ball, count
	};

	sf::RenderWindow fWindow;
	sf::Time fFrameRate;
	bool fFocus = true;

	sf::Music fMusic;

	TexturesHolder fTextures;
	FontsHolder fFonts;
	SoundBuffersHolder fSoundBuffers;

	std::vector<Ball*> fBalls;
	std::vector<Cell*> fGrid;
	std::vector<Brick*> fBricks;
	sf::Vector2i gridSize;
	sf::Vector2i cellSize;
	std::vector<Side*> fBrickCollisions;
	std::vector<Cell*> fCellDrawer;

	std::vector<Button*> fGeneralButtons;
	std::vector<Button*> fMenuStateButtons;
	std::vector<Button*> fEditStateButtons;
	std::vector<Button*> fGameStateButtons;
	std::vector<Button*> fPauseStateButtons;
	std::vector<Button*> fDeadStateButtons;
	std::vector<Button*> fLevelStateButtons;
	std::vector<Button*> fWinStateButtons;

	sf::Sprite fPlayerLifes;
	std::vector<PickupItem*> fItems;

	sf::Text fMusicCheckMark;
	sf::Text fSoundEffectCheckMark;

	/*level state stuff*/
	std::vector<Button*> fLevelsButtons;
	void loadLevel(unsigned int level);
	void deleteBricks();
	void deleteEntities();

	static const int fNameLevelCharLimit = 10;
	void loadLevelNames();
	std::vector<std::string> fLevelNames;
	ChatBox* fLevelNamingChatBox;

	sf::Time fDoubleClickDelay = sf::seconds(0.5f);
	sf::Time fDoubleClickAccu = sf::Time::Zero;
	bool fStartDoubleClickTimer = false;

	void deleteLevel(int level);
	int fCurrentLevel = -1;
	/******************/

	/*Edit mode stuff*/
	enum PlacementType
	{
		None, brick = 2, fball, fspeedBoost, fheart, fchangingpaddle
	};
	PlacementType fCurrentPlacingType;

	void deleteCells();
	void createCells();
	void saveLevel();

	sf::VertexArray fVisualGrid;
	void createVisualGrid();
	/****************/

	//stats stuff
	sf::Text StatsText;
	sf::Time fElapsedStatsTime;
	unsigned int NumFrames;
	void updateStats(sf::Time dt);

	Player* fPlayer;

	sf::Sprite fBackGround;
};