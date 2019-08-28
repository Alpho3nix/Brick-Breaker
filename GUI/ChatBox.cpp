#include "ChatBox.h"
#include "Entity.h"

ChatBox::ChatBox(sf::RenderWindow& window, TexturesHolder& textures, SoundBuffersHolder& sounds, FontsHolder& fonts)
	:enterButton(textures, sounds, fonts, Button::Type::CheckMark), fBox(sf::Vector2f(201.f, 36.f)), fFlashingLine(sf::Vector2f(1.f, 36.f)),
	fVisualText("", fonts.get(Fonts::ID::Future), 20U), fWindow(window), fErrorText("10 charaters max", fonts.get(Fonts::ID::Future), 10U),
	fSavingSound(sounds.get(SoundBuffers::ID::ButtonPressed)), fTypingSound(sounds.get(SoundBuffers::ID::TypingSound))
{
	fErrorText.setFillColor(sf::Color::Red);
	fVisualText.setFillColor(sf::Color::Black);
	enterButton.fCallbackFunc = [this]()
	{
		std::string name = fVisualText.getString();
		if (name.size() > 10)
		{
			fShowErrorText = true;
		}
		else
		{
			fShowErrorText = false;
			fHasFinishedEnteringSmth = true;
			fSavingSound.play();
		}
	};
	enterButton.setPosition(fBox.getGlobalBounds().width + 10.f, 0.f);
	fErrorText.setPosition(0.f, -10.f);
	fFlashingLine.setPosition(1.f, 0.f);
	enterButton.setCanChangeTexture(false);
}

void ChatBox::update(sf::Time dt)
{
	fFlashingLine.setPosition(fVisualText.getLocalBounds().width, 0.f);
	fFlashingAccu += dt;

	if (fFlashingAccu >= fFlashingDuration)
	{
		fFlashingAccu = sf::Time::Zero;
		fShowFlashingLine = !fShowFlashingLine;
	}
}

void ChatBox::handleEvent(const sf::Event& e)
{
	if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Button::Left)
	{
		if (getTransform().transformRect(enterButton.getBoundingRect()).contains(sf::Vector2f(sf::Mouse::getPosition(fWindow))))
		{
			enterButton.clickButton();
		}
	}
	else if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Enter)
	{
		enterButton.clickButton();
	}
	else if (e.type == sf::Event::TextEntered)
	{
		char c = e.text.unicode;
		if (std::isalnum(c)) {
			fVisualText.setString(fVisualText.getString() + c);
		}
		else if (c == 8 && fVisualText.getString().getSize() > 0)
		{
			std::string s = fVisualText.getString();
			s.erase(s.end() - 1);
			fVisualText.setString(s);
		}
		fTypingSound.play();
	}
	
}

std::string ChatBox::getText() const
{
	if (fHasFinishedEnteringSmth)
		return fVisualText.getString();
	else
		return "";
}

void ChatBox::setText(const std::string& str)
{
	fVisualText.setString(str);
}

void ChatBox::setVolume(float v)
{
	fSavingSound.setVolume(v);
	fTypingSound.setVolume(v);
}


void ChatBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(fBox, states);
	target.draw(enterButton, states);
	target.draw(fVisualText, states);
	if (fShowErrorText)
		target.draw(fErrorText, states);
	if(fShowFlashingLine)
		target.draw(fFlashingLine, states);
}
