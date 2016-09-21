#include "stdafx.h"
#include "GameVersionService.h"

namespace Stormancer
{
	GameVersionService::GameVersionService(Stormancer::Scene* scene)
		: _scene(scene)
	{
		_scene->addRoute("gameVersion.update", [this](Stormancer::Packetisp_ptr packet) {
			msgpack::unpacked result;
			std::string buffer;
			*packet->stream >> buffer;

			msgpack::unpack(result, buffer.data(), buffer.size());
			std::string version;
			result.get().convert(&version);

			_gameVersion = version;

			if (_onGameVersionUpdate)
			{
				_onGameVersionUpdate(version);
			}
		});

		_scene->addRoute("serverVersion.update", [this](Stormancer::Packetisp_ptr packet) {
			msgpack::unpacked result;
			std::string buffer;
			*packet->stream >> buffer;

			msgpack::unpack(result, buffer.data(), buffer.size());
			std::string version;
			result.get().convert(&version);
			
			if (_onServerVersionUpdate)
			{
				_onServerVersionUpdate(version);
			}
		});
	}

	GameVersionService::~GameVersionService()
	{
	}

	const char* GameVersionService::gameVersion()
	{
		return _gameVersion.c_str();
	}

	void GameVersionService::onGameVersionUpdate(std::function<void(std::string)> callback)
	{
		_onGameVersionUpdate = callback;
	}

	void GameVersionService::onServerVersionUpdate(std::function<void(std::string)> callback)
	{
		_onServerVersionUpdate = callback;
	}
}
