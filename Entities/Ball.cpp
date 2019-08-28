#include "Ball.h"
#include "UtilityFunc.h"
#include "Player.h"

Ball::Ball(sf::RenderWindow& window, TexturesHolder& textures, SoundBuffersHolder& soundBuffers, Player& player, float speed)
	:fSpeed(speed), fVelocity((1 - 2 * Utility::randomInt(0, 1)) * speed / float(sqrt(2)), (1 - 2 * Utility::randomInt(0, 1))* speed / float(sqrt(2)))
	, fSoundEffectBrick(soundBuffers.get(SoundBuffers::ID::BallTouchBrick)), fSoundEffectPaddle(soundBuffers.get(SoundBuffers::ID::BallTouchPaddle))
	, fWindow(window), fSoundEffectSide(soundBuffers.get(SoundBuffers::ID::BallTouchSide)), fSoundEffectPlayerLose(soundBuffers.get(SoundBuffers::ID::PlayerLose))
{

	fSprite.setTexture(textures.get(Textures::ID::Paddles_and_Ball));
	fSprite.setTextureRect(sf::IntRect(98, -2, 54, 54));

	setPosition(sf::Vector2f(window.getSize() / 2U));

	fBoundingRect = sf::FloatRect(19, 19, 17, 17);
}

void Ball::update(sf::Time dt)
{
	move(fVelocity * dt.asSeconds());
}

void Ball::setVelocity(sf::Vector2f newVel)
{
	fVelocity = newVel;
}

sf::Vector2f Ball::getVelocity() const
{
	return fVelocity;
}

void Ball::setSpeed(float newSpeed)
{
	fSpeed = newSpeed;
	int x = getVelocity().x / abs(getVelocity().x);
	int y = getVelocity().y / abs(getVelocity().y);
	setVelocity(sf::Vector2f(x * fSpeed / float(sqrt(2)), y * fSpeed / float(sqrt(2))));
}

float Ball::getSpeed() const
{
	return fSpeed;
}

void Ball::playBrickColSound()
{
	fSoundEffectBrick.play();
}

void Ball::playPaddleColSound()
{
	fSoundEffectPaddle.play();
}

void Ball::playSideColSound()
{
	fSoundEffectSide.play();
}

void Ball::playPlayerLoseSound()
{
	fSoundEffectPlayerLose.play();
}

void Ball::setVolume(float v)
{
	fSoundEffectBrick.setVolume(v);
	fSoundEffectPaddle.setVolume(v);
	fSoundEffectPlayerLose.setVolume(v);
	fSoundEffectSide.setVolume(v);
}

void Ball::respawn()
{
	setPosition(sf::Vector2f(WIDTH / 2, HEIGHT / 2));
	setVelocity(((Utility::randomInt(0, 1) == 1) ? sf::Vector2f(-getSpeed() / float(sqrt(2)), getSpeed() / float(sqrt(2)))
		: sf::Vector2f(getSpeed() / float(sqrt(2)), getSpeed() / float(sqrt(2)))));
}
