#pragma once
#include "Entity.h"
#include "SFML/Audio.hpp"

class Player;

class Ball : public Entity
{
public:
	Ball(sf::RenderWindow& window, TexturesHolder& textures, SoundBuffersHolder& soundBuffers, Player& player, float speed = 150.f);

	void update(sf::Time dt) override;

	void setVelocity(sf::Vector2f newVel);
	sf::Vector2f getVelocity() const;

	void setSpeed(float newSpeed);
	float getSpeed() const;

	void playBrickColSound();
	void playPaddleColSound();
	void playSideColSound();
	void playPlayerLoseSound();
	
	void setVolume(float v);

	void respawn();
private:

	sf::Vector2f fVelocity;
	float fSpeed;
	sf::Sound fSoundEffectPaddle;
	sf::Sound fSoundEffectBrick;
	sf::Sound fSoundEffectSide;
	sf::Sound fSoundEffectPlayerLose;
	sf::RenderWindow& fWindow;
};