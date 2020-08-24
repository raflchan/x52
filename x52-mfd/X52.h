#pragma once

#include "stdafx.h"
#include "X52Device.h"


class X52
{
public:
	X52();
	~X52();
	std::vector<void*> devices;
	std::vector<X52Device*> x52devices;

	void init();
	void test();
	X52Device* device_get(void* hDevice);

private:


	void device_add(void* hDevice);
	void device_remove(void* hDevice);

	static void __stdcall DirectOutput_Device_Callback(void* hDevice, bool bAdded, void* pvContext);
	static void __stdcall DirectOutput_Enumerate_Callback(void* hDevice, void* pvContext);

};