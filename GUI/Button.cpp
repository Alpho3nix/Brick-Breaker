#include "Button.h"
#include "Player.h"
#include "UtilityFunc.h"

Button::Button(TexturesHolder& textures, SoundBuffersHolder& soundBuffers, FontsHolder& fonts, Type type, Color color, const std::string& label, unsigned int charSize)
	:fType(type), fLabel(label, fonts.get(Fonts::ID::Future_Thin), charSize), fSprite(), fDrawLabel(false), fColor(color),
	fPressedSound(soundBuffers.get(SoundBuffers::ID::ButtonPressed)), fReleasedSound(soundBuffers.get(SoundBuffers::ID::ButtonReleased)), fCanChangeTexture(true)
{
	handleType(textures);
}

sf::FloatRect Button::getBoundingRect() const
{
	return getTransform().transformRect(fSprite.getGlobalBounds());
}

void Button::clickButton()
{
	if (fCanChangeTexture)
	{
		if (fType == Type::CheckMark)
		{
			changeTexture(fSprite.getTextureRect() == fNormalTextureRect);
			fCallbackFunc();
			fPressedSound.play();
		}
		else {
			fSprite.setTextureRect(fPressedTextureRect);
			fCallbackFunc();
			fPressedSound.play();
		}
	}
	else
	{
		fCallbackFunc();
		fPressedSound.play();
	}
}

void Button::unclickButton()
{
	fSprite.setTextureRect(fNormalTextureRect);
	//fReleasedSound.play();
}

void Button::playClickSound()
{
	fPressedSound.play();
}

void Button::playUnclickSound()
{
	fReleasedSound.play();
}

void Button::changeTexture(bool pressed)
{
	if (pressed)
	{
		fSprite.setTextureRect(fPressedTextureRect);
	}
	else
	{
		fSprite.setTextureRect(fNormalTextureRect);
	}
}

void Button::setVolume(float v)
{
	fPressedSound.setVolume(v);
	fReleasedSound.setVolume(v);
}

void Button::update(sf::Time dt)
{
}

bool Button::currentTexture() const
{
	return fSprite.getTextureRect() == fPressedTextureRect;
}

void Button::changePaddleButton(unsigned int type)
{
	switch (type)
	{
	case Player::Green:
		fSpriteOverButton.setTextureRect(sf::IntRect(105, 52, 140, 46));
		Utility::CenterOrigin(fSpriteOverButton);
		fSpriteOverButton.setPosition(fNormalTextureRect.width / 2.f, fNormalTextureRect.height / 2.f);
		fSpriteOverButton.setRotation(0.f);
		break;
	case Player::Purple:
		fSpriteOverButton.setTextureRect(sf::IntRect(105, 102, 140, 46));
		Utility::CenterOrigin(fSpriteOverButton);
		fSpriteOverButton.setPosition(fNormalTextureRect.width / 2.f, fNormalTextureRect.height / 2.f);
		fSpriteOverButton.setRotation(0.f);
		break;
	case Player::Blue:
		fSpriteOverButton.setTextureRect(sf::IntRect(52, 5, 46, 140));
		Utility::CenterOrigin(fSpriteOverButton);
		fSpriteOverButton.setPosition(fNormalTextureRect.width / 2.f, fNormalTextureRect.height / 2.f);
		fSpriteOverButton.setRotation(90.f);
		break;
	case Player::Red:
		fSpriteOverButton.setTextureRect(sf::IntRect(2, 5, 46, 140));
		Utility::CenterOrigin(fSpriteOverButton);
		fSpriteOverButton.setPosition(fNormalTextureRect.width / 2.f, fNormalTextureRect.height / 2.f);
		fSpriteOverButton.setRotation(90.f);
		break;
	default:
		break;
	}
}

void Button::setColor(sf::Color color)
{
	fSprite.setColor(color);
}

void Button::setSprite(sf::Sprite customSprite, sf::IntRect pressedTexRect, sf::IntRect normalTexRect)
{
	fSprite = customSprite;
	fNormalTextureRect = normalTexRect;
	fPressedTextureRect = pressedTexRect;
}

void Button::setCanChangeTexture(bool flag)
{
	fCanChangeTexture = flag;
}

bool Button::canChangeTexture() const
{
	return fCanChangeTexture;
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(fSprite, states);
	if (fDrawAdditionalSprite)
	{
		target.draw(fSpriteOverButton, states);
	}
	if (fDrawLabel)
	{
		target.draw(fLabel, states);
	}
}

void Button::handleType(TexturesHolder& textures)
{

	sf::Texture& tex = textures.get(Textures::ID::UI);
	fSprite.setTexture(tex);
	switch (fType)
	{
	case Button::Type::Empty:
	{
		fDrawLabel = true;

		switch (fColor)
		{
		case Button::Color::Yellow:
		{
			fNormalTextureRect = sf::IntRect(190, 46, 190, 49);
			fPressedTextureRect = sf::IntRect(0, 49, 190, 45);
		}
		break;
		case Button::Color::Blue:
		{
			fNormalTextureRect = sf::IntRect(0, 350, 190, 49);
			fPressedTextureRect = sf::IntRect(0, 305, 190, 45);
		}
		break;
		case Button::Color::Green:
		{
			fNormalTextureRect = sf::IntRect(0, 512, 190, 49);
			fPressedTextureRect = sf::IntRect(0, 704, 190, 45);
		}
		break;
		case Button::Color::White:
		{
			fNormalTextureRect = sf::IntRect(512, 237, 190, 49);
			fPressedTextureRect = sf::IntRect(512, 98, 190, 45);
		}
		break;
		case Button::Color::Orange:
		{
			fNormalTextureRect = sf::IntRect(702, 512, 190, 49);
			fPressedTextureRect = sf::IntRect(512, 512, 190, 45);
		}
		break;
		default:
			break;
		}

	}
		break;
	case Button::Type::Brick:
		fNormalTextureRect = sf::IntRect(512, 700, 190, 45);
		fPressedTextureRect = sf::IntRect(702, 561, 190, 49);

		break;
	case Button::Type::Paddle:
		fNormalTextureRect = sf::IntRect(512, 143, 190, 45);
		fPressedTextureRect = sf::IntRect(512, 143, 190, 45);

		fSpriteOverButton.setTexture(textures.get(Textures::ID::Paddles_and_Ball));
		fSpriteOverButton.setTextureRect(sf::IntRect(105, 52, 140, 46));
		Utility::CenterOrigin(fSpriteOverButton);
		fSpriteOverButton.setPosition(fNormalTextureRect.width / 2.f, fNormalTextureRect.height / 2.f);

		fDrawAdditionalSprite = true;
		break;
	case Button::Type::Ball:
		fNormalTextureRect = sf::IntRect(0, 561, 190, 45);
		fPressedTextureRect = sf::IntRect(0, 655, 190, 49);

		fSpriteOverButton.setTexture(textures.get(Textures::ID::Paddles_and_Ball));
		fSpriteOverButton.setTextureRect(sf::IntRect(98, -2, 54, 54));
		Utility::CenterOrigin(fSpriteOverButton);
		fSpriteOverButton.setPosition(fNormalTextureRect.width / 2.f, fNormalTextureRect.height / 2.f);
		fDrawAdditionalSprite = true;
		break;
	case Button::Type::CheckMark:
		fNormalTextureRect = sf::IntRect(660, 469, 36, 36);
		fPressedTextureRect = sf::IntRect(660, 433, 36, 36);
		break;
	case Button::Type::Heart:
		fNormalTextureRect = sf::IntRect(190, 305, 190, 45);
		fPressedTextureRect = sf::IntRect(0, 399, 190, 49);

		fSpriteOverButton.setTexture(textures.get(Textures::ID::PlayerLifes));
		fSpriteOverButton.setTextureRect(sf::IntRect(0, 40, 23, 20));
		Utility::CenterOrigin(fSpriteOverButton);
		fSpriteOverButton.setPosition(fNormalTextureRect.width / 2.f, fNormalTextureRect.height / 2.f);
		fDrawAdditionalSprite = true;
		break;
	case Button::Type::SpeedBoost:
		fNormalTextureRect = sf::IntRect(190, 305, 190, 45);
		fPressedTextureRect = sf::IntRect(0, 399, 190, 49);

		fSpriteOverButton.setTexture(textures.get(Textures::ID::Bricks_andOthers));
		fSpriteOverButton.setTextureRect(sf::IntRect(257, 282, 22, 21));
		Utility::CenterOrigin(fSpriteOverButton);
		fSpriteOverButton.setPosition(fNormalTextureRect.width / 2.f, fNormalTextureRect.height / 2.f);
		fDrawAdditionalSprite = true;
		break;
	default:
		break;
	}

	fSprite.setTextureRect(fNormalTextureRect);

	Utility::CenterOrigin(fLabel);
	fLabel.setPosition(fSprite.getLocalBounds().width / 2.f, fSprite.getLocalBounds().height / 2.f);
}
