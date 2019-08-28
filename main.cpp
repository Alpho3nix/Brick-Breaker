#include "BrickPongApp.h"
#include <iostream>

int main()
{
	try {
		BrickPongApp brickPongApp;
		brickPongApp.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		system("pause");
		return 1;
	}
	return 0;
}
