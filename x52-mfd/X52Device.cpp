#include "X52Device.h"

#include "stdafx.h"
#include "utils.h"

#include "X52.h"

constexpr auto INPUT_SPACER_TIME = 50; // ms


X52Device::X52Device(void* hDevice, void* x52)
{
	this->x52 = x52;
	this->hDevice = hDevice;
	this->current_page = 0;
	this->dwPage = 0;
	this->pageName = L"TestPage";
	this->current_selection = 0;
	this->guid = guid_to_str(static_cast<GUID*>(hDevice));

	DO_ERROR(DirectOutput_RegisterPageCallback(this->hDevice, this->DirectOutput_Page_Callback, this->x52));
	LOG_INFO("Registered devices page callback! " + guid_to_str(static_cast<GUID*>(hDevice)));

	DO_ERROR(DirectOutput_RegisterSoftButtonCallback(hDevice, this->DirectOutput_SoftButton_Callback, this->x52));
	LOG_INFO("Registered devices soft button callback! " + guid_to_str(static_cast<GUID*>(hDevice)));

	DO_ERROR(DirectOutput_AddPage(this->hDevice, this->dwPage, this->pageName, FLAG_SET_AS_ACTIVE));
	LOG_INFO("Registered device data page! " + guid_to_str(static_cast<GUID*>(hDevice)));


	//X52Page page(std::make_tuple("Site1: Item 1", "Site1: Item 2 >", "Site1: Item 3 >"), true);
	//this->page_add(page);

	//X52Page page2(std::make_tuple("Site 2", "No Interaction", "Only Text"), false);
	//this->page_add(page2);

	//X52Page page3(std::make_tuple("Site3: Item 4", "Site3: Item 5", "Site3: Item 6"), true);
	//this->page_add(page3);

	this->drawPage();
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
	if (this->pages.size() == 0)
	{
		std::string top = "    x52-mfd";
		std::string mid = "    by rafl";
		std::string bot = "easy x52 mfd interface usind DirectOutput.dll";

		DO_WARN(DirectOutput_SetString(this->hDevice, this->dwPage, X52_MFD_TOP, static_cast<DWORD>(top.length()), std::wstring(top.begin(), top.end()).c_str()));
		DO_WARN(DirectOutput_SetString(this->hDevice, this->dwPage, X52_MFD_MID, static_cast<DWORD>(mid.length()), std::wstring(mid.begin(), mid.end()).c_str()));
		DO_WARN(DirectOutput_SetString(this->hDevice, this->dwPage, X52_MFD_BOT, static_cast<DWORD>(bot.length()), std::wstring(bot.begin(), bot.end()).c_str()));
		return;
	}

	auto page = this->pages.at(this->current_page);
	
	std::string top = page.get_text_top();
	std::string mid = page.get_text_mid();
	std::string bot = page.get_text_bot();

	if (page.is_interactive())
	{
		switch (this->current_selection)
		{
		case 0:
			top = ">" + top;
			mid = " " + mid;
			bot = " " + bot;
			break;
		case 1:
			top = " " + top;
			mid = ">" + mid;
			bot = " " + bot;
			break;
		case 2:
			top = " " + top;
			mid = " " + mid;
			bot = ">" + bot;
			break;
		default:
			throw std::runtime_error("This shouldn't happen!");
		}
	}

	DO_WARN(DirectOutput_SetString(this->hDevice, this->dwPage, X52_MFD_TOP, static_cast<DWORD>(top.length()), std::wstring(top.begin(), top.end()).c_str()));
	DO_WARN(DirectOutput_SetString(this->hDevice, this->dwPage, X52_MFD_MID, static_cast<DWORD>(mid.length()), std::wstring(mid.begin(), mid.end()).c_str()));
	DO_WARN(DirectOutput_SetString(this->hDevice, this->dwPage, X52_MFD_BOT, static_cast<DWORD>(bot.length()), std::wstring(bot.begin(), bot.end()).c_str()));
}

void __stdcall X52Device::DirectOutput_Page_Callback(void* hDevice, DWORD dwPage, bool bActivated, void* pvContext)
{
	X52* x52 = static_cast<X52*>(pvContext);
	X52Device* x52device = x52->device_get(hDevice);

	if (bActivated && dwPage == x52device->dwPage)
	{
		LOG_DEBUG("Page Callback -> redrawing page!");
		x52device->drawPage();
	}
}

void __stdcall X52Device::DirectOutput_SoftButton_Callback(void* hDevice, DWORD dwButtons, void* pvContext)
{
	X52* x52 = static_cast<X52*>(pvContext);
	X52Device* x52device = x52->device_get(hDevice);
	x52device->handle_input(dwButtons);
}

void X52Device::handle_input(DWORD dwButton)
{
	if (dwButton == 0)
		return;

	static std::map<DWORD, unsigned long long> lastPresses = { {SoftButton_Down, 0}, {SoftButton_Up, 0}, {SoftButton_Select, 0} };
	unsigned long long current = current_time_millis();

	auto search = lastPresses.find(dwButton);
	if (search == lastPresses.end())
	{
		LOG_DEBUG("Unknown Button!");
		return;
	}
	unsigned long long lastPress = search->second;

	if (lastPress + INPUT_SPACER_TIME < current)
	{
		search->second = current;

		// TODO: this is not optimal, because of bit representation
		switch (dwButton)
		{
		case SoftButton_Up:
			LOG_DEBUG("Up");
			this->vertical_movement(dwButton);
			break;

		case SoftButton_Down:
			LOG_DEBUG("Down");
			this->vertical_movement(dwButton);
			break;

		case SoftButton_Select:
			LOG_DEBUG("Select");
			break;

		default:
			LOG_DEBUG("Unhandled Button");
			break;
		}
	}
}

void X52Device::vertical_movement(DWORD direction)
{
	if (this->pages.size() == 0) return;

	auto page_nr = this->current_page;
	X52Page page = this->pages.at(page_nr);

	if (page.is_interactive())
	{
		int a;
		int b;
		switch (direction)
		{
		case SoftButton_Up:
			a = (current_selection - 1l);
			b = 3;
			break;

		case SoftButton_Down:
			a = (current_selection + 1l);
			b = 3;
			break;

		default:
			return;
		}
		this->current_selection = a < 0 ? b - 1 : a % b;
		if (a < 0)
		{
			int a2 = (page_nr - 1l);
			int b2 = this->pages.size();
			this->current_page = a2 < 0 ? b2 - 1 : a2 % b2;
		}
		else if(a >= b)
		{
			int a2 = (page_nr + 1l);
			int b2 = this->pages.size();
			this->current_page = a2 < 0 ? b2 - 1 : a2 % b2;
		}
	}
	else
	{
		int a;
		int b;
		switch (direction)
		{
		case SoftButton_Up:
			a = (page_nr - 1l);
			b = this->pages.size();
			break;

		case SoftButton_Down:
			a = (page_nr + 1l);
			b = this->pages.size();
			break;

		default:
			return;
		}
		this->current_page = a < 0 ? b - 1 : a % b;
	}
	this->drawPage();

}
