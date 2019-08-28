#pragma once
#include "ResourceHolder.hpp"

namespace Fonts
{
	enum class ID
	{
		Future, Future_Thin
	};
}

namespace Textures
{
	enum class ID
	{
		Paddles_and_Ball, Bricks_andOthers, Background, UI, PlayerLifes
	};
}

namespace SoundBuffers
{
	enum class ID
	{
		BallTouchBrick, BallTouchPaddle, BallTouchSide, PlayerLose, ButtonPressed, ButtonReleased, TypingSound, HeartPickup, SpeedPickup
	};
}

typedef ResourceHolder<sf::Font, Fonts::ID> FontsHolder;
typedef ResourceHolder<sf::Texture, Textures::ID> TexturesHolder;
typedef ResourceHolder<sf::SoundBuffer, SoundBuffers::ID> SoundBuffersHolder;