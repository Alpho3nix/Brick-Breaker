#pragma once
#include <math.h>
#include "SFML/Graphics.hpp"
#include <iostream>
#include <random>

namespace
{
	std::default_random_engine createRandomEngine()
	{
		auto seed = static_cast<unsigned long>(std::time(nullptr));
		return std::default_random_engine(seed);
	}

	auto RandomEngine = createRandomEngine();
}

class Utility {
public:

	enum OutSide
	{
		None, Left, Right, Top, Bottom
	};

	static float square(float x) { return x * x; }

	template<typename T>
	static void CenterOrigin(T& thing)
	{
		thing.setOrigin(thing.getLocalBounds().left + thing.getLocalBounds().width / 2.f, thing.getLocalBounds().top + thing.getLocalBounds().height / 2.f);
	}

	static float length(const sf::Vector2f& vec)
	{
		return sqrt(square(vec.x) + square(vec.y));
	}

	static float getDistance(const sf::Vector2f& a, const sf::Vector2f& b)
	{
		return length(b - a);
	}

	static int randomInt(int exclusiveMax)
	{
		std::uniform_int_distribution<> distr(0, exclusiveMax - 1);
		return distr(RandomEngine);
	}

	static int randomInt(int min, int max)
	{
		std::uniform_int_distribution<> distr(min, max);
		return distr(RandomEngine);
	}

	static float randomFloat(float Max)
	{
		std::uniform_real_distribution<> distr(0.f, Max);
		return float(distr(RandomEngine));
	}

	static double randomFloat(double min, double max)
	{
		std::uniform_real_distribution<> distr(min, max);
		return distr(RandomEngine);
	}

	static OutSide isOutOfTheRect(const sf::FloatRect & pos, const sf::FloatRect & rect)
	{
		if (pos.left < rect.left)
			return Left;
		if (pos.top < rect.top)
			return Top;
		if (pos.left + pos.width > rect.left + rect.width)
			return Right;
		if (pos.top + pos.height > rect.top + rect.height) {
			return Bottom;
		}

		return None;
	}
};
