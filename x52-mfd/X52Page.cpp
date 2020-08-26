#include "X52Page.h"

#include "stdafx.h"


X52Page::X52Page()
{
	this->interactive = false;
	this->top = "";
	this->mid = "";
	this->bot = "";

	this->callbacks[0] = nullptr;
	this->callbacks[1] = nullptr;
	this->callbacks[2] = nullptr;
}

X52Page::X52Page(bool interactive)
{
	this->interactive = interactive;
	this->top = "";
	this->mid = "";
	this->bot = "";

	this->callbacks[0] = nullptr;
	this->callbacks[1] = nullptr;
	this->callbacks[2] = nullptr;
}

//X52Page::X52Page(std::tuple<std::string, std::string, std::string> text) : id(_id_cnt++)
//{
//	this->interactive = false;
//	this->top = std::get<0>(text);
//	this->mid = std::get<1>(text);
//	this->bot = std::get<2>(text);
//}
//
//X52Page::X52Page(std::tuple<std::string, std::string, std::string> text, bool interactive) : id(_id_cnt++)
//{
//	this->interactive = interactive;
//	this->top = std::get<0>(text);
//	this->mid = std::get<1>(text);
//	this->bot = std::get<2>(text);
//}

X52Page::X52Page(std::tuple<std::string, std::function<void()>> top, std::tuple<std::string, std::function<void()>> mid, std::tuple<std::string, std::function<void()>> bot)

{
	this->interactive = false;
	this->top = std::get<0>(top);
	this->mid = std::get<0>(mid);
	this->bot = std::get<0>(bot);

	this->callbacks[0] = nullptr;
	this->callbacks[1] = nullptr;
	this->callbacks[2] = nullptr;
}
X52Page::X52Page(std::tuple<std::string, std::function<void()>> top, std::tuple<std::string, std::function<void()>> mid, std::tuple<std::string, std::function<void()>> bot, bool interactive)
{
	this->interactive = interactive;
	this->top = std::get<0>(top);
	this->mid = std::get<0>(mid);
	this->bot = std::get<0>(bot);

	this->callbacks[0] = std::get<1>(top);
	this->callbacks[1] = std::get<1>(mid);
	this->callbacks[2] = std::get<1>(bot);
}

std::string X52Page::get_text_top()
{
	return this->top;
}
std::string X52Page::get_text_mid()
{
	return this->mid;
}
std::string X52Page::get_text_bot()
{
	return this->bot;
}

void X52Page::set_text_top(std::string text)
{
	this->top = text;
}
void X52Page::set_text_mid(std::string text)
{
	this->mid = text;
}
void X52Page::set_text_bot(std::string text)
{
	this->bot = text;
}

void X52Page::call_selection(int selection)
{
	auto func = this->callbacks[selection];
	if (func)
		func();
	else
		LOG_DEBUG("There is no callback set, ignoring!");
}

bool X52Page::is_interactive()
{
	return this->interactive;
}
