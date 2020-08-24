// x52-mfd.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "stdafx.h"

#include "X52.h"
#include "X52Device.h"

void select_callback_func(std::tuple<int, int> val)
{
	LOG_DEBUG("Select on Page-Id " + std::to_string(std::get<0>(val)) + " on item " + std::to_string(std::get<1>(val)));
}

int main()
{
	try
	{
		auto x52 = X52::X52();

		x52.init();
		X52Device* device = x52.x52devices[0];
		device->setSelectCallback(select_callback_func);

		std::string _x;
		std::cin >> _x;

	}
	catch(std::runtime_error e){
		std::cout << "caught exception: " << e.what() << std::endl;
	}

}
