#pragma once
#include <string>
#include <vector>
#include <Windows.h>

#include "X52Page.h"

class X52Device
{
public:
	X52Device(void* hDevice);

	void page_add(X52Page page);
	unsigned int page_get_number(X52Page page);
	void page_set(unsigned int pagenr);

	const void* hDevice_get();

private:
	void* hDevice;
	DWORD dwPage;
	const wchar_t* pageName;
	
	std::string guid;
	unsigned int current_page;
	std::vector<X52Page> pages;

	void drawPage();

	static void __stdcall DirectOutput_Page_Callback(void* hDevice, DWORD dwPage, bool bActivated, void* pvContext);
	static void __stdcall DirectOutput_SoftButton_Callback(void* hDevice, DWORD dwButtons, void* pvContext);

	void input_sanitize(DWORD dwButton);

};
