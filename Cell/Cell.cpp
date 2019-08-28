#include "Cell.h"
#include "Brick.h"

Cell::Cell(int width, int height)
	:size(width, height), fNeighbors(FullNeighborCount), fBrick(nullptr), fType(Type::None)
{
	for (int i = 0; i < FullNeighborCount; ++i)
	{
		fNeighbors[i] = nullptr;
	}
	for (int i = 0; i < SideCount; ++i)
	{
		fSides[i] = nullptr;
	}
}

Cell::Cell(sf::Vector2i size)
	: size(size), fNeighbors(FullNeighborCount), fBrick(nullptr), fType(Type::None)
{
	for (int i = 0; i < FullNeighborCount; ++i)
	{
		fNeighbors[i] = nullptr;
	}
	for (int i = 0; i < SideCount; ++i)
	{
		fSides[i] = nullptr;
	}
}

sf::Vector2i Cell::getSize() const
{
	return size;
}

bool Cell::isEmpty() const
{
	return fBrick == nullptr;
}

void Cell::setBrick(Brick* brick)
{
	fBrick = brick;
}

Brick* Cell::getBrick() const
{
	return fBrick;
}

void Cell::updateCell(std::vector<Side*>& sides)
{
	//if there's no brick in this cell, no need to check anything
	if (isEmpty() || (fNeighbors[North] && !fNeighbors[North]->isEmpty() && fNeighbors[South] && !fNeighbors[South]->isEmpty()
		&& fNeighbors[West] && !fNeighbors[West]->isEmpty() && fNeighbors[East] && !fNeighbors[East]->isEmpty()))
	{
		return;
	}

	updateEachNeighbors(sides);

}

void Cell::setPosition(float x, float y)
{
	fPosition = sf::Vector2f(x, y);
}

void Cell::setPosition(sf::Vector2f pos)
{
	fPosition = pos;
}

sf::Vector2f Cell::getPosition() const
{
	return fPosition;
}

sf::FloatRect Cell::getBoundingRect() const
{
	return sf::FloatRect(getPosition(), sf::Vector2f(size));
}


void Cell::updateEachNeighbors(std::vector<Side*>& sides)
{
	for (int i = 0; i < NeighborCount; ++i)
	{
		
		if ((fNeighbors[i] && fNeighbors[i]->isEmpty()) || !fNeighbors[i]) {	//gotta check if it's not a nullptr
			isNeighborNotBrick = true;
			switch (i)	//sides
			{
			case West:
				if (fNeighbors[North] && fNeighbors[North]->fSides[Left])	//if the north neighbor already has a side created
				{
					//then extend that side
					fSides[Left] = fNeighbors[North]->fSides[Left];
					if (fSides[Left]->fB == sf::Vector2f(fBrick->myGetPosition().x, fBrick->myGetPosition().y))	//fB is the point we want to extend
					{
						fSides[Left]->fB = sf::Vector2f(fBrick->myGetPosition().x, fBrick->myGetPosition().y + fBrick->getBoundingRect().height);
					}
					else	//fA is the point we want to extent
					{
						fSides[Left]->fA = sf::Vector2f(fBrick->myGetPosition().x, fBrick->myGetPosition().y + fBrick->getBoundingRect().height);
					}
				}
				else	//else we have to create a new side
				{
					sides.push_back(new Side(sf::Vector2f(fBrick->myGetPosition().x, fBrick->myGetPosition().y),
						sf::Vector2f(fBrick->myGetPosition().x, fBrick->myGetPosition().y + fBrick->getBoundingRect().height), true));
					fSides[Left] = sides[sides.size() - 1];
				}
				break;
			case South:
				//same as before but for the bottom side
				if (fNeighbors[West] && fNeighbors[West]->fSides[Bottom])
				{
					fSides[Bottom] = fNeighbors[West]->fSides[Bottom];
					if (fSides[Bottom]->fB == sf::Vector2f(fBrick->myGetPosition().x, fBrick->myGetPosition().y + fBrick->getBoundingRect().height))	//fB is the point we want to extend
					{
						fSides[Bottom]->fB = sf::Vector2f(fBrick->myGetPosition().x + fBrick->getBoundingRect().width, fBrick->myGetPosition().y + fBrick->getBoundingRect().height);
					}
					else	//fA is the point we want to extent
					{
						fSides[Bottom]->fA = sf::Vector2f(fBrick->myGetPosition().x + fBrick->getBoundingRect().width, fBrick->myGetPosition().y + fBrick->getBoundingRect().height);
					}
				}
				else
				{
					sides.push_back(new Side(sf::Vector2f(fBrick->myGetPosition().x, fBrick->myGetPosition().y + fBrick->getBoundingRect().height),
						sf::Vector2f(fBrick->myGetPosition().x + fBrick->getBoundingRect().width, fBrick->myGetPosition().y + fBrick->getBoundingRect().height), true));
					fSides[Bottom] = sides[sides.size() - 1];
				}
				break;
			case East:
				if (fNeighbors[North] && fNeighbors[North]->fSides[Right])	//if the north neighbor already has a side created
				{
					//then extend that side
					fSides[Right] = fNeighbors[North]->fSides[Right];
					if (fSides[Right]->fB == sf::Vector2f(fBrick->myGetPosition().x + fBrick->getBoundingRect().width, fBrick->myGetPosition().y))	//fB is the point we want to extend
					{
						fSides[Right]->fB = sf::Vector2f(fBrick->myGetPosition().x + fBrick->getBoundingRect().width, fBrick->myGetPosition().y + fBrick->getBoundingRect().height);
					}
					else	//fA is the point we want to extent
					{
						fSides[Right]->fA = sf::Vector2f(fBrick->myGetPosition().x + fBrick->getBoundingRect().width, fBrick->myGetPosition().y + fBrick->getBoundingRect().height);
					}
				}
				else	//else we have to create a new side
				{
					sides.push_back(new Side(sf::Vector2f(fBrick->myGetPosition().x + fBrick->getBoundingRect().width, fBrick->myGetPosition().y),
						sf::Vector2f(fBrick->myGetPosition().x + fBrick->getBoundingRect().width, fBrick->myGetPosition().y + fBrick->getBoundingRect().height), true));
					fSides[Right] = sides[sides.size() - 1];
				}
				break;
			case North:
				if (fNeighbors[West] && fNeighbors[West]->fSides[Top])
				{
					fSides[Top] = fNeighbors[West]->fSides[Top];
					if (fSides[Top]->fB == sf::Vector2f(fBrick->myGetPosition().x, fBrick->myGetPosition().y))	//fB is the point we want to extend
					{
						fSides[Top]->fB = sf::Vector2f(fBrick->myGetPosition().x + fBrick->getBoundingRect().width, fBrick->myGetPosition().y);
					}
					else	//fA is the point we want to extent
					{
						fSides[Top]->fA = sf::Vector2f(fBrick->myGetPosition().x + fBrick->getBoundingRect().width, fBrick->myGetPosition().y);
					}
				}
				else
				{
					sides.push_back(new Side(sf::Vector2f(fBrick->myGetPosition().x, fBrick->myGetPosition().y),
						sf::Vector2f(fBrick->myGetPosition().x + fBrick->getBoundingRect().width, fBrick->myGetPosition().y), true));
					fSides[Top] = sides[sides.size() - 1];
				}
				break;
			default:
				break;
			}

		}
		
		
	}
}
