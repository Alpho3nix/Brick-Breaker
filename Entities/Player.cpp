#include "Player.h"
#include <iostream>

Player::Player(sf::RenderWindow& window, TexturesHolder& textures, Type type, float paddleVelocity)
	:Entity(), fType(type), fPaddleVelocity(), fPaddleSpeed(paddleVelocity), fWindow(window), fLife(3)
{
	handleType(type, textures);
}

void Player::setType(Type newType)
{
	if (newType == fType)
		return;
	
	fType = newType;
	switch (newType)
	{
	case Player::Green:
	{
		fSprite.setTextureRect(sf::IntRect(105, 52, 140, 46));
		setPosition(WIDTH / 2.f - fSprite.getGlobalBounds().width / 2.f, HEIGHT - fSprite.getGlobalBounds().height);
		fBoundingRect = sf::FloatRect(17, 20, 106, 6);
		fBorderBoundingLeft = sf::FloatRect(fBoundingRect.left, fBoundingRect.top, 5.f, fBoundingRect.height);
		fBorderBoundingRight = sf::FloatRect(fBoundingRect.left + fBoundingRect.width - 5.f, fBoundingRect.top, 5.f, fBoundingRect.height);

	}
		break;
	case Player::Purple:
	{
		fSprite.setTextureRect(sf::IntRect(105, 102, 140, 46));
		setPosition(WIDTH / 2.f - fSprite.getGlobalBounds().width / 2.f, 0.f);
		fBoundingRect = sf::FloatRect(17, 20, 106, 6);
		fBorderBoundingLeft = sf::FloatRect(fBoundingRect.left, fBoundingRect.top, 5.f, fBoundingRect.height);
		fBorderBoundingRight = sf::FloatRect(fBoundingRect.left + fBoundingRect.width - 5.f, fBoundingRect.top, 5.f, fBoundingRect.height);
	}
		break;
	case Player::Blue:
	{
		fSprite.setTextureRect(sf::IntRect(52, 5, 46, 140));
		setPosition(0.f, HEIGHT / 2.f - fSprite.getGlobalBounds().height / 2.f);
		fBoundingRect = sf::FloatRect(20, 17, 6, 106);
		fBorderBoundingLeft = sf::FloatRect(fBoundingRect.left, fBoundingRect.top, fBoundingRect.width, 5.f);
		fBorderBoundingRight = sf::FloatRect(fBoundingRect.left, fBoundingRect.top + fBoundingRect.height - 5.f, fBoundingRect.width, 5.f);
	}
		break;
	case Player::Red:
	{
		fSprite.setTextureRect(sf::IntRect(2, 5, 46, 140));
		setPosition(WIDTH - fSprite.getGlobalBounds().width, HEIGHT / 2.f - fSprite.getGlobalBounds().height / 2.f);
		fBoundingRect = sf::FloatRect(20, 17, 6, 106);
		fBorderBoundingLeft = sf::FloatRect(fBoundingRect.left, fBoundingRect.top, fBoundingRect.width, 5.f);
		fBorderBoundingRight = sf::FloatRect(fBoundingRect.left, fBoundingRect.top + fBoundingRect.height - 5.f, fBoundingRect.width, 5.f);
	}
		break;
	default:
		break;
	}
}

Player::Type Player::getType() const
{
	return fType;
}

void Player::handleEvent(const sf::Event& e)
{
	if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Space)
	{
		//Shoot !
	}
}

void Player::realTimeInput(sf::Time dt)
{
	fPaddleVelocity = sf::Vector2f(0.f, 0.f);
	if (fType >= Type::Blue) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		{
			//go upwards
			fPaddleVelocity = sf::Vector2f(0.f, -fPaddleSpeed * dt.asSeconds());
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			//go downwards
			fPaddleVelocity = sf::Vector2f(0.f, fPaddleSpeed * dt.asSeconds());
		}
	}
	else
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			//go right
			fPaddleVelocity = sf::Vector2f(fPaddleSpeed * dt.asSeconds(), 0.f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		{
			//go left
			fPaddleVelocity = sf::Vector2f(-fPaddleSpeed * dt.asSeconds(), 0.f);
		}
	}
}

void Player::update(sf::Time dt)
{
	realTimeInput(dt);
	move(fPaddleVelocity);

	while (!isNotOutside())
	{
		move(-fPaddleVelocity);
	}
}

void Player::takeLife()
{
	if (!fIsDead) {
		if (--fLife <= 0)
		{
			fLife = 0;
			fIsDead = true;
			//DEAD
		}
	}
}

void Player::addLife()
{
	if (fLife < 3)
	{
		fLife++;
	}
	else
	{
		fLife = 3;
	}
}

int Player::getLifes() const
{
	return fLife;
}

void Player::setSpeed(float speed)
{
	fPaddleSpeed = speed;
}

float Player::getSpeed() const
{
	return fPaddleSpeed;
}


sf::FloatRect Player::getBorderBoundingLeft() const
{
	return getTransform().transformRect(fBorderBoundingLeft);
}

sf::FloatRect Player::getBorderBoundingRight() const
{
	return getTransform().transformRect(fBorderBoundingRight);
}


void Player::handleType(Type type, TexturesHolder& textures)
{
	switch (type)
	{
	case Player::Green:
	{
		sf::Texture& tex = textures.get(Textures::ID::Paddles_and_Ball);
			
		fSprite.setTexture(tex);
		fSprite.setTextureRect(sf::IntRect(105, 52, 140, 46));
		setPosition(WIDTH / 2.f - fSprite.getGlobalBounds().width / 2.f, HEIGHT - fSprite.getGlobalBounds().height);
		fBoundingRect = sf::FloatRect(17, 20, 106, 6);
		fBorderBoundingLeft = sf::FloatRect(fBoundingRect.left, fBoundingRect.top, 5.f, fBoundingRect.height);
		fBorderBoundingRight = sf::FloatRect(fBoundingRect.left + fBoundingRect.width - 5.f, fBoundingRect.top, 5.f, fBoundingRect.height);
		
	}
	break;
	case Player::Purple:
	{

		sf::Texture& tex = textures.get(Textures::ID::Paddles_and_Ball);
			
		fSprite.setTexture(tex);
		fSprite.setTextureRect(sf::IntRect(105, 102, 140, 46));
		setPosition(WIDTH / 2.f - fSprite.getGlobalBounds().width / 2.f, 0.f);
		fBoundingRect = sf::FloatRect(17, 20, 106, 6);
		fBorderBoundingLeft = sf::FloatRect(fBoundingRect.left, fBoundingRect.top, 5.f, fBoundingRect.height);
		fBorderBoundingRight = sf::FloatRect(fBoundingRect.left + fBoundingRect.width - 5.f, fBoundingRect.top, 5.f, fBoundingRect.height);
		
	}
	break;
	case Player::Blue:
	{

		sf::Texture& tex = textures.get(Textures::ID::Paddles_and_Ball);
		fSprite.setTexture(tex);
		fSprite.setTextureRect(sf::IntRect(52, 5, 46, 140));
		setPosition(0.f, HEIGHT / 2.f - fSprite.getGlobalBounds().height / 2.f);
		fBoundingRect = sf::FloatRect(20, 17, 6, 106);
		fBorderBoundingLeft = sf::FloatRect(fBoundingRect.left, fBoundingRect.top, fBoundingRect.width, 5.f);
		fBorderBoundingRight = sf::FloatRect(fBoundingRect.left, fBoundingRect.top + fBoundingRect.height - 5.f, fBoundingRect.width, 5.f);
		
	}
	break;
	case Player::Red:
	{

		sf::Texture& tex = textures.get(Textures::ID::Paddles_and_Ball);
		fSprite.setTexture(tex);
		fSprite.setTextureRect(sf::IntRect(2, 5, 46, 140));
		setPosition(WIDTH - fSprite.getGlobalBounds().width, HEIGHT / 2.f - fSprite.getGlobalBounds().height / 2.f);
		fBoundingRect = sf::FloatRect(20, 17, 6, 106);
		fBorderBoundingLeft = sf::FloatRect(fBoundingRect.left, fBoundingRect.top, fBoundingRect.width, 5.f);
		fBorderBoundingRight = sf::FloatRect(fBoundingRect.left, fBoundingRect.top + fBoundingRect.height - 5.f, fBoundingRect.width, 5.f);
		
	}
	break;
	default:
		throw std::logic_error("the player type specified doesn't exist");
		break;
	}
}

bool Player::isNotOutside() const
{
	switch (fType)
	{
	case Player::Green:
	case Player::Purple:
		if (getBoundingRect().left <= 0.f || getBoundingRect().left + getBoundingRect().width >= WIDTH)
			return false;
		break;
	case Player::Blue:
	case Player::Red:
		if (getBoundingRect().top <= 0.f || getBoundingRect().top + getBoundingRect().height >= HEIGHT)
			return false;
		break;
	default:
		break;
	}

	return true;
}
