#include "stdafx.h"
#include "MatchmakingPlugin.h"
#include "MatchmakingService.h"
#include "GameSession.h"
#include "BugReportService.h"
namespace Stormancer
{
	void MatchmakingPlugin::sceneCreated(Stormancer::Scene* scene)
	{
		if (scene)
		{
			auto name = scene->getHostMetadata("stormancer.plugins.matchmaking");

			if (name && std::strlen(name) > 0 )
			{
				auto service = std::make_shared<MatchmakingService>(scene);
				scene->dependencyResolver()->registerDependency<MatchmakingService>(service);
			}

			name = scene->getHostMetadata("stormancer.gamesession");
			if (name && std::strlen(name) > 0)
			{
				auto service = std::make_shared<GameSessionService>(scene);
				scene->dependencyResolver()->registerDependency<GameSessionService>(service);
			}
			name = scene->getHostMetadata("stormancer.bugReporting");
			if (name && std::strlen(name) > 0)
			{
				auto service = std::make_shared<BugReportService>(scene);
				scene->dependencyResolver()->registerDependency<BugReportService>(service);
			}
		}
	}

	void MatchmakingPlugin::destroy()
	{
		delete this;
	}
};
