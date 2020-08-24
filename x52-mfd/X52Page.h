#pragma once
#include <string>
#include <tuple>

class X52Page
{
public:
	X52Page();
	X52Page(bool interactive);
	X52Page(std::tuple<std::string, std::string, std::string> text);
	X52Page(std::tuple<std::string, std::string, std::string> text, bool interactive);

	const int id;

	std::string get_text_top();
	std::string get_text_mid();
	std::string get_text_bot();

	void set_text_top(std::string text);
	void set_text_mid(std::string text);
	void set_text_bot(std::string text);

	bool is_interactive();

	bool operator==(const X52Page& page)
	{
		bool interactive_check = this->interactive == page.interactive;
		bool string_check = this->top == page.top && this->mid == page.mid && this->bot == page.bot;

		return interactive_check && string_check;
	}

private:
	bool interactive;
	std::string top, mid, bot;
};
