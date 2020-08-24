#pragma once

#include "stdafx.h"

std::string guid_to_str(GUID* guid);
unsigned long long current_time_millis();

void DO_CALL(HRESULT res);
void exit_critically(HRESULT res);
