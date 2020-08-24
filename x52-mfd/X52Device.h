#pragma once
#include <string>
#include <vector>
#include <Windows.h>

#include "X52Page.h"

class X52Device
{
public:
	const void* x52;

	X52Device(void* hDevice, void* x52);


	const void* hDevice_get();

	void page_add(X52Page page);
	void page_set(unsigned int pagenr);
	unsigned int page_get_number(X52Page page);



private:
	void* hDevice;
	DWORD dwPage;
	const wchar_t* pageName;
	
	short current_selection;
	std::string guid;
	unsigned int current_page;
	std::vector<X52Page> pages;


	void drawPage();
	void handle_input(DWORD dwButton);
	void vertical_movement(DWORD direction);

	static void __stdcall DirectOutput_Page_Callback(void* hDevice, DWORD dwPage, bool bActivated, void* pvContext);
	static void __stdcall DirectOutput_SoftButton_Callback(void* hDevice, DWORD dwButtons, void* pvContext);


};
