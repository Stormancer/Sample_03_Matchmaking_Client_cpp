#pragma once
#include "headers.h"
#include <stormancer.h>

namespace Stormancer
{
	struct GameServerInformations
	{
	public:
		std::string Ip;
		int Port;

		MSGPACK_DEFINE(Ip, Port);
	};
	struct RoundResult
	{
	public:
		std::string winnerId;

		int winnerScore;
		int loserScore;

		MSGPACK_DEFINE(winnerId, winnerScore, loserScore);
	};

	struct GameResults
	{
	public:
		std::string winnerId;

		bool forfait;
		std::vector<RoundResult> rounds;

		MSGPACK_DEFINE(winnerId,forfait,rounds);
	};

	
	
	
	class GameSessionService
	{
	public:
		GameSessionService(Stormancer::Scene* scene);

		pplx::task<std::shared_ptr<Result<GameServerInformations>>> waitServerReady(pplx::cancellation_token);

		std::vector<std::string> getConnectedPlayers();

		void unsubscribeConnectedPlayersChanged(Action<>::TIterator handle);
		Action<>::TIterator onConnectedPlayersChanged(std::function<void()> callback);

		pplx::task<std::shared_ptr<Result<void>>> sendGameResults(GameResults results);

	private:
		Action<> _onConnectedPlayersChanged;
		Scene* _scene;
		std::vector<std::string> _users;
		pplx::task_completion_event<GameServerInformations> _waitServerTce;
	};
}