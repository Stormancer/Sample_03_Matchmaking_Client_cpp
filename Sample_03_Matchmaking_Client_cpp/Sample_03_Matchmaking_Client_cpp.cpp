// Sample_03_Matchmaking_Client_cpp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Online\AuthenticationPlugin.h"
#include "Online\MatchMakingPlugin.h"
#include "Logger.h"

const std::string accountId = "-dont-nod11-battlecrew";
const std::string application = "dev-server";
const std::string endpoint = "http://api.stormancer.com";

//const std::string accountId = "test";
//const std::string application = "test";
//const std::string endpoint = "http://localhost:8081";

Stormancer::Client* client;
std::unordered_map<std::string, pplx::task<Stormancer::Scene*>> scenes;


pplx::task<Stormancer::Scene*> ConnectToServices(std::string steamToken)
{


	auto auth = client->dependencyResolver()->resolve<Stormancer::IAuthenticationService>();


	return auth->steamLogin(steamToken)
		.then([auth](pplx::task<std::shared_ptr<Stormancer::Result<Stormancer::Scene*>>> result) {
		try
		{

			auto v = result.get();
			if (!v->success())
			{
				printf("an error occured while trying to authenticate with the server");
				throw std::exception(v->reason());
			}
			else
			{
				return auth->getPrivateScene("services");
			}
		}
		catch (const std::exception&)
		{
			printf("an error occured while trying to authenticate with the server.");
			throw;
		}
	})
		.then([auth](pplx::task<Stormancer::Result<Stormancer::Scene*>*> result) {
		try
		{
			printf("%i\n", std::this_thread::get_id());
			auto service = result.get()->get();
			Stormancer::destroy(result.get());
			printf((std::string("Authenticated as") + auth->userId() + "\n").c_str());
			pplx::task<Stormancer::Scene*> t = service->connect().then([service,auth](pplx::task<Stormancer::Result<void>*> t) {
				
				return service;
			});
			return t;
		}
		catch (const std::exception&)
		{
			printf("an error occured while trying to get the 'services' scene.");
			throw;
		}
	});
}

template<typename T>
pplx::task<std::shared_ptr<T>> GetService(const std::string sceneId)
{
	pplx::task_completion_event<std::shared_ptr<T>> tce;
	auto it = scenes.find(sceneId);
	if (it != scenes.end())
	{
		return it->second.then([](pplx::task<Stormancer::Scene*> t) {

			return t.get()->dependencyResolver()->resolve<T>();

		});
	}
	else
	{
		auto auth = client->dependencyResolver()->resolve<Stormancer::IAuthenticationService>();
		if (auth->connectionState() != Stormancer::GameConnectionState::Authenticated)
		{
			tce.set_exception(std::runtime_error("Client not connected to the server."));
		}
		else
		{
			pplx::task<Stormancer::Scene*> resultTask = auth->getPrivateScene(sceneId).then([tce, sceneId](pplx::task<Stormancer::Result<Stormancer::Scene*>*> t) {


				Stormancer::Result<Stormancer::Scene*>* result = t.get();

				if (result->success())
				{
					auto r= result->get();
					delete result;
					return r->connect().then([r](auto t1) {return r; });
				}
				else
				{
					auto reason = std::string(result->reason());
					delete result;
					scenes.erase(sceneId);
					throw std::runtime_error("Failed to connect to scene : " + reason);
				}

			});
			scenes[sceneId] = resultTask;
			return resultTask.then([](pplx::task<Stormancer::Scene*> t) {

				return t.get()->dependencyResolver()->resolve<T>();

			});
		}
	}



	return pplx::create_task(tce, pplx::task_options(client->dependencyResolver()->resolve<Stormancer::IActionDispatcher>()));
}

int main(int argc, char *argv[])
{
	Stormancer::ILogger::instance(std::make_shared<Stormancer::Logger>());
	auto config = Stormancer::Configuration::create(endpoint, accountId, application);
	auto d1 = std::make_shared<Stormancer::SameThreadActionDispatcher>();
	config->actionDispatcher = d1;

	config->addPlugin(new Stormancer::AuthenticationPlugin());
	config->addPlugin(new Stormancer::MatchmakingPlugin());
	client = Stormancer::Client::createClient(config);

	
	auto ticket = argv[1];
	auto gameMode = argv[2];

	ConnectToServices(ticket )
		.then([](pplx::task<Stormancer::Scene*> t)
	{
		return GetService<Stormancer::MatchmakingService>("matchmaking-fast");
	})
		.then([=](pplx::task<std::shared_ptr<Stormancer::MatchmakingService>> t)
	{

		auto matchmaking = t.get();
		auto rq = Stormancer::MatchmakingRequest();
		rq.gameMode = gameMode;
		rq.ranking = 50;
		matchmaking->onMatchFound([](Stormancer::MatchmakingResponse match) {
			printf("Match found!\n");
		});
		matchmaking->onMatchParametersUpdate([](Stormancer::MatchmakingRequest params) {
			printf(("received matchmaking params : "+params.gameMode+"\n").c_str());
		});
		matchmaking->onMatchUpdate([](Stormancer::MatchState state) {
			switch (state)
			{
			case Stormancer::MatchState::SearchStart:
				printf("search started\n");
				break;
			case Stormancer::MatchState::CandidateFound:
				printf("candidate found\n");
				break;
			case Stormancer::MatchState::Canceled:
				printf("match find cancelled\n");
			default:
				break;
			}
		});
		printf("%i\n", std::this_thread::get_id());
		auto task = matchmaking->findMatch("Battlecrew", rq);
		printf("starting matchmaking\n");
		return task;
	});

	printf("%i\n", std::this_thread::get_id());


	///Game loop
	while (true)
	{

		//Give at most 5ms to the dispatcher to execute pending tasks
		//d1->update(std::chrono::milliseconds(5));
		//d2->update(std::chrono::milliseconds(5));
		//Sleep for 10ms (for demo)
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	return 0;
}



