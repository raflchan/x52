#include "X52.h"

#include "utils.h"


void __stdcall DirectOutput_Device_Callback(void* hDevice, bool bAdded, void* pvContext);
void __stdcall DirectOutput_SoftButton_Callback(void* hDevice, DWORD dwButtons, void* pvContext);
void __stdcall DirectOutput_Enumerate_Callback(void* hDevice, void* pvContext);

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

	DO_CALL(DirectOutput_Initialize(appname));
	DO_CALL(DirectOutput_RegisterDeviceCallback(this->DirectOutput_Device_Callback, this));
	DO_CALL(DirectOutput_Enumerate(this->DirectOutput_Enumerate_Callback, this));
}

void X52::device_add(void* hDevice)
{
	this->devices.push_back(hDevice);
	this->x52devices.push_back(X52Device(hDevice));
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
		if ((*iter).hDevice_get() == hDevice)
		{
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
		std::cout << "Added a device! " << guid_to_str(static_cast<GUID*>(hDevice)) << std::endl;
	}
	else {
		x52->device_remove(hDevice);
		std::cout << "Removed a device! " << guid_to_str(static_cast<GUID*>(hDevice)) << std::endl;
	}
}

void __stdcall X52::DirectOutput_Enumerate_Callback(void* hDevice, void* pvContext)
{
	X52* x52 = static_cast<X52*>(pvContext);
	x52->device_add(hDevice);
	std::cout << "Detected a device! " << guid_to_str(static_cast<GUID*>(hDevice)) << std::endl;
}

void X52::test()
{

	DWORD dwPage = 0x00;
	const wchar_t* pageDebugName = L"TestPage";
	DO_CALL(DirectOutput_AddPage(devices[0], dwPage, pageDebugName, FLAG_SET_AS_ACTIVE));


	int _x;
	std::cin >> _x;

	DO_CALL(DirectOutput_Deinitialize());
	std::cout << "Programm exited nominally!" << std::endl;
	exit(EXIT_SUCCESS);
}
