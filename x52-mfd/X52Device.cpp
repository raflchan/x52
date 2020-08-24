#include "X52Device.h"

#include "stdafx.h"
#include "utils.h"

#define INPUT_SPACER_TIME 100 // ms


X52Device::X52Device(void* hDevice)
{
	this->hDevice = hDevice;
	this->current_page = 0;
	this->dwPage = 0;
	this->pageName = L"TestPage";

	DO_CALL(DirectOutput_RegisterPageCallback(this->hDevice, this->DirectOutput_Page_Callback, this));
	std::cout << "Registered devices page callback! " << guid_to_str(static_cast<GUID*>(hDevice)) << std::endl;
	DO_CALL(DirectOutput_RegisterSoftButtonCallback(hDevice, this->DirectOutput_SoftButton_Callback, this));
	std::cout << "Registered devices soft button callback! " << guid_to_str(static_cast<GUID*>(hDevice)) << std::endl;


	X52Page page(std::make_tuple( "Top", "Mid", "Bottom" ), false);
	this->page_add(page);

	this->drawPage();
	std::cout << "cool" << std::endl;
}

void X52Device::page_add(X52Page page)
{
	this->pages.push_back(page);
}

unsigned int X52Device::page_get_number(X52Page page)
{
	int i = 0;
	for (auto it = this->pages.begin(); it != pages.end(); it++)
	{
		if (*it == page)
			return i;

		i++;
	}

	throw std::runtime_error("Page not registered!");
}

void X52Device::page_set(unsigned int pagenr)
{
	if (pagenr >= 0 && pagenr < this->pages.size())
	{
		this->current_page = pagenr;
		this->drawPage();
	}

	throw std::runtime_error("Invalid Page Nr.!");
}

const void* X52Device::hDevice_get()
{
	return this->hDevice;
}

void X52Device::drawPage()
{

	//this->dwPage = this->dwPage + 1;
	this->dwPage = 0;
	// TODO: this gets changed somehow???
	std::cout << this->dwPage << std::endl;
	DO_CALL(DirectOutput_AddPage(this->hDevice, this->dwPage, this->pageName, FLAG_SET_AS_ACTIVE));

	auto page = this->pages.at(this->current_page);
	
	if (page.is_interactive())
	{

	}
	else
	{
		std::string top = page.get_text_top();
		std::string mid = page.get_text_mid();
		std::string bot = page.get_text_bot();
		DO_CALL(DirectOutput_SetString(this->hDevice, this->dwPage, X52_MFD_TOP, static_cast<DWORD>(top.length()), std::wstring(top.begin(), top.end()).c_str()));
		DO_CALL(DirectOutput_SetString(this->hDevice, this->dwPage, X52_MFD_MID, static_cast<DWORD>(mid.length()), std::wstring(mid.begin(), mid.end()).c_str()));
		DO_CALL(DirectOutput_SetString(this->hDevice, this->dwPage, X52_MFD_BOT, static_cast<DWORD>(bot.length()), std::wstring(bot.begin(), bot.end()).c_str()));
	}
}

void __stdcall X52Device::DirectOutput_Page_Callback(void* hDevice, DWORD dwPage, bool bActivated, void* pvContext)
{
	X52Device* x52 = static_cast<X52Device*>(pvContext);

	std::cout << "Page Callback -> redrawing page!" << std::endl;

	x52->drawPage();

	//std::string activated = bActivated ? "activated" : "deactivated";
	//std::cout << "Page " << std::hex << std::setfill('0') << std::setw(4) << "0x" << dwPage << " has been " << activated << std::endl;
}

void __stdcall X52Device::DirectOutput_SoftButton_Callback(void* hDevice, DWORD dwButtons, void* pvContext)
{
	X52Device* x52 = static_cast<X52Device*>(pvContext);
	x52->input_sanitize(dwButtons);
}

void X52Device::input_sanitize(DWORD dwButton)
{
	if (dwButton == 0)
		return;

	static std::map<DWORD, unsigned long long> lastPresses = { {SoftButton_Down, 0}, {SoftButton_Up, 0}, {SoftButton_Select, 0} };
	unsigned long long current = current_time_millis();

	auto search = lastPresses.find(dwButton);
	if (search == lastPresses.end())
	{
		std::cout << "Unknown Button!" << std::endl;
		return;
	}
	unsigned long long lastPress = search->second;

	if (lastPress + INPUT_SPACER_TIME < current)
	{
		search->second = current;

		// TODO: this is not optimal, because of bit representation
		switch (dwButton)
		{
		case SoftButton_Down:
			std::cout << "Down" << std::endl;
			break;
		case SoftButton_Up:
			std::cout << "Up" << std::endl;
			break;
		case SoftButton_Select:
			std::cout << "Select" << std::endl;
			break;
		default:
			std::cout << "Unhandled Button" << std::endl;
			break;
		}
	}
}
