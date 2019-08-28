#pragma once
#include <vector>
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/VertexArray.hpp"

class Brick;

struct Side
{
	Side(sf::Vector2f a, sf::Vector2f b, bool enable)
		:fA(a), fB(b), fEnable(enable)
	{}
	Side()
		:fA(0.f, 0.f), fB(0.f, 0.f), fEnable(false)
	{}

	sf::Vector2f fA;
	sf::Vector2f fB;
	bool fEnable;
};

class Cell
{
public:

	enum Neighbor
	{
		West, South, East, North, NeighborCount,
		NorthWest = 4, NorthEast, SouthWest, SouthEast, FullNeighborCount
	};
	enum class Type
	{
		None, BorderLeft, BorderTop, BorderRight, BorderBottom, CornerTL, CornerTR, CornerBR, CornerBL, Middle
	};
	enum Sides
	{
		Left, Bottom, Right, Top, SideCount
	};

	Cell(int width, int height);
	Cell(sf::Vector2i size);

	sf::Vector2i getSize() const;
	bool isEmpty() const;

	void setBrick(Brick* brick);
	Brick* getBrick() const;

	void updateCell(std::vector<Side*>& sides);

	Type fType;

	void setPosition(float x, float y);
	void setPosition(sf::Vector2f pos);

	sf::Vector2f getPosition() const;
	sf::FloatRect getBoundingRect() const;

	std::vector<Cell*> fNeighbors;
	Side* fSides[SideCount];


private:
	sf::Vector2i size;

	sf::Vector2f fPosition;
	Brick* fBrick;


	bool isNeighborNotBrick = false;
	void updateEachNeighbors(std::vector<Side*>& sides);

};