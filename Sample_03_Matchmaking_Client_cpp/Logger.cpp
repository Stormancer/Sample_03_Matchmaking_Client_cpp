#include "stdafx.h"
#include "Logger.h"

void Stormancer::Logger::log(const std::string message)
{
	printf(message.c_str());
	printf("\n");
}

void Stormancer::Logger::log(LogLevel level, const std::string category, const std::string message)
{
	log(ILogger::format(level,category,message));
}

void Stormancer::Logger::log(const std::exception & e)
{
	log(LogLevel::Error, "exception", e.what());
}
