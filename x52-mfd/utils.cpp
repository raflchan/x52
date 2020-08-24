#include "utils.h"
#include "stdafx.h"

std::string guid_to_str(GUID* guid)
{
	std::stringstream stream;
	unsigned char* a = guid->Data4;
	stream << std::hex
		<< "{"
		<< guid->Data1
		<< "-"
		<< guid->Data2
		<< "-"
		<< guid->Data3
		<< "-"
		<< static_cast<unsigned>(a[0])
		<< static_cast<unsigned>(a[1])
		<< "-"
		<< static_cast<unsigned>(a[2])
		<< static_cast<unsigned>(a[3])
		<< static_cast<unsigned>(a[4])
		<< static_cast<unsigned>(a[5])
		<< static_cast<unsigned>(a[6])
		<< static_cast<unsigned>(a[7])
		<< "}";
	return stream.str();
}

unsigned long long current_time_millis()
{
	using namespace std::chrono;
	return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void exit_critically(HRESULT res)
{
	_com_error err(res);
	const TCHAR* errMsg = err.ErrorMessage();
	DirectOutput_Deinitialize();

	std::cout << "Error encountered: ";
	std::wcout << errMsg << std::endl;
	exit(EXIT_FAILURE);
}
