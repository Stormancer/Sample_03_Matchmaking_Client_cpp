#pragma once
#include "..\stdafx.h"
#include "MatchMakingService.h"
namespace Stormancer
{
	class MatchmakingPlugin : public Stormancer::IPlugin
	{
	public:
		void sceneCreated(Stormancer::Scene* scene);
		void destroy();
	};
};
