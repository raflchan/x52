#pragma once

#include "stdafx.h"

#define LOG_DEBUG(x) \
	std::cout << "[DEBUG] " << __FILE__ << ":" << std::dec << __LINE__ << " " << x << std::endl;

#define LOG_INFO(x) \
	std::cout << "[INFO]  " << __FILE__ << ":" << std::dec << __LINE__ << " " << x << std::endl;

#define LOG_WARN(x) \
	std::cout << "[WARN]  " << __FILE__ << ":" << std::dec << __LINE__ << " " << x << std::endl;

#define LOG_ERROR(x) \
	std::cout << "[ERROR] " << __FILE__ << ":" << std::dec << __LINE__ << " " << x << std::endl;

#define DO_ERROR(x)		\
	if (x != S_OK){		\
		LOG_ERROR("Failed DirectOutput call!"); \
		exit_critically(x);} \
	else				\
		nullptr

#define DO_WARN(x)		\
	if (x != S_OK){LOG_WARN("Failed DirectOutput call!");} else nullptr


std::string guid_to_str(GUID* guid);
unsigned long long current_time_millis();

void exit_critically(HRESULT res);
