#include "Entity.h"

Entity::Entity()
	:fSprite(), fBoundingRect()
{
}

sf::FloatRect Entity::getBoundingRect() const
{
	return getTransform().transformRect(fBoundingRect);
}

sf::FloatRect Entity::getSpriteBoundingRect() const
{
	return getTransform().transformRect(fSprite.getGlobalBounds());
}

void Entity::handleEvent(const sf::Event& e)
{
}

void Entity::realTimeInput(sf::Time dt)
{
}

void Entity::update(sf::Time dt)
{
}

bool Entity::isDead() const
{
	return fIsDead;
}

void Entity::setDead(bool flag)
{
	fIsDead = flag;
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(fSprite, states);
	
	//debugging purposes only
	//drawBoundingRect(target, states);
}

void Entity::drawBoundingRect(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::RectangleShape rect;
	rect.setPosition(getBoundingRect().left, getBoundingRect().top);
	rect.setSize(sf::Vector2f(getBoundingRect().width, getBoundingRect().height));
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineColor(sf::Color::White);
	rect.setOutlineThickness(1.f);

	target.draw(rect);
}
