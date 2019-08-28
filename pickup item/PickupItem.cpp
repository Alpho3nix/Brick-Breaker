#include "PickupItem.h"
#include "Player.h"
#include "Ball.h"
#include "UtilityFunc.h"

PickupItem::PickupItem(TexturesHolder& textures, SoundBuffersHolder& sounds, Type type)
	:fSprite(), fPickupSound(sounds.get(SoundBuffers::ID::HeartPickup)), fType(type)
{
	handleType(type, textures, sounds);
}

sf::FloatRect PickupItem::getBoundingRect() const
{
	return getTransform().transformRect(fSprite.getGlobalBounds());
}

void PickupItem::effectOnPlayer(Player& player)
{
	fPickupSound.play();
	fEffectPlayer(player);
	fBeginDeathTimer = true;
}

void PickupItem::effectOnBall(Ball& ball, Player& player)
{
	fPickupSound.play();
	fEffectBall(ball);
	fBeginDeathTimer = true;
	fEffectPlayer(player);
}

void PickupItem::kill()
{
	fDead = true;
}

bool PickupItem::isDead() const
{
	return fDead;
}

void PickupItem::update(sf::Time dt)
{
	if (fBeginDeathTimer)
	{
		fDeathAccu += dt;
		if (fDeathAccu >= fDeathDelay)
		{
			fDeathAccu = sf::Time::Zero;
			fBeginDeathTimer = false;
			kill();
		}
		else
		{
			fSprite.setTextureRect(sf::IntRect(0, 0, 0, 0));
		}
	}
}

void PickupItem::setVolume(float v)
{
	fPickupSound.setVolume(v);
}

PickupItem::Type PickupItem::getType() const
{
	return fType;
}

void PickupItem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(fSprite, states);
}

void PickupItem::handleType(Type type, TexturesHolder& textures, SoundBuffersHolder& sounds)
{
	switch (type)
	{
	case PickupItem::Heart:
		fSprite.setTexture(textures.get(Textures::ID::PlayerLifes));
		fSprite.setTextureRect(sf::IntRect(0, 40, 23, 20));

		fPickupSound.setBuffer(sounds.get(SoundBuffers::ID::HeartPickup));
		fEffectPlayer = [](Player& player)
		{
			player.addLife();
		};
		break;
	case PickupItem::SpeedBoost:
		fSprite.setTexture(textures.get(Textures::ID::Bricks_andOthers));
		fSprite.setTextureRect(sf::IntRect(257, 282, 22, 21));

		fPickupSound.setBuffer(sounds.get(SoundBuffers::ID::SpeedPickup));
		fEffectBall = [](Ball& ball)
		{
			ball.setSpeed(ball.getSpeed() + 20.f);
		};
		fEffectPlayer = [](Player& player)
		{
			player.setSpeed(player.getSpeed() + 20.f);
		};
		break;
	default:
		break;
	}

	Utility::CenterOrigin(fSprite);
}
