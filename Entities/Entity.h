#pragma once
#include "ResourceIdentifiers.h"

const unsigned int WIDTH = 1152;
const unsigned int HEIGHT = 860;
static bool fEnableMusic = true;
static bool fEnableSoundEffects = true;

class Entity : public sf::Drawable, public sf::Transformable
{
public:
	Entity();

	virtual sf::FloatRect getBoundingRect() const;
	sf::FloatRect getSpriteBoundingRect() const;
	virtual void handleEvent(const sf::Event& e);
	virtual void realTimeInput(sf::Time dt);

	virtual void update(sf::Time dt);

	virtual bool isDead() const;
	void setDead(bool flag);

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void drawBoundingRect(sf::RenderTarget& target, sf::RenderStates states) const;

protected:
	sf::Sprite fSprite;
	sf::FloatRect fBoundingRect;

	bool fIsDead = false;
};