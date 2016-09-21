#pragma once
#include <stormancer.h>

namespace Stormancer
{
	class GameVersionPlugin : public Stormancer::IPlugin
	{
	public:
		void sceneCreated(Stormancer::Scene* scene);
		void destroy();
	};
}

