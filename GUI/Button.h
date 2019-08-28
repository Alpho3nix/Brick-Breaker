#pragma once
#include "ResourceIdentifiers.h"
#include "SFML/Audio.hpp"
#include <functional>

class Button : public sf::Drawable, public sf::Transformable
{
public:
	enum class Type {
		Empty, Brick, Paddle, Ball, CheckMark, SpeedBoost, Heart
	};
	enum class Color
	{
		Yellow, Blue, Green, White, Orange
	};
	Button(TexturesHolder& textures, SoundBuffersHolder& soundBuffers, FontsHolder& fonts, Type type, Color color = Color::White, const std::string& label = "", unsigned int charSize = 30U);

	sf::FloatRect getBoundingRect() const;

	void clickButton();
	void unclickButton();

	void playClickSound();
	void playUnclickSound();
	void changeTexture(bool pressed);

	void setVolume(float v);

	void update(sf::Time dt);

	//true == pressed, false == released
	bool currentTexture() const;

	void changePaddleButton(unsigned int type);
	void setColor(sf::Color color);
	
	void setSprite(sf::Sprite customSprite, sf::IntRect pressedTexRect, sf::IntRect normalTexRect);

	void setCanChangeTexture(bool flag);
	bool canChangeTexture() const;

	std::function<void()> fCallbackFunc;
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	
	void handleType(TexturesHolder& textures);

	Type fType;
	Color fColor;
	sf::Sprite fSprite;
	bool fDrawAdditionalSprite = false;
	sf::Sprite fSpriteOverButton;
	sf::IntRect fPressedTextureRect;
	sf::IntRect fNormalTextureRect;

	bool fDrawLabel;
	sf::Text fLabel;

	sf::Sound fPressedSound;
	sf::Sound fReleasedSound;

	bool fCanChangeTexture;

};