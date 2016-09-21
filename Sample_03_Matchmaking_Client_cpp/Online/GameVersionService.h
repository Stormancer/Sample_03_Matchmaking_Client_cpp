#pragma once
#include <stormancer.h>

namespace Stormancer
{
	class GameVersionService
	{
	public:
		GameVersionService(Stormancer::Scene* scene);
		~GameVersionService();

		const char* gameVersion();

		void onGameVersionUpdate(std::function<void(std::string)> callback);
		void onServerVersionUpdate(std::function<void(std::string)> callback);

	private:
		std::string _gameVersion = "unknown";
		Stormancer::Scene* _scene = nullptr;
		std::function<void(std::string)> _onGameVersionUpdate = nullptr;
		std::function<void(std::string)> _onServerVersionUpdate = nullptr;
	};
}

