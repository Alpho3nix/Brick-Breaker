#include "Brick.h"
#include "UtilityFunc.h"
#include "PickupItem.h"

Brick::Brick(TexturesHolder& textures, SoundBuffersHolder& sounds, Type type, std::vector<PickupItem*>& items, bool rotated)
	:fType(type), fBreakableOneShotRects(CountOneShot), fBreakableTwoShotRects(CountTwoShot), fRotated(rotated), fItems(items)
	, fTextures(textures), fSounds(sounds)
{
	fBreakableOneShotRects =
	{
		sf::IntRect(468, 44, 43, 43),
		sf::IntRect(511, 84, 43, 43),
		sf::IntRect(511, 44, 43, 43),
		sf::IntRect(511, 131, 43, 43),
		sf::IntRect(468, 131, 43, 43),
		sf::IntRect(468, 84, 43, 43),
		sf::IntRect(468, 174, 43, 43),
		sf::IntRect(502, 218, 43, 43),
		sf::IntRect(511, 174, 43, 43),
		sf::IntRect(525, 261, 43, 43)
	};
	fBreakableTwoShotRects =
	{
		sf::IntRect(0, 43, 128, 43), sf::IntRect(0, 86, 128, 43),
		sf::IntRect(129, 43, 128, 43), sf::IntRect(129, 0, 128, 43),
		sf::IntRect(129, 217, 128, 43), sf::IntRect(129, 174, 128, 43),
		sf::IntRect(257, 130, 128, 43), sf::IntRect(0, 0, 128, 43),
		sf::IntRect(257, 87, 128, 43), sf::IntRect(257, 43, 128, 43),
		sf::IntRect(0, 130, 128, 43), sf::IntRect(0, 173, 128, 43),
		sf::IntRect(257, 0, 128, 43), sf::IntRect(257, 217, 128, 43),
		sf::IntRect(129, 130, 128, 43), sf::IntRect(129, 87, 128, 43),
		sf::IntRect(129, 260, 128, 43), sf::IntRect(0, 260, 128, 43),
		sf::IntRect(257, 173, 128, 43), sf::IntRect(0, 217, 128, 43)
	};


	handleType(textures, type);
	fBoundingRect = fSprite.getGlobalBounds();

	if (fRotated)
	{
		setRotation(90.f);
	}
}

void Brick::applyDamage()
{
	switch (fType)
	{
	case Brick::BreakableOneShot:
		fIsDead = true;
		break;
	case Brick::BreakableTwoShot:
		if (!fBroken) {
			fBroken = true;
			changeTexture(fBroken);
		}
		else
		{
			if (fColor == red)
			{
				PickupItem* item = new PickupItem(fTextures, fSounds, PickupItem::Heart);
				item->setPosition(myGetPosition().x + getBoundingRect().width / 2.f, myGetPosition().y + getBoundingRect().height / 2.f);
				fItems.push_back(item);
			}
			else if (fColor == yellow)
			{
				PickupItem* item = new PickupItem(fTextures, fSounds, PickupItem::SpeedBoost);
				item->setPosition(myGetPosition().x + getBoundingRect().width / 2.f, myGetPosition().y + getBoundingRect().height / 2.f);
				fItems.push_back(item);
			}
			fIsDead = true;
		}
		break;
	case Brick::NotBreakable:
		break;
	default:
		break;
	}
}

void Brick::mySetPosition(sf::Vector2f pos)
{
	if (fRotated)
	{
		setPosition(pos.x + fBoundingRect.height, pos.y);
	}
	else
	{
		setPosition(pos);
	}
}

void Brick::mySetPosition(float x, float y)
{
	if (fRotated)
	{
		setPosition(x + fBoundingRect.height, y);
	}
	else
	{
		setPosition(x, y);
	}
}

sf::Vector2f Brick::myGetPosition() const
{
	if (fRotated)
	{
		return sf::Vector2f(getPosition().x - fBoundingRect.height, getPosition().y);
	}

	return getPosition();
}

void Brick::handleType(TexturesHolder& textures, Type type)
{
	fSprite.setTexture(textures.get(Textures::ID::Bricks_andOthers));

	switch (type)
	{
	case Brick::BreakableOneShot:
	{
		int r = Utility::randomInt(CountOneShot);
		fSprite.setTextureRect(fBreakableOneShotRects[r]);
		fColor = static_cast<Color>(r);
	}
		break;
	case Brick::BreakableTwoShot:
	{

		int r = Utility::randomInt(CountTwoShot);
		if (r % 2 != 0)
			r--;

		fSprite.setTextureRect(fBreakableTwoShotRects[r]);
		fColor = static_cast<Color>(r);
	}
		break;
	case Brick::NotBreakable:

		fSprite.setTextureRect(fBreakableTwoShotRects[gray]);
		fColor = gray;
		break;
	default:
		break;
	}
}

void Brick::changeTexture(bool brokenTex)
{
	if (brokenTex)
	{
		fSprite.setTextureRect(fBreakableTwoShotRects[fColor + 1]);
	}
	else
	{
		fSprite.setTextureRect(fBreakableTwoShotRects[fColor]);
	}
}
