#pragma once



namespace Stormancer
{
	enum class MatchState
	{
		Unknown = -1,
		SearchStart = 0,
		CandidateFound = 1,
		WaitingPlayersReady = 2,
		Success = 3,
		Failed = 4,
		Canceled = 5
	};

	enum class FleetSize
	{
		Unknown = -1,
		Small = 1,
		Medium = 2,
		Large = 3
	};

	enum class Readiness
	{
		Unknown = 0,
		Ready = 1,
		NotReady = 2
	};

	struct Player
	{
	public:

		std::string playerId;
		std::string pseudo;
		std::string platformId;
		std::string platform;

		MSGPACK_DEFINE(playerId, pseudo, platformId, platform);
	};

	struct MatchmakingRequest
	{
	public:
		std::string gameMode;
		int ranking;

		MSGPACK_DEFINE(gameMode, ranking);
	};

	struct MatchmakingResponse
	{
	public:
		std::string gameId;
		std::vector<Player> team1;
		std::vector<Player> team2;
		std::vector<std::string> optionalParameters;

		MSGPACK_DEFINE(gameId, team1, team2, optionalParameters);

	};

	struct ReadyVerificationRequest
	{
	public:
		std::map<std::string, Readiness> members;
		std::string matchId;
		Stormancer::int32 timeout;
		Stormancer::int32 membersCountReady;
		Stormancer::int32 membersCountTotal;
	};

	// Msgpack doesn't support using C++11 enum class so we use int32 instead in this temporary internal functions
	namespace Internal
	{
		struct MatchmakingResponse
		{
		public:
			std::string gameId;
			std::string hostPlatformId;
			std::vector<Player> team1;
			std::vector<Player> team2;
			std::vector<std::string> optionalParameters;


			MSGPACK_DEFINE(gameId, hostPlatformId, team1, team2, optionalParameters);
		};

		struct ReadyVerificationRequest
		{
		public:
			std::map<std::string, Stormancer::int32> members;
			std::string matchId;
			Stormancer::int32 timeout;

			MSGPACK_DEFINE(members, matchId, timeout);
		};
	}

	class MatchmakingService
	{
	public:
		MatchmakingService(Stormancer::Scene* scene);
		~MatchmakingService();

		MatchmakingService(const MatchmakingService& other) = delete;
		MatchmakingService(const MatchmakingService&& other) = delete;
		MatchmakingService& operator=(const MatchmakingService&& other) = delete;

		void onMatchUpdate(std::function<void(MatchState)> callback);
		void onMatchParametersUpdate(std::function<void(MatchmakingRequest)> callback);
		void onMatchReadyUpdate(std::function<void(ReadyVerificationRequest)> callback);
		void onMatchFound(std::function<void(MatchmakingResponse)> callback);

		MatchState matchState() const;

		template<typename T>
		pplx::task<std::shared_ptr<Stormancer::Result<>>> findMatch(std::string provider, T requestParams)
		{
			pplx::task_completion_event<std::shared_ptr<Stormancer::Result<>>> tce;

			_isMatching = true;



			auto observable = _rpcService->rpc("match.find", [provider, requestParams](Stormancer::bytestream* stream) {
				msgpack::pack(stream, provider);
				msgpack::pack(stream, requestParams);
			}, PacketPriority::MEDIUM_PRIORITY);

			auto onNext = [this, tce](Stormancer::Packetisp_ptr packet) {
				_isMatching = false;
				auto sub = _matchmakingSubscription.lock();
				if (sub)
				{
					sub->unsubscribe();
				}
				std::shared_ptr<Stormancer::Result<>> res(new Stormancer::Result<>());
				//auto res = new std::shared_ptr<Stormancer::Result<>*>();
				res->set();
				tce.set(res);
			};

			auto onError = [this, tce](const char* error) {
				_isMatching = false;
				auto sub = _matchmakingSubscription.lock();
				if (sub)
				{
					sub->unsubscribe();
				}

				std::shared_ptr<Stormancer::Result<>> res(new Stormancer::Result<>());
				//auto res = new std::shared_ptr<Stormancer::Result<>*>();
				res->setError(1, error);
				tce.set(res);
			};

			auto onComplete = []() {
			};

			_matchmakingSubscription = observable->subscribe(onNext, onError, onComplete);

			return pplx::create_task(tce);
		}

		void resolve(bool acceptMatch);

		void cancel();

	private:
		Stormancer::Scene* _scene = nullptr;
		std::shared_ptr<Stormancer::IRpcService> _rpcService = nullptr;
		bool _isMatching = false;
		std::weak_ptr<Stormancer::ISubscription> _matchmakingSubscription;
		std::function<void(MatchState)> _onMatchUpdate;
		std::function<void(MatchmakingRequest)> _onMatchParametersUpdate;
		std::function<void(ReadyVerificationRequest)> _onMatchReadyUpdate;
		std::function<void(MatchmakingResponse)> _onMatchFound;
		MatchState _matchState = MatchState::Unknown;
		MatchmakingRequest _matchmakingRequest;
	};
};
