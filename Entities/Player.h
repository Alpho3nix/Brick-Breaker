#pragma once
#include "Entity.h"


class Player : public Entity
{
public:

	enum Type
	{
		Green, Purple, Blue, Red, Count
	};

	Player(sf::RenderWindow& window, TexturesHolder& textures, Type type, float paddleVelocity = 100.f);

	void setType(Type newType);
	Type getType() const;

	void handleEvent(const sf::Event& e) override;
	void realTimeInput(sf::Time dt) override;
	void update(sf::Time dt) override;

	void takeLife();
	void addLife();

	int getLifes() const;

	void setSpeed(float speed);
	float getSpeed() const;

	sf::FloatRect getBorderBoundingLeft() const;
	sf::FloatRect getBorderBoundingRight() const;

private:

	void handleType(Type type, TexturesHolder& textures);

	Type fType;
	bool isNotOutside() const;

	sf::Vector2f fPaddleVelocity;
	float fPaddleSpeed;

	sf::FloatRect fBorderBoundingLeft, fBorderBoundingRight;
	sf::RenderWindow& fWindow;

	int fLife;
};