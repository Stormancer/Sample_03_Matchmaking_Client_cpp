#include "stdafx.h"
#include "Logger.h"

void Stormancer::Logger::log(const char* message)
{
	printf(message);
	printf("\n");
}

void Stormancer::Logger::log(LogLevel level, const char* category, const char* message, const char* data)
{
	log(ILogger::format(level,category,message,data).get());
}

void Stormancer::Logger::log(const std::exception & e)
{
	log(LogLevel::Error, "exception", e.what(),"");
}
