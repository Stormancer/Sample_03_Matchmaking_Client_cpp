// Sample_03_Matchmaking_Client_cpp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Online\AuthenticationPlugin.h"

const std::string accountId = "dotemu-windjammers";
const std::string application = "dev-server";
const std::string endpoint = "http://api.stormancer.com";


pplx::task<Stormancer::Scene*> ConnectToServices(Stormancer::Client* client, std::string steamToken)
{
	

	auto auth = client->dependencyResolver()->resolve<Stormancer::IAuthenticationService>();


	return auth->steamLogin("u1")
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
		.then([](pplx::task<Stormancer::Result<Stormancer::Scene*>*> result) {
		try
		{
			printf("%i\n", std::this_thread::get_id());
			auto service = result.get()->get();
			Stormancer::destroy(result.get());
			return service->connect().then([service](pplx::task<Stormancer::Result<void>*> t) {return service; });
		}
		catch (const std::exception&)
		{
			printf("an error occured while trying to get the 'services' scene.");
			throw;
		}
	});
}

int main()
{
	
	auto config = Stormancer::Configuration::create(endpoint, accountId, application);
	auto d1 = std::make_shared<Stormancer::MainThreadActionDispatcher>();
	config->actionDispatcher = d1;

	config->addPlugin(new Stormancer::AuthenticationPlugin());
	auto client = Stormancer::Client::createClient(config);

	
	
	ConnectToServices(client, "u1");

	printf("%i\n", std::this_thread::get_id());


	///Game loop
	while (true)
	{

		//Give at most 5ms to the dispatcher to execute pending tasks
		d1->update(std::chrono::milliseconds(5));
		//d2->update(std::chrono::milliseconds(5));
		//Sleep for 10ms (for demo)
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	return 0;
}



