#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "ResourceIdentifiers.h"
#include <functional>

class Player;
class Ball;

class PickupItem : public sf::Drawable, public sf::Transformable
{
public:
	enum Type
	{
		Heart, SpeedBoost
	};

	PickupItem(TexturesHolder& textures, SoundBuffersHolder& sounds, Type type);

	sf::FloatRect getBoundingRect() const;
	
	void effectOnPlayer(Player& player);
	void effectOnBall(Ball& ball, Player& player);

	void kill();
	bool isDead() const;

	void update(sf::Time dt);


	void setVolume(float v);
	Type getType() const;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::Sprite fSprite;
	void handleType(Type type, TexturesHolder& textures, SoundBuffersHolder& sounds);

	sf::Sound fPickupSound;

	std::function<void(Player& player)> fEffectPlayer;
	std::function<void(Ball& ball)> fEffectBall;
	bool fDead = false;

	Type fType;

	sf::Time fDeathDelay = sf::seconds(0.5f);
	sf::Time fDeathAccu = sf::Time::Zero;
	bool fBeginDeathTimer = false;
};