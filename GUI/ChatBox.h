#pragma once
#include "Button.h"

class ChatBox : public sf::Drawable, public sf::Transformable
{
public:
	ChatBox(sf::RenderWindow& window, TexturesHolder& textures, SoundBuffersHolder& sounds, FontsHolder& fonts);
	
	void update(sf::Time dt);
	void handleEvent(const sf::Event& e);

	std::string getText() const;
	void setText(const std::string& str);

	void setVolume(float v);
	bool fHasFinishedEnteringSmth = false;
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::RectangleShape fBox;
	sf::RectangleShape fFlashingLine;
	sf::Time fFlashingDuration = sf::seconds(0.5f);
	sf::Time fFlashingAccu = sf::Time::Zero;
	bool fShowFlashingLine = true;

	sf::Text fVisualText;
	sf::Text fErrorText;
	bool fShowErrorText = false;
	Button enterButton;
	sf::RenderWindow& fWindow;

	sf::Sound fTypingSound;
	sf::Sound fSavingSound;
};