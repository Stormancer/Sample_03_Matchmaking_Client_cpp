#pragma once
#include "ILogger.h"

namespace Stormancer
{
	class Logger :public ILogger
	{
		// Hérité via ILogger
		virtual void log(const char* message) override;
		virtual void log(LogLevel level, const char* category, const char* message, const char* data) override;
		virtual void log(const std::exception & e) override;
	};
}