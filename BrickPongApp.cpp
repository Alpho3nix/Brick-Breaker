#include "BrickPongApp.h"
#include "Player.h"
#include "Ball.h"
#include "UtilityFunc.h"
#include "Brick.h"
#include "PickupItem.h"
#include "ChatBox.h"
#include <sstream>
#include <fstream>

//ATTENTION
//ATTENTION
//ATTENTION
//ATTENTION
//ATTENTION
//ATTENTION
//ATTENTION
/*********!!! YOU SHOULD NOT USE THE getPosition() METHOD FOR BRICKS BUT RATHER THE myGetPosition() METHOD, SAME FOR setPosition() AND mySetPosition() !!!********/
/*********!!! YOU SHOULD NOT USE THE getPosition() METHOD FOR BRICKS BUT RATHER THE myGetPosition() METHOD, SAME FOR setPosition() AND mySetPosition() !!!********/
/*********!!! YOU SHOULD NOT USE THE getPosition() METHOD FOR BRICKS BUT RATHER THE myGetPosition() METHOD, SAME FOR setPosition() AND mySetPosition() !!!********/
/*********!!! YOU SHOULD NOT USE THE getPosition() METHOD FOR BRICKS BUT RATHER THE myGetPosition() METHOD, SAME FOR setPosition() AND mySetPosition() !!!********/
/*********!!! YOU SHOULD NOT USE THE getPosition() METHOD FOR BRICKS BUT RATHER THE myGetPosition() METHOD, SAME FOR setPosition() AND mySetPosition() !!!********/
/*********!!! YOU SHOULD NOT USE THE getPosition() METHOD FOR BRICKS BUT RATHER THE myGetPosition() METHOD, SAME FOR setPosition() AND mySetPosition() !!!********/
//ATTENTION
//ATTENTION
//ATTENTION
//ATTENTION
//ATTENTION
//ATTENTION

const unsigned int WINDOW_WIDTH = WIDTH + 200;
const unsigned int WINDOW_HEIGHT = HEIGHT;

BrickPongApp::BrickPongApp()
	:fWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Brick Pong !", sf::Style::Close), fFrameRate(sf::seconds(1.f / 60.f)), fTextures(), fGrid(), fBrickCollisions(), fBackGround(), fCurrentState(State::Menu)
	, fFonts(), fSoundBuffers(), fGameStateButtons(), fEditStateButtons(), fPauseStateButtons(), fMenuStateButtons(), fDeadStateButtons(), NumFrames(0), fElapsedStatsTime(sf::Time::Zero), fCurrentPlacingType(None), fVisualGrid()
	, fItems()
{
	Player::Type playerType = Player::Green;

	if (playerType >= Player::Blue) {
		cellSize = sf::Vector2i(43, 128);	//size of each brick when rotated by 90 degrees
	}
	else
	{
		cellSize = sf::Vector2i(128, 43);	//size of each brick
	}

	loadTextures();
	loadSoundBuffers();
	loadFonts();

	StatsText.setFont(fFonts.get(Fonts::ID::Future));
	StatsText.setCharacterSize(10U);

	fPlayer = new Player(fWindow, fTextures, playerType, 260.f);

	/*building menu*/
	Button* playButton = new Button(fTextures, fSoundBuffers, fFonts, Button::Type::Empty, Button::Color::Yellow, "PLAY");
	playButton->fCallbackFunc = [this]()
	{
		fCurrentState = State::Game;
		fPlayer->addLife();
		fPlayer->addLife();
		fPlayer->addLife();
		fPlayer->setDead(false);
	};
	playButton->setPosition(WINDOW_WIDTH / 2.f - playButton->getBoundingRect().width / 2.f, WINDOW_HEIGHT / 2.f - playButton->getBoundingRect().height);
	fMenuStateButtons.push_back(playButton);

	Button* enterLevelStateButton = new Button(fTextures, fSoundBuffers, fFonts, Button::Type::Empty, Button::Color::Blue, "LEVELS");
	enterLevelStateButton->fCallbackFunc = [this]()
	{
		fCurrentState = State::Level;
	};
	enterLevelStateButton->setPosition(playButton->getPosition().x, playButton->getPosition().y + playButton->getBoundingRect().height + 10.f);
	fMenuStateButtons.push_back(enterLevelStateButton);

	Button* quitButton = new Button(fTextures, fSoundBuffers, fFonts, Button::Type::Empty, Button::Color::Orange, "QUIT");
	quitButton->fCallbackFunc = [this]()
	{
		fWindow.close();
	};
	quitButton->setPosition(enterLevelStateButton->getPosition().x, enterLevelStateButton->getPosition().y + enterLevelStateButton->getBoundingRect().height + 10.f);
	fMenuStateButtons.push_back(quitButton);

	/**************/

	/*Pause buttons*/
	Button* resumeButton = new Button(fTextures, fSoundBuffers, fFonts, Button::Type::Empty, Button::Color::Blue, "RESUME");
	resumeButton->fCallbackFunc = [this]()
	{
		fCurrentState = State::Game;
	};
	resumeButton->setPosition(playButton->getPosition());
	fPauseStateButtons.push_back(resumeButton);

	Button* restartPauseButton = new Button(fTextures, fSoundBuffers, fFonts, Button::Type::Empty, Button::Color::Blue, "RESTART");
	restartPauseButton->fCallbackFunc = [this]()
	{
		loadLevel(fCurrentLevel);
		fCurrentState = State::Game;
		fPlayer->setDead(false);
		fPlayer->addLife();	fPlayer->addLife();	fPlayer->addLife();
		for (Ball* b : fBalls)
		{
			b->setSpeed(250.f);
		}
	};
	restartPauseButton->setPosition(resumeButton->getPosition().x, resumeButton->getPosition().y + resumeButton->getBoundingRect().height + 10.f);
	restartPauseButton->setColor(sf::Color::Color(255, 255, 255, 125));
	fPauseStateButtons.push_back(restartPauseButton);

	Button* backToMenuButton0 = new Button(fTextures, fSoundBuffers, fFonts, Button::Type::Empty, Button::Color::Green, "MENU");
	backToMenuButton0->fCallbackFunc = [this]()
	{
		fCurrentState = State::Menu;
	};
	backToMenuButton0->setPosition(quitButton->getPosition());
	fPauseStateButtons.push_back(backToMenuButton0);
	/***************/

	/*Dead buttons*/
	Button* restartButton = new Button(fTextures, fSoundBuffers, fFonts, Button::Type::Empty, Button::Color::Blue, "RESTART");
	restartButton->fCallbackFunc = [this]()
	{
		loadLevel(fCurrentLevel);
		fCurrentState = State::Game;
		fPlayer->setDead(false);
		fPlayer->addLife();	fPlayer->addLife();	fPlayer->addLife();
		for (Ball* bll : fBalls)
		{
			bll->setSpeed(250.f);
		}
	};
	restartButton->setPosition(resumeButton->getPosition());
	restartButton->setColor(sf::Color::Color(255, 255, 255, 125));
	fDeadStateButtons.push_back(restartButton);

	Button* backToMenuButton01 = new Button(fTextures, fSoundBuffers, fFonts, Button::Type::Empty, Button::Color::Green, "MENU");
	backToMenuButton01->fCallbackFunc = [this]()
	{
		fCurrentState = State::Menu;
	};
	backToMenuButton01->setPosition(restartPauseButton->getPosition());
	fDeadStateButtons.push_back(backToMenuButton01);
	/**************/

	if (!fMusic.openFromFile("Music/music.ogg"))
		throw std::runtime_error("can't open file Music/music.ogg");
	fMusic.setLoop(true);
	fMusic.play();

	fMusicCheckMark.setFont(fFonts.get(Fonts::ID::Future_Thin));
	fMusicCheckMark.setString("M");
	fMusicCheckMark.setCharacterSize(30U);
	fMusicCheckMark.setPosition(5.f, WINDOW_HEIGHT - fMusicCheckMark.getGlobalBounds().height - 20.f);

	/*general buttons*/
	Button* checkMusic = new Button(fTextures, fSoundBuffers, fFonts, Button::Type::CheckMark);
	checkMusic->fCallbackFunc = [this]()
	{
		fEnableMusic = !fEnableMusic;
	};
	checkMusic->setPosition(fMusicCheckMark.getPosition().x + fMusicCheckMark.getGlobalBounds().width + 5.f, fMusicCheckMark.getPosition().y);
	fGeneralButtons.push_back(checkMusic);

	fSoundEffectCheckMark.setFont(fFonts.get(Fonts::ID::Future_Thin));
	fSoundEffectCheckMark.setString("SE");
	fSoundEffectCheckMark.setCharacterSize(30U);
	fSoundEffectCheckMark.setPosition(checkMusic->getPosition().x + checkMusic->getBoundingRect().width + 5.f, fMusicCheckMark.getPosition().y);

	Button* checkSoundEffects = new Button(fTextures, fSoundBuffers, fFonts, Button::Type::CheckMark);
	checkSoundEffects->fCallbackFunc = [this, checkSoundEffects]()
	{
		fEnableSoundEffects = !fEnableSoundEffects;
		if (!fEnableSoundEffects)
		{
			checkSoundEffects->setVolume(0);
		}
		else
		{
			checkSoundEffects->setVolume(100);
		}
	};
	checkSoundEffects->setPosition(fSoundEffectCheckMark.getPosition().x + fSoundEffectCheckMark.getGlobalBounds().width + 5.f, fMusicCheckMark.getPosition().y);
	fGeneralButtons.push_back(checkSoundEffects);
	/****************/

	/*LevelState buttons*/
	Button* backToMenuButtonLevel = new Button(fTextures, fSoundBuffers, fFonts, Button::Type::Empty, Button::Color::Green, "MENU");
	backToMenuButtonLevel->fCallbackFunc = [this]()
	{
		fCurrentState = State::Menu;
	};
	backToMenuButtonLevel->setPosition(5.f, 5.f);
	fLevelStateButtons.push_back(backToMenuButtonLevel);

	loadLevelNames();

	for (unsigned int i = 0; i < fLevelNames.size(); ++i) {
		Button* levelButton = new Button(fTextures, fSoundBuffers, fFonts, Button::Type::Empty, Button::Color::Blue, fLevelNames[i], 20);
		levelButton->fCallbackFunc = [this, i]()
		{
			loadLevel(i);
		};
		levelButton->setPosition(WINDOW_WIDTH / 4.f + levelButton->getBoundingRect().width * int(i % 5) + 10.f * int(i % 5),
			50.f + int(i / 5) * levelButton->getBoundingRect().height + int(i / 5) * 10.f);
		fLevelsButtons.push_back(levelButton);

	}
	/*******************/

	//game buttons
	Button* backToMenuButton = new Button(fTextures, fSoundBuffers, fFonts, Button::Type::Empty, Button::Color::Green, "MENU");
	backToMenuButton->fCallbackFunc = [this]()
	{
		fCurrentState = State::Menu;
	};
	backToMenuButton->setPosition(WINDOW_WIDTH - backToMenuButton->getBoundingRect().width - 5.f, 5.f);
	fGameStateButtons.push_back(backToMenuButton);

	Button* enterEditButton = new Button(fTextures, fSoundBuffers, fFonts, Button::Type::Empty, Button::Color::Blue, "EDITMODE");
	enterEditButton->fCallbackFunc = [this]()
	{
		fCurrentState = State::Edit;
	};
	enterEditButton->setPosition(backToMenuButton->getPosition().x, backToMenuButton->getPosition().y + backToMenuButton->getBoundingRect().height + 10.f);
	fGameStateButtons.push_back(enterEditButton);

	////////////////

	fBackGround.setTexture(fTextures.get(Textures::ID::Background));
	fBackGround.setTextureRect(sf::IntRect(0, 0, WIDTH, HEIGHT));
	fBalls.push_back(new Ball(fWindow, fTextures, fSoundBuffers, *fPlayer, 250.f));
	
	gridSize = sf::Vector2i(WIDTH / cellSize.x, HEIGHT / cellSize.y);

	createCells();

	/*edit buttons*/
	Button* backToMenuButton1 = new Button(fTextures, fSoundBuffers, fFonts, Button::Type::Empty, Button::Color::Green, "MENU");
	backToMenuButton1->fCallbackFunc = [this]()
	{
		fCurrentState = State::Menu;
	};
	backToMenuButton1->setPosition(WINDOW_WIDTH - backToMenuButton1->getBoundingRect().width - 5.f, 5.f);
	fEditStateButtons.push_back(backToMenuButton1);

	Button* backToGameButton = new Button(fTextures, fSoundBuffers, fFonts, Button::Type::Empty, Button::Color::Blue, "GAMEMODE");
	backToGameButton->fCallbackFunc = [this]()
	{
		fCurrentState = State::Game;
	};
	backToGameButton->setPosition(enterEditButton->getPosition());
	fEditStateButtons.push_back(backToGameButton);

	Button* brickPlacingButton = new Button(fTextures, fSoundBuffers, fFonts, Button::Type::Brick);
	brickPlacingButton->fCallbackFunc = [this]()
	{
		if (fCurrentPlacingType == None)
		{
			fCurrentPlacingType = brick;
		}
		else
		{
			for (int i = 0; i < fEditStateButtons.size(); ++i)
			{
				if (i != brick)
				{
					fEditStateButtons[i]->unclickButton();
				}
			}
			fCurrentPlacingType = brick;
		}
	};
	brickPlacingButton->setPosition(backToGameButton->getBoundingRect().left, backToGameButton->getBoundingRect().top + backToGameButton->getBoundingRect().height + 10.f);
	fEditStateButtons.push_back(brickPlacingButton);

	Button* ballPlacingButton = new Button(fTextures, fSoundBuffers, fFonts, Button::Type::Ball);
	ballPlacingButton->fCallbackFunc = [this]()
	{
		if (fCurrentPlacingType == None)
		{
			fCurrentPlacingType = fball;
		}
		else
		{
			for (int i = 0; i < fEditStateButtons.size(); ++i)
			{
				if (i != fball)
				{
					fEditStateButtons[i]->unclickButton();
				}
			}
			fCurrentPlacingType = fball;
		}
	};
	ballPlacingButton->setPosition(backToGameButton->getBoundingRect().left, brickPlacingButton->getBoundingRect().top + brickPlacingButton->getBoundingRect().height + 10.f);
	fEditStateButtons.push_back(ballPlacingButton);

	Button* speedBoostButton = new Button(fTextures, fSoundBuffers, fFonts, Button::Type::SpeedBoost);
	speedBoostButton->fCallbackFunc = [this]()
	{
		if (fCurrentPlacingType == None)
		{
			fCurrentPlacingType = fspeedBoost;
		}
		else
		{
			for (int i = 0; i < fEditStateButtons.size(); ++i)
			{
				if (i != fspeedBoost)
				{
					fEditStateButtons[i]->unclickButton();
				}
			}
			fCurrentPlacingType = fspeedBoost;
		}
	};
	speedBoostButton->setPosition(ballPlacingButton->getPosition().x, ballPlacingButton->getPosition().y + ballPlacingButton->getBoundingRect().height + 10.f);
	fEditStateButtons.push_back(speedBoostButton);

	Button* upLifeButton = new Button(fTextures, fSoundBuffers, fFonts, Button::Type::Heart);
	upLifeButton->fCallbackFunc = [this]()
	{
		if (fCurrentPlacingType == None)
		{
			fCurrentPlacingType = fheart;
		}
		else
		{
			for (int i = 0; i < fEditStateButtons.size(); ++i)
			{
				if (i != fheart)
				{
					fEditStateButtons[i]->unclickButton();
				}
			}
			fCurrentPlacingType = fheart;
		}
	};
	upLifeButton->setPosition(speedBoostButton->getPosition().x, speedBoostButton->getPosition().y + speedBoostButton->getBoundingRect().height + 10.f);
	fEditStateButtons.push_back(upLifeButton);

	Button* changingPaddleButton = new Button(fTextures, fSoundBuffers, fFonts, Button::Type::Paddle);
	changingPaddleButton->fCallbackFunc = [this, changingPaddleButton]()
	{
		Player::Type newType = static_cast<Player::Type>((fPlayer->getType() + 1) % Player::Type::Count);

		if (newType >= Player::Blue && fPlayer->getType() < Player::Blue) {
			cellSize = sf::Vector2i(43, 128);	//size of each brick when rotated by 90 degrees
			gridSize = sf::Vector2i(WIDTH / cellSize.x, HEIGHT / cellSize.y);

			deleteCells();
			createCells();
			createVisualGrid();
		}
		else if(newType < Player::Blue && fPlayer->getType() >= Player::Blue)
		{
			cellSize = sf::Vector2i(128, 43);	//size of each brick
			gridSize = sf::Vector2i(WIDTH / cellSize.x, HEIGHT / cellSize.y);

			deleteCells();
			createCells();
			createVisualGrid();
		}

		fPlayer->setType(newType);
		changingPaddleButton->changePaddleButton(fPlayer->getType());
	};
	changingPaddleButton->setPosition(backToGameButton->getBoundingRect().left, upLifeButton->getBoundingRect().top + upLifeButton->getBoundingRect().height + 10.f);
	fEditStateButtons.push_back(changingPaddleButton);

	Button* saveLevelButton = new Button(fTextures, fSoundBuffers, fFonts, Button::Type::Empty, Button::Color::Yellow, "SAVELEVEL");
	saveLevelButton->fCallbackFunc = [this]()
	{
		fCurrentState = State::LevelNaming;
	};
	saveLevelButton->setPosition(changingPaddleButton->getPosition().x, WINDOW_HEIGHT - saveLevelButton->getBoundingRect().height - 5.f);
	fEditStateButtons.push_back(saveLevelButton);

	/*************/

	/*LevelNaming buttons*/
	fLevelNamingChatBox = new ChatBox(fWindow, fTextures, fSoundBuffers, fFonts);
	fLevelNamingChatBox->setPosition(WIDTH / 2.f, HEIGHT / 2.f);
	/*********************/

	/*Winstate buttons*/
	fWinText.setFont(fFonts.get(Fonts::ID::Future_Thin));
	fWinText.setCharacterSize(60U);
	fWinText.setString("YOU WON !");
	Utility::CenterOrigin(fWinText);
	fWinText.setPosition(WIDTH / 2.f, HEIGHT / 4.f);
	
	Button* nextLevelButton = new Button(fTextures, fSoundBuffers, fFonts, Button::Type::Empty, Button::Color::Blue, "NEXTLEVEL");
	nextLevelButton->fCallbackFunc = [this]()
	{
		if (fCurrentLevel != -1)
		{
			fCurrentLevel = (++fCurrentLevel) % fLevelNames.size();
			loadLevel(fCurrentLevel);
		}
	};
	nextLevelButton->setPosition(restartPauseButton->getPosition());
	nextLevelButton->setColor(sf::Color::Color(255, 255, 255, 125));
	fWinStateButtons.push_back(nextLevelButton);

	Button* quitToMenuButton = new Button(fTextures, fSoundBuffers, fFonts, Button::Type::Empty, Button::Color::Orange, "MENU");
	quitToMenuButton->fCallbackFunc = [this]()
	{
		fCurrentState = State::Menu;
	};
	quitToMenuButton->setPosition(nextLevelButton->getPosition().x, nextLevelButton->getPosition().y + nextLevelButton->getBoundingRect().height + 10.f);
	fWinStateButtons.push_back(quitToMenuButton);

	/*****************/

	fPlayerLifes.setTexture(fTextures.get(Textures::ID::PlayerLifes));
	fPlayerLifes.setTextureRect(sf::IntRect(1, 0, 69, 20));
	fPlayerLifes.setPosition(WIDTH / 2.f - fPlayerLifes.getGlobalBounds().width / 2.f, 0.f);

	fVisualGrid.setPrimitiveType(sf::PrimitiveType::Lines);
	createVisualGrid();
}

void BrickPongApp::run()
{
	sf::Clock clock;
	sf::Time deltaTime = sf::Time::Zero, accumulator = sf::Time::Zero;
	while (fWindow.isOpen())
	{
		deltaTime = clock.restart();
		accumulator += deltaTime;
		while (accumulator >= fFrameRate)
		{
			accumulator -= fFrameRate;

			handleEvent();
			if (fFocus) {
				update(fFrameRate);
			}
		}

		updateStats(deltaTime);
		render();
	}
}

void BrickPongApp::deleteCells()
{
	for (Cell* c : fGrid)
	{
		delete c->getBrick();
		delete c;
	}

	fGrid.clear();
	fBricks.clear();
}

void BrickPongApp::createCells()
{
	if (fGrid.size() != 0)
		return;

	fGrid.resize(gridSize.x * gridSize.y);
	//First we create the cells
	for (int y = 0; y < gridSize.y; ++y)
	{
		for (int x = 0; x < gridSize.x; ++x)
		{
			fGrid[x + y * gridSize.x] = new Cell(cellSize);
			fGrid[x + y * gridSize.x]->setPosition(x * cellSize.x, y * cellSize.y);
		}
	}

	//Then we assign each cell its neighbors
	for (size_t y = 0; y < gridSize.y; ++y)
	{
		for (size_t x = 0; x < gridSize.x; ++x)
		{
			if (x == 0 && y == 0)	//it's the top left corner
			{
				fGrid[x + y * gridSize.x]->fNeighbors[Cell::South] = fGrid[x + (y + 1) * gridSize.x];
				fGrid[x + y * gridSize.x]->fNeighbors[Cell::East] = fGrid[(x + 1) + y * gridSize.x];
				fGrid[x + y * gridSize.x]->fNeighbors[Cell::SouthEast] = fGrid[(x + 1) + (y + 1) * gridSize.x];
				fGrid[x + y * gridSize.x]->fType = Cell::Type::CornerTL;
			}
			else if (x == 0 && y == gridSize.y - 1)	//it's the bottom left corner
			{
				fGrid[x + y * gridSize.x]->fNeighbors[Cell::North] = fGrid[x + (y - 1) * gridSize.x];
				fGrid[x + y * gridSize.x]->fNeighbors[Cell::East] = fGrid[(x + 1) + y * gridSize.x];
				fGrid[x + y * gridSize.x]->fNeighbors[Cell::NorthEast] = fGrid[x + 1 + (y - 1) * gridSize.x];
				fGrid[x + y * gridSize.x]->fType = Cell::Type::CornerBL;
			}
			else if (x == gridSize.x - 1 && y == 0)	//it's the top right corner
			{
				fGrid[x + y * gridSize.x]->fNeighbors[Cell::South] = fGrid[x + (y + 1) * gridSize.x];
				fGrid[x + y * gridSize.x]->fNeighbors[Cell::West] = fGrid[(x - 1) + y * gridSize.x];
				fGrid[x + y * gridSize.x]->fNeighbors[Cell::SouthWest] = fGrid[(x - 1) + (y + 1) * gridSize.x];
				fGrid[x + y * gridSize.x]->fType = Cell::Type::CornerTR;
			}
			else if (x == gridSize.x - 1 && y == gridSize.y - 1)	//it's the bottom right corner
			{
				fGrid[x + y * gridSize.x]->fNeighbors[Cell::North] = fGrid[x + (y - 1) * gridSize.x];
				fGrid[x + y * gridSize.x]->fNeighbors[Cell::West] = fGrid[(x - 1) + y * gridSize.x];
				fGrid[x + y * gridSize.x]->fNeighbors[Cell::NorthWest] = fGrid[x - 1 + (y - 1) * gridSize.x];
				fGrid[x + y * gridSize.x]->fType = Cell::Type::CornerBR;
			}
			else if (x == 0)	//it's on the left hand size
			{
				fGrid[x + y * gridSize.x]->fNeighbors[Cell::North] = fGrid[x + (y - 1) * gridSize.x];
				fGrid[x + y * gridSize.x]->fNeighbors[Cell::South] = fGrid[x + (y + 1) * gridSize.x];
				fGrid[x + y * gridSize.x]->fNeighbors[Cell::East] = fGrid[(x + 1) + y * gridSize.x];
				fGrid[x + y * gridSize.x]->fNeighbors[Cell::NorthEast] = fGrid[x + 1 + (y - 1) * gridSize.x];
				fGrid[x + y * gridSize.x]->fNeighbors[Cell::SouthEast] = fGrid[(x + 1) + (y + 1) * gridSize.x];
				fGrid[x + y * gridSize.x]->fType = Cell::Type::BorderLeft;
			}
			else if (y == 0)	//it's on the top
			{
				fGrid[x + y * gridSize.x]->fNeighbors[Cell::South] = fGrid[x + (y + 1) * gridSize.x];
				fGrid[x + y * gridSize.x]->fNeighbors[Cell::East] = fGrid[(x + 1) + y * gridSize.x];
				fGrid[x + y * gridSize.x]->fNeighbors[Cell::West] = fGrid[(x - 1) + y * gridSize.x];
				fGrid[x + y * gridSize.x]->fNeighbors[Cell::SouthEast] = fGrid[(x + 1) + (y + 1) * gridSize.x];
				fGrid[x + y * gridSize.x]->fNeighbors[Cell::SouthWest] = fGrid[(x - 1) + (y + 1) * gridSize.x];
				fGrid[x + y * gridSize.x]->fType = Cell::Type::BorderTop;
			}
			else if (x == gridSize.x - 1)	//it's on the right hand side
			{
				fGrid[x + y * gridSize.x]->fNeighbors[Cell::North] = fGrid[x + (y - 1) * gridSize.x];
				fGrid[x + y * gridSize.x]->fNeighbors[Cell::South] = fGrid[x + (y + 1) * gridSize.x];
				fGrid[x + y * gridSize.x]->fNeighbors[Cell::West] = fGrid[(x - 1) + y * gridSize.x];
				fGrid[x + y * gridSize.x]->fNeighbors[Cell::NorthWest] = fGrid[x - 1 + (y - 1) * gridSize.x];
				fGrid[x + y * gridSize.x]->fNeighbors[Cell::SouthWest] = fGrid[(x - 1) + (y + 1) * gridSize.x];
				fGrid[x + y * gridSize.x]->fType = Cell::Type::BorderRight;
			}
			else if (y == gridSize.y - 1)	//it's on the bottom
			{
				fGrid[x + y * gridSize.x]->fNeighbors[Cell::North] = fGrid[x + (y - 1) * gridSize.x];
				fGrid[x + y * gridSize.x]->fNeighbors[Cell::East] = fGrid[(x + 1) + y * gridSize.x];
				fGrid[x + y * gridSize.x]->fNeighbors[Cell::West] = fGrid[(x - 1) + y * gridSize.x];
				fGrid[x + y * gridSize.x]->fNeighbors[Cell::NorthEast] = fGrid[x + 1 + (y - 1) * gridSize.x];
				fGrid[x + y * gridSize.x]->fNeighbors[Cell::NorthWest] = fGrid[x - 1 + (y - 1) * gridSize.x];
				fGrid[x + y * gridSize.x]->fType = Cell::Type::BorderBottom;
			}
			else	//it's somewhere in the middle
			{
				fGrid[x + y * gridSize.x]->fNeighbors[Cell::North] = fGrid[x + (y - 1) * gridSize.x];
				fGrid[x + y * gridSize.x]->fNeighbors[Cell::South] = fGrid[x + (y + 1) * gridSize.x];
				fGrid[x + y * gridSize.x]->fNeighbors[Cell::East] = fGrid[(x + 1) + y * gridSize.x];
				fGrid[x + y * gridSize.x]->fNeighbors[Cell::West] = fGrid[(x - 1) + y * gridSize.x];

				fGrid[x + y * gridSize.x]->fNeighbors[Cell::NorthEast] = fGrid[x + 1 + (y - 1) * gridSize.x];
				fGrid[x + y * gridSize.x]->fNeighbors[Cell::NorthWest] = fGrid[x - 1 + (y - 1) * gridSize.x];
				fGrid[x + y * gridSize.x]->fNeighbors[Cell::SouthEast] = fGrid[(x + 1) + (y + 1) * gridSize.x];
				fGrid[x + y * gridSize.x]->fNeighbors[Cell::SouthWest] = fGrid[(x - 1) + (y + 1) * gridSize.x];
				fGrid[x + y * gridSize.x]->fType = Cell::Type::Middle;
			}
		}
	}
}

void BrickPongApp::saveLevel()
{
	std::ofstream level_file;
	level_file.open("Levels/levels.txt", std::ios::app);
	if (level_file.fail())
	{
		throw std::runtime_error("can't open Levels/levels.txt");
		return;
	}

	level_file << "(";
	for (Ball* bll : fBalls)
	{
		level_file << "[" << bll->getPosition().x << " " << bll->getPosition().y << "]";
	}
	level_file << ";";
	for (PickupItem* item : fItems)
	{
		level_file << "[" << item->getPosition().x << " " << item->getPosition().y << " " << item->getType() << "]";
	}
	level_file << "|" << fPlayer->getType() << ")\n";
	level_file << "{" << "\n";
	for (int y = 0; y < gridSize.y; ++y)
	{
		for (int x = 0; x < gridSize.x; ++x)
		{
			if (fGrid[x + y * gridSize.x]->isEmpty())
				level_file << "0 ";
			else
				level_file << "1 ";
		}
		level_file << "\n";
	}
	level_file << "}" << "\n";


	level_file.close();

	fLevelNames.clear();
	loadLevelNames();

	int index = fLevelNames.size() - 1;
	Button* levelButton = new Button(fTextures, fSoundBuffers, fFonts, Button::Type::Empty, Button::Color::Blue, fLevelNames[index], 20);
	levelButton->fCallbackFunc = [this, index]()
	{
		loadLevel(index);
	};
	levelButton->setPosition(WINDOW_WIDTH / 4.f + levelButton->getBoundingRect().width * int(index % 5) + int(index % 5) * 10.f,
		50.f + int(index / 5) * levelButton->getBoundingRect().height + int(index / 5) * 10.f);
	fLevelsButtons.push_back(levelButton);


}

void BrickPongApp::loadLevel(unsigned int level)
{
	if (level > 1000)
		return;

	deleteEntities();
	for (PickupItem* it : fItems)
	{
		delete it;
	}
	fItems.clear();
	fCurrentLevel = level;

	Player::Type playerType = Player::Green;

	std::ifstream levelToLoad;
	levelToLoad.open("Levels/levels.txt");
	if (levelToLoad.fail())
	{
		throw std::runtime_error("can't open Levels/levels.txt");
		return;
	}

	for (int i = 0; i < level; ++i)
	{
		char c;
		while (levelToLoad >> c && c != '}')
		{}
	}

	char c;
	levelToLoad >> c;
	if (c != '(')
		throw std::runtime_error("syntax error while loading a level");

	while (levelToLoad >> c && (c != '|' && c != ';'))
	{
		if (c == '[')
		{
			float ballPosX, ballPosY;
			levelToLoad >> ballPosX >> ballPosY;
			Ball* bll = new Ball(fWindow, fTextures, fSoundBuffers, *fPlayer, 250.f);
			bll->setPosition(ballPosX, ballPosY);
			fBalls.push_back(bll);
		}
	}


	if(c != '|' && c != ';')
		throw std::runtime_error("syntax error while loading a level");


	int p_type;
	if (c == '|')
	{
		levelToLoad >> p_type;
	}
	else if (c == ';')
	{
		while (levelToLoad >> c && c != '|')
		{
			if (c == '[')
			{
				float ballPosX, ballPosY;
				int it_type = 0;
				levelToLoad >> ballPosX >> ballPosY >> it_type;
				PickupItem* item = new PickupItem(fTextures, fSoundBuffers, static_cast<PickupItem::Type>(it_type));
				item->setPosition(ballPosX, ballPosY);
				fItems.push_back(item);
			}
			
		}

		levelToLoad >> p_type;
	}
	
	playerType = static_cast<Player::Type>(p_type);
	
	fPlayer = new Player(fWindow, fTextures, playerType, 260.f);

	levelToLoad >> c;

	if(c != ')')
		throw std::runtime_error("syntax error while loading a level");


	levelToLoad >> c;
	if(c != '{')
		throw std::runtime_error("syntax error while loading a level");

	if (playerType >= Player::Blue)
	{
		cellSize.x = 43;
		cellSize.y = 128;
		gridSize = sf::Vector2i(WIDTH / cellSize.x, HEIGHT / cellSize.y);
		
	}
	else
	{
		cellSize.x = 128;
		cellSize.y = 43;
		gridSize = sf::Vector2i(WIDTH / cellSize.x, HEIGHT / cellSize.y);
	}


	deleteCells();
	createCells();

	for (int y = 0; y < gridSize.y; ++y)
	{
		for (int x = 0; x < gridSize.x; ++x)
		{
			int b;
			levelToLoad >> b;
			if (b == 1)
			{
				Brick* the_brick = new Brick(fTextures, fSoundBuffers, Brick::Type::BreakableTwoShot, fItems, playerType >= Player::Blue);
				the_brick->mySetPosition(fGrid[x + y * gridSize.x]->getPosition());

				fGrid[x + y * gridSize.x]->setBrick(the_brick);
				fBricks.push_back(the_brick);
			}
			else
			{
				fGrid[x + y * gridSize.x]->setBrick(nullptr);
			}
		}
	}

	levelToLoad >> c;
	if(c != '}')
		throw std::runtime_error("syntax error while loading a level");

	fCurrentState = State::Pause;
	fPlayerLifes.setTextureRect(sf::IntRect(1, 0, 69, 20));
	createVisualGrid();
	levelToLoad.close();
}

void BrickPongApp::deleteBricks()
{
	for (Brick* b : fBricks)
	{
		b->setDead(true);
	}
}

void BrickPongApp::deleteEntities()
{
	delete fPlayer;
	for (Ball* bll : fBalls)
	{
		delete bll;
	}
	fBalls.clear();
}

void BrickPongApp::loadLevelNames()
{
	std::ifstream names;
	names.open("Levels/levelNames.txt");

	if (names.fail())
		throw std::runtime_error("can't open file Levels/levelNames.txt");
	std::string name = "";
	while (!names.eof())
	{
		std::getline(names, name);
		for (int i = 0; i < name.size(); ++i)
		{
			if(!std::isalnum(name[i]))
				name[i] = ' ';
		}
		if(name != "")
			fLevelNames.push_back(name);
		
	}

	names.close();
}

void BrickPongApp::deleteLevel(int level)
{
	/*****LevelsData*****/
	std::ifstream levelStream;
	levelStream.open("Levels/levels.txt");
	if (levelStream.fail())
		throw std::runtime_error("can't open file Levels/levels.txt");


	std::vector<std::string> strings_before;
	//go to the start of the designated level
	for (int i = 0; i < level; ++i)
	{
		std::string str = "";
		while (str != "}")
		{
			std::getline(levelStream, str);
			strings_before.push_back(str);
		}
	}

	long pos_start = levelStream.tellg();

	//go to the end of the designated level
	char c;
	while (levelStream >> c && c != '}')
	{}
	
	long pos_end = levelStream.tellg();

	std::vector<std::string> strings_after;
	while (!levelStream.eof())
	{
		std::string str;
		std::getline(levelStream, str);
		strings_after.push_back(str);
	}

	levelStream.close();
	
	//we're there, so let's delete it !
	//that is moving the non deleted content to a new file
	std::ofstream levelFile;
	levelFile.open("Levels/levels.txt");
	if (levelFile.fail())
		throw std::runtime_error("can't open file Levels/levels.txt");
	
	for (std::string& str : strings_before)
	{
		levelFile << str << "\n";
	}
	for (std::string& str : strings_after)
	{
		levelFile << str << "\n";
	}
	
	levelFile.close();

	/****************/

	/*****Names*****/

	std::ifstream levelNameStream;
	levelNameStream.open("Levels/levelNames.txt");
	if (levelNameStream.fail())
		throw std::runtime_error("can't open file Levels/levelNames.txt");

	std::vector<std::string> names_before;
	for (int i = 0; i < level;)
	{
		if (levelNameStream.eof())
			throw std::logic_error("the level specified doesn't exist");
		std::string str;
		std::getline(levelNameStream, str);
		if (str != "") {
			++i;
			names_before.push_back(str);
		}
	}

	std::string str;
	long namePos_start = levelNameStream.tellg();
	std::getline(levelNameStream, str);
	long namePos_end = levelNameStream.tellg();

	std::vector<std::string> names_after;
	while (!levelNameStream.eof())
	{
		std::string str1;
		std::getline(levelNameStream, str1);
		if(str1 != "")
			names_after.push_back(str1);
	}

	levelNameStream.close();

	//we're there, so let's delete it !
	//that is moving the non deleted content to a new file
	std::ofstream levelNameFile;
	levelNameFile.open("Levels/levelNames.txt");
	if (levelNameFile.fail())
		throw std::runtime_error("can't open file Levels/levelNames.txt");
	if (names_before.size() == 0 && names_after.size() == 0)
	{
		levelNameFile << "";
	}
	else
	{
		for (std::string& s : names_before)
		{
			levelNameFile << s << "\n";
		}
		for (std::string& s : names_after)
		{
			levelNameFile << s << "\n";
		}
	}
	levelNameFile.close();

	/**************/

	fLevelNames.clear();
	loadLevelNames();

	for (Button* b : fLevelsButtons)
	{
		delete b;
	}
	fLevelsButtons.clear();

	for (int i = 0; i < fLevelNames.size(); ++i) {
		Button* levelButton = new Button(fTextures, fSoundBuffers, fFonts, Button::Type::Empty, Button::Color::Blue, fLevelNames[i], 20);
		levelButton->fCallbackFunc = [this, i]()
		{
			loadLevel(i);
		};
		levelButton->setPosition(WINDOW_WIDTH / 4.f + levelButton->getBoundingRect().width * int(i % 5) + 10.f * int(i % 5),
			50.f + int(i / 5) * levelButton->getBoundingRect().height + int(i / 5) * 10.f);
		fLevelsButtons.push_back(levelButton);

	}
}

void BrickPongApp::createVisualGrid()
{
	fVisualGrid.clear();

	sf::Color gridVertColor = sf::Color::Black;
	for (int y = 0; y < gridSize.y; ++y)
	{
		sf::Vertex vert(sf::Vector2f(0.f, y * cellSize.y), gridVertColor);
		sf::Vertex vert1(sf::Vector2f(WIDTH, y * cellSize.y), gridVertColor);
		fVisualGrid.append(vert);
		fVisualGrid.append(vert1);
	}
	for (int x = 0; x < gridSize.x; ++x)
	{
		sf::Vertex vert(sf::Vector2f(x * cellSize.x, 0.f), gridVertColor);
		sf::Vertex vert1(sf::Vector2f(x * cellSize.x, HEIGHT), gridVertColor);
		fVisualGrid.append(vert);
		fVisualGrid.append(vert1);
	}
}

void BrickPongApp::updateStats(sf::Time dt)
{
	fElapsedStatsTime += dt;
	NumFrames++;

	if (fElapsedStatsTime >= sf::seconds(1.0f))
	{
		StatsText.setString("Frames / Second = " + std::to_string(NumFrames) +
			"\nTime / Update = " + std::to_string(fElapsedStatsTime.asMicroseconds() / NumFrames) + "us");

		fElapsedStatsTime -= sf::seconds(1.0f);
		NumFrames = 0;
	}
}

BrickPongApp::~BrickPongApp()
{
	delete fPlayer;
	delete fLevelNamingChatBox;
	for (PickupItem* item : fItems)
	{
		delete item;
	}
	for (Ball* bll : fBalls)
	{
		delete bll;
	}
	for (Cell* c : fGrid)
	{
		delete c->getBrick();
		delete c;
	}
	for (Side* s : fBrickCollisions)
	{
		delete s;
	}
	for (Button* b : fGeneralButtons)
	{
		delete b;
	}
	for (Button* b : fMenuStateButtons)
	{
		delete b;
	}
	for (Button* b : fGameStateButtons)
	{
		delete b;
	}
	for (Button* b : fEditStateButtons)
	{
		delete b;
	}
	for (Button* b : fPauseStateButtons)
	{
		delete b;
	}
	for (Button* b : fDeadStateButtons)
	{
		delete b;
	}
	for (Button* b : fWinStateButtons)
	{
		delete b;
	}
}

void BrickPongApp::loadTextures()
{
	fTextures.load("Textures/paddles_and_ball.png", Textures::ID::Paddles_and_Ball);
	fTextures.load("Textures/Bricks_andOthers.png", Textures::ID::Bricks_andOthers);
	fTextures.load("Textures/Background1.jpg", Textures::ID::Background);
	fTextures.load("Textures/UI.png", Textures::ID::UI);
	fTextures.load("Textures/lifes.png", Textures::ID::PlayerLifes);
}

void BrickPongApp::loadSoundBuffers()
{
	fSoundBuffers.load("SoundEffects/pockSound3.wav", SoundBuffers::ID::BallTouchBrick);
	fSoundBuffers.load("SoundEffects/pockSound1.wav", SoundBuffers::ID::BallTouchPaddle);
	fSoundBuffers.load("SoundEffects/pockSound2.wav", SoundBuffers::ID::BallTouchSide);
	fSoundBuffers.load("SoundEffects/playerLose.wav", SoundBuffers::ID::PlayerLose);
	fSoundBuffers.load("SoundEffects/buttonPressed.ogg", SoundBuffers::ID::ButtonPressed);
	fSoundBuffers.load("SoundEffects/buttonReleased.ogg", SoundBuffers::ID::ButtonReleased);
	fSoundBuffers.load("SoundEffects/key-press-1.wav", SoundBuffers::ID::TypingSound);
	fSoundBuffers.load("SoundEffects/pickup.wav", SoundBuffers::ID::HeartPickup);
	fSoundBuffers.load("SoundEffects/pickup1.wav", SoundBuffers::ID::SpeedPickup);
}

void BrickPongApp::loadFonts()
{
	fFonts.load("Fonts/future.ttf", Fonts::ID::Future);
	fFonts.load("Fonts/future_thin.ttf", Fonts::ID::Future_Thin);
}

void BrickPongApp::update(sf::Time dt)
{
	if (!fEnableMusic)
	{
		fMusic.setVolume(0);
	}
	else
	{
		fMusic.setVolume(100);
	}
	if (fCurrentState == State::Level || fCurrentState == State::Menu)
	{
		fBackGround.setTextureRect(sf::IntRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
	}
	else
	{
		fBackGround.setTextureRect(sf::IntRect(0, 0, WIDTH, HEIGHT));
	}

	switch (fCurrentState)
	{
	case BrickPongApp::State::Menu:
		updateMenuState(dt);
		break;
	case BrickPongApp::State::Game:
		updateGameState(dt);
		break;
	case BrickPongApp::State::Edit:
		updateEditState(dt);
		break;
	case BrickPongApp::State::Pause:
		updatePauseState(dt);
		break;
	case BrickPongApp::State::Dead:
		updateDeadState(dt);
		break;
	case State::Level:
		updateLevelState(dt);
		break;
	case State::LevelNaming:
		updateLevelNaming(dt);
		break;
	case State::Win:
		updateWinState(dt);
		break;
	default:
		break;
	}
}

void BrickPongApp::handleEvent()
{
	sf::Event e;
	while (fWindow.pollEvent(e))
	{
		switch (e.type)
		{
		case sf::Event::Closed:
			fWindow.close();
			break;
		case sf::Event::GainedFocus:
			fFocus = true;
			break;
		case sf::Event::LostFocus:
			fFocus = false;
			break;
		case sf::Event::MouseButtonPressed:
			if (e.mouseButton.button == sf::Mouse::Button::Left)
			{
				for (Button* b : fGeneralButtons)
				{
					if (b->getBoundingRect().contains(sf::Vector2f(sf::Mouse::getPosition(fWindow))))
					{
						b->clickButton();
						b->update(sf::seconds(0.f));
					}
				}
			}
			break;
		default:
			break;
		}

		

		switch (fCurrentState)
		{
		case BrickPongApp::State::Menu:
			handleMenuEvent(e);
			break;
		case BrickPongApp::State::Game:
			handleGameEvent(e);
			break;
		case BrickPongApp::State::Edit:
			handleEditEvent(e);
			break;
		case BrickPongApp::State::Pause:
			handlePauseEvent(e);
			break;
		case BrickPongApp::State::Dead:
			handleDeadEvent(e);
			break;
		case State::Level:
			handleLevelEvent(e);
			break;
		case State::LevelNaming:
			handleLevelNamingEvent(e);
			break;
		case State::Win:
			handleWinStateEvent(e);
			break;
		default:
			break;
		}
	}
}

void BrickPongApp::handleGameEvent(const sf::Event& e)
{
	fPlayer->handleEvent(e);
	for (Ball* bll : fBalls)
	{
		bll->handleEvent(e);
	}
	if (e.type == sf::Event::MouseButtonPressed)
	{
		for (Button* b : fGameStateButtons)
		{
			if (b->getBoundingRect().contains(sf::Vector2f(sf::Mouse::getPosition(fWindow))))
			{
				b->clickButton();
			}
		}
	}
	if (e.type == sf::Event::KeyPressed)
	{
		if (e.key.code == sf::Keyboard::E)
			fCurrentState = State::Edit;
		else if (e.key.code == sf::Keyboard::Escape)
			fCurrentState = State::Pause;
	}
}

void BrickPongApp::handleEditEvent(const sf::Event& e)
{
	switch (e.type)
	{
	case sf::Event::MouseButtonPressed:
	{
		for (Button* b : fEditStateButtons)
		{
			if (b->getBoundingRect().contains(sf::Vector2f(sf::Mouse::getPosition(fWindow))))
			{
				b->clickButton();
			}
		}

		switch (fCurrentPlacingType)
		{
		case BrickPongApp::fball:
			if (e.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(fWindow));
				if (mousePos.x > 0.f && mousePos.x < WIDTH && mousePos.y > 0.f && mousePos.y < HEIGHT && fGrid[getGridCellPosition(mousePos).x + getGridCellPosition(mousePos).y * gridSize.x]->isEmpty()) {
					Ball* bll = new Ball(fWindow, fTextures, fSoundBuffers, *fPlayer, 250.f);
					bll->setPosition(mousePos.x - bll->getSpriteBoundingRect().width / 2.f, mousePos.y - bll->getSpriteBoundingRect().height / 2.f);
					fBalls.push_back(bll);
				}
			}
			if (e.mouseButton.button == sf::Mouse::Right)
			{
				for (auto itr = fBalls.begin(); itr != fBalls.end(); ++itr)
				{
					if ((*itr)->getBoundingRect().contains(sf::Vector2f(sf::Mouse::getPosition(fWindow))))
					{
						delete (*itr);
						fBalls.erase(itr);
						break;
					}
				}
			}
			break;
		case BrickPongApp::fspeedBoost:
			if (e.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(fWindow));
				if (mousePos.x > 0.f && mousePos.x < WIDTH && mousePos.y > 0.f && mousePos.y < HEIGHT && fGrid[getGridCellPosition(mousePos).x + getGridCellPosition(mousePos).y * gridSize.x]->isEmpty()) {
					PickupItem* item = new PickupItem(fTextures, fSoundBuffers, PickupItem::Type::SpeedBoost);
					item->setPosition(mousePos.x, mousePos.y);
					fItems.push_back(item);
				}
			}
			if (e.mouseButton.button == sf::Mouse::Right)
			{
				for (auto itr = fItems.begin(); itr != fItems.end(); ++itr)
				{
					if ((*itr)->getBoundingRect().contains(sf::Vector2f(sf::Mouse::getPosition(fWindow))))
					{
						delete (*itr);
						fItems.erase(itr);
						break;
					}
				}
			}
			break;
		case BrickPongApp::fheart:
			if (e.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(fWindow));
				if (mousePos.x > 0.f && mousePos.x < WIDTH && mousePos.y > 0.f && mousePos.y < HEIGHT && fGrid[getGridCellPosition(mousePos).x + getGridCellPosition(mousePos).y * gridSize.x]->isEmpty()) {
					PickupItem* item = new PickupItem(fTextures, fSoundBuffers, PickupItem::Type::Heart);
					item->setPosition(mousePos.x, mousePos.y);
					fItems.push_back(item);
				}
			}
			if (e.mouseButton.button == sf::Mouse::Right)
			{
				for (auto itr = fItems.begin(); itr != fItems.end(); ++itr)
				{
					if ((*itr)->getBoundingRect().contains(sf::Vector2f(sf::Mouse::getPosition(fWindow))))
					{
						delete (*itr);
						fItems.erase(itr);
						break;
					}
				}
			}
			break;
		default:
			break;
		}
	}
		break;
	case sf::Event::KeyPressed:

		if (e.key.code == sf::Keyboard::E)
			fCurrentState = State::Game;
		else if (e.key.code == sf::Keyboard::Escape)
			fCurrentState = State::Pause;
	
		break;
	default:
		break;
	}
}

void BrickPongApp::handleMenuEvent(const sf::Event& e)
{
	if (e.type == sf::Event::MouseButtonPressed)
	{
		sf::Vector2f mousePos(sf::Mouse::getPosition(fWindow));
		
		for (Button* b : fMenuStateButtons)
		{
			if (b->getBoundingRect().contains(mousePos))
			{
				b->clickButton();
			}
		}

	}
}

void BrickPongApp::handlePauseEvent(const sf::Event& e)
{
	if (e.type == sf::Event::MouseButtonPressed)
	{
		for (Button* b : fPauseStateButtons)
		{
			if (b->getBoundingRect().contains(sf::Vector2f(sf::Mouse::getPosition(fWindow))))
			{
				b->clickButton();
			}
		}
	}
}

void BrickPongApp::handleDeadEvent(const sf::Event& e)
{
	if (e.type == sf::Event::MouseButtonPressed)
	{
		for (Button* b : fDeadStateButtons)
		{
			if (b->getBoundingRect().contains(sf::Vector2f(sf::Mouse::getPosition(fWindow))))
			{
				b->clickButton();
			}
		}
	}
}

void BrickPongApp::handleLevelEvent(const sf::Event& e)
{
	if (e.type == sf::Event::MouseButtonPressed)
	{
		for (Button* b : fLevelStateButtons)
		{
			if (b->getBoundingRect().contains(sf::Vector2f(sf::Mouse::getPosition(fWindow))))
			{
				b->clickButton();
			}
		}

		for (Button* b : fLevelsButtons)
		{
			if (b->getBoundingRect().contains(sf::Vector2f(sf::Mouse::getPosition(fWindow))))
			{
				if (fStartDoubleClickTimer && b->currentTexture())
				{
					b->clickButton();
				}
				else {
					for (Button* b1 : fLevelsButtons)
					{
						b1->unclickButton();
					}
					b->playClickSound();
					b->changeTexture(true);
					fStartDoubleClickTimer = true;

				}
			}
		}
	}
	else  if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Delete)
	{
		for (int i = 0; i < fLevelsButtons.size(); ++i)
		{
			if (fLevelsButtons[i]->currentTexture())
			{
				//delete level
				deleteLevel(i);
			}
		}
	}
}

void BrickPongApp::handleLevelNamingEvent(const sf::Event& e)
{
	fLevelNamingChatBox->handleEvent(e);
	if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)
	{
		fCurrentState = State::Edit;
	}
}

void BrickPongApp::handleWinStateEvent(const sf::Event& e)
{
	if (e.type == sf::Event::MouseButtonPressed)
	{
		for (Button* b : fWinStateButtons)
		{
			if (b->getBoundingRect().contains(sf::Vector2f(sf::Mouse::getPosition(fWindow))))
			{
				b->clickButton();
			}
		}
	}
}

void BrickPongApp::updateGameState(sf::Time dt)
{

	/*reseting the sides in each cells*/
	for (Cell* c : fGrid) {
		for (int i = 0; i < Cell::SideCount; ++i)
		{
			c->fSides[i] = nullptr;
		}
	}
	/************************/

	/*clearing out the side's container*/
	for (Side* s : fBrickCollisions)
	{
		delete s;
	}
	fBrickCollisions.clear();
	/************************/

	/*updating each brick, that is calculating the collision shape made by the bricks*/
	for (Brick* b : fBricks)
	{
		sf::Vector2i pos = getGridCellPosition(b->myGetPosition());
		fGrid[pos.x + pos.y * gridSize.x]->updateCell(fBrickCollisions);

	}
	/**************************************/
	
	for (PickupItem* item : fItems)
	{
		item->update(dt);
		if (fEnableSoundEffects)
		{
			item->setVolume(100);
		}
		else
		{
			item->setVolume(0);
		}
	}


	/*updating collisions and entities*/
	checkCollisions();
	fPlayer->update(dt);
	for (Ball* bll : fBalls)
	{
		bll->update(dt); 
		if (fEnableSoundEffects)
		{
			bll->setVolume(100);
		}
		else
		{
			bll->setVolume(0);
		}
	}

	for (auto itr = fBricks.begin(); itr != fBricks.end(); ++itr)
	{
		if ((*itr)->isDead())
		{
			sf::Vector2i pos = getGridCellPosition((*itr)->myGetPosition());
			fGrid[pos.x + pos.y * gridSize.x]->setBrick(nullptr);
			delete (*itr);
			fBricks.erase(itr);
			break;
		}
	}

	for (auto itr = fItems.begin(); itr != fItems.end(); ++itr)
	{
		if ((*itr)->isDead())
		{
			delete (*itr);
			fItems.erase(itr);
			break;
		}
	}

	/*******************************/

	if (fPlayer->isDead())
	{
		fCurrentState = State::Dead;
		fPlayerLifes.setTextureRect(sf::IntRect(0, 0, 0, 0));
	}
	else
	{
		fPlayerLifes.setTextureRect(sf::IntRect(1, 0 + 20 * (3 - fPlayer->getLifes()), 23 * fPlayer->getLifes(), 20));
		fPlayerLifes.setPosition(WIDTH / 2.f - fPlayerLifes.getGlobalBounds().width / 2.f, 0.f);
	}

	if (fBricks.empty() && !fPlayer->isDead() && fCurrentLevel != -1)
	{
		//YOU WIN !
		fCurrentState = State::Win;
	}

	for (Button* b : fGameStateButtons)
	{
		if (fEnableSoundEffects)
		{
			b->setVolume(100);
		}
		else
		{
			b->setVolume(0);
		}
	}
}

void BrickPongApp::updateMenuState(sf::Time dt)
{
	fCurrentLevel = -1;
	for (Button* b : fMenuStateButtons)
	{
		if (fEnableSoundEffects)
		{
			b->setVolume(100);
		}
		else
		{
			b->setVolume(0);
		}
	}
}

void BrickPongApp::updateEditState(sf::Time dt)
{
	fEditStateButtons[fchangingpaddle]->changePaddleButton(fPlayer->getType());

	sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(fWindow));
	sf::Vector2i mouseGridPos = getGridCellPosition(mousePos);
	if (mouseGridPos.x >= 0 && mouseGridPos.x < gridSize.x && mouseGridPos.y >= 0 && mouseGridPos.y < gridSize.y && fCurrentPlacingType == brick) {

		Cell* c = fGrid[mouseGridPos.x + mouseGridPos.y * gridSize.x];
		bool buff = true;
		for (int i = 0; i < fBalls.size(); ++i)
		{
			if (fBalls[i] && fBalls[i]->getBoundingRect().intersects(c->getBoundingRect()))
				buff = false;
		}

		if (buff) 
		{
		
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
				bool buff = true;
				if (fPlayer->getType() >= Player::Blue && c->getPosition().x == 0.f)
					buff = false;
				if (c->isEmpty() && buff)
				{
					Brick* brick = new Brick(fTextures, fSoundBuffers, Brick::Type::BreakableTwoShot, fItems, fPlayer->getType() >= Player::Blue);
					brick->mySetPosition(c->getPosition());
					c->setBrick(brick);
					fBricks.push_back(brick);
				}
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
			{
				if (!c->isEmpty())
				{
					c->getBrick()->setDead(true);
				}
			}
		}
	}

	/*reseting the sides in each cells*/
	for (Cell* c : fGrid) {
		for (int i = 0; i < Cell::SideCount; ++i)
		{
			c->fSides[i] = nullptr;
		}
	}
	/************************/

	/*clearing out the side's container*/
	for (Side* s : fBrickCollisions)
	{
		delete s;
	}
	fBrickCollisions.clear();
	/************************/

	/*updating each brick, that is calculating the collision shape made by the bricks*/
	for (Brick* b : fBricks)
	{
		sf::Vector2i pos = getGridCellPosition(b->myGetPosition());
		fGrid[pos.x + pos.y * gridSize.x]->updateCell(fBrickCollisions);

	}
	/**************************************/

	for (auto itr = fBricks.begin(); itr != fBricks.end(); ++itr)
	{
		if ((*itr)->isDead())
		{
			sf::Vector2i pos = getGridCellPosition((*itr)->myGetPosition());
			fGrid[pos.x + pos.y * gridSize.x]->setBrick(nullptr);
			delete (*itr);
			fBricks.erase(itr);
			break;
		}
	}

	for (Button* b : fEditStateButtons)
	{
		if (fEnableSoundEffects)
		{
			b->setVolume(100);
		}
		else
		{
			b->setVolume(0);
		}
	}

}

void BrickPongApp::updateDeadState(sf::Time dt)
{
	if (fCurrentLevel != -1)
	{
		fDeadStateButtons[0]->setColor(sf::Color::White);
	}
	for (Button* b : fDeadStateButtons)
	{
		if (fEnableSoundEffects)
		{
			b->setVolume(100);
		}
		else
		{
			b->setVolume(0);
		}
	}
}

void BrickPongApp::updatePauseState(sf::Time dt)
{
	if (fCurrentLevel != -1)
	{
		fPauseStateButtons[1]->setColor(sf::Color::White);
	}
	for (Button* b : fPauseStateButtons)
	{
		if (fEnableSoundEffects)
		{
			b->setVolume(100);
		}
		else
		{
			b->setVolume(0);
		}
	}
}

void BrickPongApp::updateLevelState(sf::Time dt)
{
	if (fStartDoubleClickTimer)
	{
		fDoubleClickAccu += dt;
		if (fDoubleClickAccu >= fDoubleClickDelay)
		{
			fDoubleClickAccu = sf::Time::Zero;
			fStartDoubleClickTimer = false;
		}
	}

	for (Button* b : fLevelStateButtons)
	{
		if (fEnableSoundEffects)
		{
			b->setVolume(100);
		}
		else
		{
			b->setVolume(0);
		}
	}
	for (Button* b : fLevelsButtons)
	{
		if (fEnableSoundEffects)
		{
			b->setVolume(100);
		}
		else
		{
			b->setVolume(0);
		}
	}
}

void BrickPongApp::updateLevelNaming(sf::Time dt)
{
	fLevelNamingChatBox->update(dt);
	if (fEnableSoundEffects)
	{
		fLevelNamingChatBox->setVolume(100);
	}
	else
	{
		fLevelNamingChatBox->setVolume(0);
	}

	if (fLevelNamingChatBox->fHasFinishedEnteringSmth)
	{
		std::string name = fLevelNamingChatBox->getText();

		std::ofstream outNames;
		outNames.open("Levels/levelNames.txt", std::ios::app);
		if (outNames.fail())
			throw std::runtime_error("can't open file Levels/levelNames.txt");

		outNames << name << "\n";

		outNames.close();

		saveLevel();
		fLevelNamingChatBox->fHasFinishedEnteringSmth = false;
		fLevelNamingChatBox->setText("");
		fCurrentState = State::Edit;
	}
}

void BrickPongApp::updateWinState(sf::Time dt)
{
	if (fCurrentLevel != -1)
	{
		fWinStateButtons[0]->setColor(sf::Color::White);
	}
	for (Button* b : fWinStateButtons)
	{
		if (fEnableSoundEffects)
		{
			b->setVolume(100);
		}
		else
		{
			b->setVolume(0);
		}
	}
}

void BrickPongApp::render()
{
	fWindow.clear();

	fWindow.draw(fBackGround);


	if (fCurrentState != State::Menu && fCurrentState != State::Level) {
		fWindow.draw(*fPlayer);
		for (Ball* bll : fBalls)
		{
			fWindow.draw(*bll);
		}

		for (PickupItem* item : fItems)
		{
			fWindow.draw(*item);
		}

		for (Cell* c : fGrid)
		{
			if (!c->isEmpty())
			{
				fWindow.draw(*c->getBrick());
			}
		}

		if (fCurrentState == State::Edit)
		{
			fWindow.draw(fVisualGrid);
		}
		fWindow.draw(fPlayerLifes);

		//for testing purposes only
		//for (Side* s : fBrickCollisions)
		//{
		//	sf::Color vertColor = sf::Color::Red;
		//	sf::VertexArray vertArr;
		//	vertArr.setPrimitiveType(sf::PrimitiveType::Lines);
		//	vertArr.append(sf::Vertex(s->fA, vertColor));
		//	vertArr.append(sf::Vertex(s->fB, vertColor));
		//	fWindow.draw(vertArr);
		//}
		//for (Cell* cell : fCellDrawer)
		//{
		//	sf::RectangleShape rect(sf::Vector2f(cell->getBoundingRect().width, cell->getBoundingRect().height));
		//	rect.setPosition(cell->getPosition());
		//	rect.setFillColor(sf::Color::Transparent);
		//	rect.setOutlineThickness(1.f);
		//	rect.setOutlineColor(sf::Color::Green);
		//	fWindow.draw(rect);
		//}
	}

	if (fCurrentState != State::Game && fCurrentState != State::Edit)
	{
		sf::RectangleShape rect{ sf::Vector2f(fWindow.getSize()) };
		rect.setFillColor(sf::Color::Color(0, 0, 0, 125));

		fWindow.draw(rect);
	}

	switch (fCurrentState)
	{
	case BrickPongApp::State::Menu:
		for (Button* b : fMenuStateButtons)
		{
			fWindow.draw(*b);
		}
		break;
	case BrickPongApp::State::Game:
		for (Button* b : fGameStateButtons)
		{
			fWindow.draw(*b);
		}
		break;
	case BrickPongApp::State::Edit:
		for (Button* b : fEditStateButtons)
		{
			fWindow.draw(*b);
		}
		break;
	case BrickPongApp::State::Pause:
		for (Button* b : fPauseStateButtons)
		{
			fWindow.draw(*b);
		}
		break;
	case BrickPongApp::State::Dead:
		for (Button* b : fDeadStateButtons)
		{
			fWindow.draw(*b);
		}
		break;
	case BrickPongApp::State::Level:
		for (Button* b : fLevelStateButtons)
		{
			fWindow.draw(*b);
		}
		for (Button* b : fLevelsButtons)
		{
			fWindow.draw(*b);
		}
		break;
	case State::LevelNaming:
		fWindow.draw(*fLevelNamingChatBox);
		break;
	case BrickPongApp::State::Win:
		for (Button* b : fWinStateButtons)
		{
			fWindow.draw(*b);
		}
		fWindow.draw(fWinText);
		break;
	default:
		break;
	}

	for (Button* b : fGeneralButtons)
	{
		fWindow.draw(*b);
	}

	fWindow.draw(fMusicCheckMark);
	fWindow.draw(fSoundEffectCheckMark);

	fWindow.draw(StatsText);
	fWindow.display();
}

bool doesCollide(Entity& a, Entity& b)
{
	return a.getBoundingRect().intersects(b.getBoundingRect());
}

void BrickPongApp::checkCollisions()
{
	checkBallPlayerCollision();
	checkBallBricksCollision();
	
	//checking ball-item collisions
	for (PickupItem* item : fItems)
	{
		for (Ball* ball : fBalls) {
			if (item->getBoundingRect().intersects(ball->getBoundingRect()))
			{
				switch (item->getType())
				{
				case PickupItem::Heart:
					item->effectOnPlayer(*fPlayer);
					break;
				case PickupItem::SpeedBoost:
					item->effectOnBall(*ball, *fPlayer);
					break;
				default:
					break;
				}
			}
		}
	}
}





void BrickPongApp::checkBallPlayerCollision()
{
	for (Ball* bll : fBalls)
	{
		if (doesCollide(*bll, *fPlayer))	//TODO: fix the bug when the ball touches the side of the paddle
		{
			bll->playPaddleColSound();
			switch (fPlayer->getType())
			{
			case Player::Type::Green:
				if (bll->getBoundingRect().intersects(fPlayer->getBorderBoundingLeft()) || bll->getBoundingRect().intersects(fPlayer->getBorderBoundingRight()))
				{
					if (bll->getBoundingRect().top + bll->getBoundingRect().height >= fPlayer->getBoundingRect().top + fPlayer->getBoundingRect().height)
					{
						playerLose(*bll);
					}
					else {
						bll->setVelocity(sf::Vector2f(-bll->getVelocity().x, -bll->getVelocity().y));
					}
				}
				else
				{
					bll->setVelocity(sf::Vector2f(bll->getVelocity().x, -bll->getVelocity().y));
				}
				break;
			case Player::Type::Purple:
				if (bll->getBoundingRect().intersects(fPlayer->getBorderBoundingLeft()) || bll->getBoundingRect().intersects(fPlayer->getBorderBoundingRight()))
				{
					if (bll->getBoundingRect().top <= fPlayer->getBoundingRect().top)
					{
						playerLose(*bll);
					}
					else {
						bll->setVelocity(sf::Vector2f(-bll->getVelocity().x, -bll->getVelocity().y));
					}
				}
				else
				{
					bll->setVelocity(sf::Vector2f(bll->getVelocity().x, -bll->getVelocity().y));
				}
				break;
			case Player::Type::Blue:
				if (bll->getBoundingRect().intersects(fPlayer->getBorderBoundingLeft()) || bll->getBoundingRect().intersects(fPlayer->getBorderBoundingRight()))
				{
					if (bll->getBoundingRect().left <= fPlayer->getBoundingRect().left)
					{
						playerLose(*bll);
					}
					else {
						bll->setVelocity(sf::Vector2f(-bll->getVelocity().x, -bll->getVelocity().y));
					}
				}
				else
				{
					bll->setVelocity(sf::Vector2f(-bll->getVelocity().x, bll->getVelocity().y));
				}
				break;
			case Player::Type::Red:
				if (bll->getBoundingRect().intersects(fPlayer->getBorderBoundingLeft()) || bll->getBoundingRect().intersects(fPlayer->getBorderBoundingRight()))
				{
					if (bll->getBoundingRect().left + bll->getBoundingRect().width >= fPlayer->getBoundingRect().left + fPlayer->getBoundingRect().width)
					{
						playerLose(*bll);
					}
					else {
						bll->setVelocity(sf::Vector2f(-bll->getVelocity().x, -bll->getVelocity().y));
					}
				}
				else
				{
					bll->setVelocity(sf::Vector2f(-bll->getVelocity().x, bll->getVelocity().y));
				}
				break;
			default:
				break;
			}
		}
		switch (fPlayer->getType())
		{
		case Player::Type::Green:
			if (bll->getBoundingRect().left <= 0.f || bll->getBoundingRect().left + bll->getBoundingRect().width >= WIDTH)
			{
				bll->setVelocity(sf::Vector2f(-bll->getVelocity().x, bll->getVelocity().y));
				bll->playSideColSound();
			}
			if (bll->getBoundingRect().top <= 0.f)
			{
				bll->setVelocity(sf::Vector2f(bll->getVelocity().x, -bll->getVelocity().y));
				bll->playSideColSound();
			}
			if (bll->getBoundingRect().top >= HEIGHT)
			{
				playerLose(*bll);
			}
			break;
		case Player::Type::Purple:
			if (bll->getBoundingRect().left <= 0.f || bll->getBoundingRect().left + bll->getBoundingRect().width >= WIDTH)
			{
				bll->setVelocity(sf::Vector2f(-bll->getVelocity().x, bll->getVelocity().y));
				bll->playSideColSound();
			}
			if (bll->getBoundingRect().top + bll->getBoundingRect().height >= HEIGHT)
			{
				bll->setVelocity(sf::Vector2f(bll->getVelocity().x, -bll->getVelocity().y));
				bll->playSideColSound();
			}
			if (bll->getBoundingRect().top + bll->getBoundingRect().height <= 0.f)
			{
				playerLose(*bll);
			}
			break;
		case Player::Type::Blue:
			if (bll->getBoundingRect().top + bll->getBoundingRect().height >= HEIGHT || bll->getBoundingRect().top <= 0.f)
			{
				bll->setVelocity(sf::Vector2f(bll->getVelocity().x, -bll->getVelocity().y));
				bll->playSideColSound();
			}
			if (bll->getBoundingRect().left + bll->getBoundingRect().width >= WIDTH)
			{
				bll->setVelocity(sf::Vector2f(-bll->getVelocity().x, bll->getVelocity().y));
				bll->playSideColSound();
			}
			if (bll->getBoundingRect().left <= 0.f)
			{
				playerLose(*bll);
			}
			break;
		case Player::Type::Red:
			if (bll->getBoundingRect().left <= 0.f)
			{
				bll->setVelocity(sf::Vector2f(-bll->getVelocity().x, bll->getVelocity().y));
				bll->playSideColSound();
			}
			if (bll->getBoundingRect().top + bll->getBoundingRect().height >= HEIGHT || bll->getBoundingRect().top <= 0.f)
			{
				bll->setVelocity(sf::Vector2f(bll->getVelocity().x, -bll->getVelocity().y));
				bll->playSideColSound();
			}
			if (bll->getBoundingRect().left + bll->getBoundingRect().width >= WIDTH)
			{
				playerLose(*bll);
			}
			break;
		default:
			break;
		}
	}
}


void BrickPongApp::checkBallBricksCollision()
{
	for (Ball* bll : fBalls)
	{
		int index = 0;
		for (Side* s : fBrickCollisions)
		{
			sf::Vector2f line = (s->fB - s->fA);
			(line.x == 0) ? line.x = 1 : line.y = 1;	//needs to be at least one in order to detect collision

			if (bll->getBoundingRect().intersects(sf::FloatRect(s->fA, line)))	//the ball is in collision with some bricks
			{
				sf::Vector2f vel;
				if (line.x > 1) {
					vel = sf::Vector2f(0.f, bll->getVelocity().y);
					bll->setVelocity(sf::Vector2f(bll->getVelocity().x, -bll->getVelocity().y));
				}
				else
				{
					vel = sf::Vector2f(bll->getVelocity().x, 0.f);
					bll->setVelocity(sf::Vector2f(-bll->getVelocity().x, bll->getVelocity().y));
				}
				if (++index == 1) {

					sf::Vector2i pos = getGridCellPosition(sf::Vector2f(bll->getBoundingRect().left + bll->getBoundingRect().width / 2.f, bll->getBoundingRect().top + bll->getBoundingRect().height / 2.f));
					int x_pos = pos.x;
					int y_pos = pos.y;

					if (x_pos >= gridSize.x)
						x_pos--;
					if (y_pos >= gridSize.y)
						y_pos--;

					Cell* cell = fGrid[x_pos + y_pos * gridSize.x];
					fCellDrawer.push_back(cell);
					if (!cell->isEmpty())
					{
						cell->getBrick()->applyDamage();
						bll->playBrickColSound();
					}
					else {
						bll->move(vel * fFrameRate.asSeconds());
						for (int i = 0; i < Cell::FullNeighborCount; ++i)
						{
							if (cell->fNeighbors[i] && !cell->fNeighbors[i]->isEmpty() && cell->fNeighbors[i]->getBoundingRect().intersects(bll->getBoundingRect()))
							{
								cell->fNeighbors[i]->getBrick()->applyDamage();
								bll->playBrickColSound();
							}
						}

						bll->move(-vel * fFrameRate.asSeconds());
					}
				}

			}
		}
	}
}

void BrickPongApp::playerLose(Ball& bll)
{
	fPlayer->takeLife();
	bll.playPlayerLoseSound();
	bll.respawn();
}

sf::Vector2i BrickPongApp::getGridPosition(const Cell& cell) const
{
	return sf::Vector2i(cell.getPosition().x * cellSize.x, cell.getPosition().y * cellSize.y);
}

sf::Vector2i BrickPongApp::getGridCellPosition(const sf::Vector2f& pos) const
{
	return sf::Vector2i(pos.x / cellSize.x, pos.y / cellSize.y);
}
