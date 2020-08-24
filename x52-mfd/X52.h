#pragma once

#include "stdafx.h"
#include "X52Device.h"


class X52
{
public:
	X52();
	~X52();
	void init();
	void test();

	std::vector<void*> devices;

private:

	std::vector<X52Device> x52devices;

	void device_add(void* hDevice);
	void device_remove(void* hDevice);

	static void __stdcall DirectOutput_Device_Callback(void* hDevice, bool bAdded, void* pvContext);
	static void __stdcall DirectOutput_Enumerate_Callback(void* hDevice, void* pvContext);

};