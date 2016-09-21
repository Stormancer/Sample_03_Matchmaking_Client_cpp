#include "stdafx.h"
#include "GameVersionPlugin.h"
#include "GameVersionService.h"


namespace Stormancer
{
	void GameVersionPlugin::sceneCreated(Stormancer::Scene* scene)
	{
		if (scene)
		{
			auto name = scene->getHostMetadata("stormancer.gameVersion");

			if (name && std::strlen(name) > 0)
			{
				auto service = new GameVersionService(scene);
				scene->dependencyResolver()->registerDependency<GameVersionService>(service);
			}
		}
	}

	void GameVersionPlugin::destroy()
	{
		delete this;
	}
}


