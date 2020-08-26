#include "X52.h"

#include "utils.h"


static bool _instantiated = false;


X52::X52()
{
	if (_instantiated)
		throw std::runtime_error("Cant instantiate multiple instances!");
	
	_instantiated = true;
}

X52::~X52()
{
	_instantiated = false;
}

void X52::init()
{
	const wchar_t* appname = L"rafl app";

	DO_ERROR(DirectOutput_Initialize(appname));
	DO_ERROR(DirectOutput_RegisterDeviceCallback(this->DirectOutput_Device_Callback, this));
	DO_ERROR(DirectOutput_Enumerate(this->DirectOutput_Enumerate_Callback, this));
}

void X52::device_add(void* hDevice)
{
	this->devices.push_back(hDevice);
	this->x52devices.push_back(new X52Device(hDevice, this));
}

void X52::device_remove(void* hDevice)
{
	for (auto iter = this->devices.begin(); iter != this->devices.end(); iter++)
	{
		if (*iter == hDevice)
		{
			this->devices.erase(iter);
			break;
		}
	}

	for (auto iter = this->x52devices.begin(); iter != this->x52devices.end(); iter++)
	{
		if ((**iter).hDevice_get() == hDevice)
		{
			delete static_cast<X52Device*>(*iter);
			this->x52devices.erase(iter);
			break;
		}
	}
}

void __stdcall X52::DirectOutput_Device_Callback(void* hDevice, bool bAdded, void* pvContext)
{
	X52* x52 = static_cast<X52*>(pvContext);
	if (bAdded)
	{
		x52->device_add(hDevice);
		LOG_INFO("Added a device! " + guid_to_str(static_cast<GUID*>(hDevice)));
	}
	else {
		x52->device_remove(hDevice);
		LOG_INFO("Removed a device! " + guid_to_str(static_cast<GUID*>(hDevice)));
	}
}

void __stdcall X52::DirectOutput_Enumerate_Callback(void* hDevice, void* pvContext)
{
	X52* x52 = static_cast<X52*>(pvContext);
	x52->device_add(hDevice);
	LOG_INFO("Detected a device! " + guid_to_str(static_cast<GUID*>(hDevice)));
}

void X52::test()
{

	DWORD dwPage = 0x00;
	const wchar_t* pageDebugName = L"TestPage";
	DO_ERROR(DirectOutput_AddPage(devices[0], dwPage, pageDebugName, FLAG_SET_AS_ACTIVE));


	int _x;
	std::cin >> _x;

	DO_ERROR(DirectOutput_Deinitialize());
	LOG_INFO("Programm exited nominally!");
	exit(EXIT_SUCCESS);
}

X52Device* X52::device_get(void* hDevice)
{
	for (auto it = this->x52devices.begin(); it != this->x52devices.end(); it++)
		if ((**it).hDevice_get() == hDevice)
			return &(**it);

	return nullptr;
}

std::vector<X52Device*> X52::get_x52devices()
{
	return this->x52devices;
}
