// Sample_03_Matchmaking_Client_cpp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Online\AuthenticationPlugin.h";

const std::string accountId = "dotemu-windjammers";
const std::string application = "dev-server";
const std::string endpoint = "http://api.stormancer.com";

int main()
{
	auto config = Stormancer::Configuration::create(endpoint, accountId, application);
	auto dispatcher = std::make_shared<Stormancer::MainThreadActionDispatcher>();
	config->actionDispatcher = dispatcher;

	config->addPlugin(new Stormancer::AuthenticationPlugin());
	auto client = Stormancer::Client::createClient(config);

	auto auth = client->dependencyResolver()->resolve<Stormancer::IAuthenticationService>();


	while (true)
	{
		dispatcher->update(10);
		Sleep(16);
	}
    return 0;
}

