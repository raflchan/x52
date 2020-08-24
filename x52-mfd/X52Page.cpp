#include "X52Page.h"

#include "stdafx.h"


X52Page::X52Page()
{
	this->interactive = false;
	this->top = "";
	this->mid = "";
	this->bot = "";
}

X52Page::X52Page(bool interactive)
{
	this->interactive = interactive;
	this->top = "";
	this->mid = "";
	this->bot = "";
}

X52Page::X52Page(std::tuple<std::string, std::string, std::string> text)
{
	this->interactive = false;
	this->top = std::get<0>(text);
	this->mid = std::get<1>(text);
	this->bot = std::get<2>(text);
}

X52Page::X52Page(std::tuple<std::string, std::string, std::string> text, bool interactive)
{
	this->interactive = interactive;
	this->top = std::get<0>(text);
	this->mid = std::get<1>(text);
	this->bot = std::get<2>(text);

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

bool X52Page::is_interactive()
{
	return this->interactive;
}
