#pragma once
#include <functional>
#include <string>
#include <tuple>

class X52Page
{
public:
	X52Page();
	X52Page(bool interactive);
	X52Page(std::tuple<std::string, std::function<void()>> top, std::tuple<std::string, std::function<void()>> mid, std::tuple<std::string, std::function<void()>> bot);
	X52Page(std::tuple<std::string, std::function<void()>> top, std::tuple<std::string, std::function<void()>> mid, std::tuple<std::string, std::function<void()>> bot, bool interactive);
	//X52Page(std::tuple<std::string, std::string, std::string> text);
	//X52Page(std::tuple<std::string, std::string, std::string> text, bool interactive);

	std::string get_text_top();
	std::string get_text_mid();
	std::string get_text_bot();

	void set_text_top(std::string text);
	void set_text_mid(std::string text);
	void set_text_bot(std::string text);

	void call_selection(int selection);

	bool is_interactive();

	bool operator==(const X52Page& page) const
	{
		bool interactive_check = this->interactive == page.interactive;
		bool string_check = this->top == page.top && this->mid == page.mid && this->bot == page.bot;

		return interactive_check && string_check;
	}

private:
	bool interactive;
	std::string top, mid, bot;
	std::function<void()> callbacks[3];
};
