// x52-mfd.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "stdafx.h"


#include "X52.h"


int main()
{
	try
	{
		auto x52 = X52::X52();

		x52.init();
		//x52.test();

		int _x;
		std::cin >> _x;

	}
	catch(std::runtime_error e){
		std::cout << "caught exception: " << e.what() << std::endl;
	}

}


