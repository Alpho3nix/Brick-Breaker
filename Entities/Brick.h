#pragma once
#include "Entity.h"
class PickupItem;

class Brick : public Entity
{
public:
	enum Type
	{
		BreakableOneShot, BreakableTwoShot, NotBreakable
	};
	Brick(TexturesHolder& textures, SoundBuffersHolder& sounds, Type type, std::vector<PickupItem*>& items, bool rotated = false);

	void applyDamage();

	void mySetPosition(sf::Vector2f pos);
	void mySetPosition(float x, float y);

	sf::Vector2f myGetPosition() const;

private:

	enum Color
	{
		LightGreen = 0, DarkGreen, LightBlue, DarkBlue, Red, Purple, Orange, Yellow, Brown, Gray, CountOneShot,
		lightGreen = 0, LightGreenBroken = 1, darkGreen = 2, DarkGreenBroken = 3, lightBlue = 4, LightBlueBroken = 5,
		darkBlue = 6, DarkBlueBroken = 7, red = 8, RedBroken = 9, purple = 10,
		PurpleBroken = 11, orange = 12, OrangeBroken = 13, yellow = 14, YellowBroken = 15,
		brown = 16, BrownBroken = 17, gray = 18, GrayBroken = 19, CountTwoShot = 20
	};

	void handleType(TexturesHolder& textures, Type type);
	void changeTexture(bool brokenTex);

	Type fType;
	Color fColor;
	std::vector<sf::IntRect> fBreakableOneShotRects;
	std::vector<sf::IntRect> fBreakableTwoShotRects;

	std::vector<PickupItem*>& fItems;
	bool fBroken = false;
	bool fRotated;

	TexturesHolder& fTextures;
	SoundBuffersHolder& fSounds;
};