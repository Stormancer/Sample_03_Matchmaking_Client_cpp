#include "stdafx.h"
#include "AuthenticationPlugin.h"
#include "AuthenticationService.h"

namespace Stormancer
{
	AuthenticationPlugin::AuthenticationPlugin()
	{
	}

	AuthenticationPlugin::~AuthenticationPlugin()
	{
	}

	void AuthenticationPlugin::clientCreated(Client* client)
	{
		auto authService = std::make_shared<AuthenticationService>(client);
		client->dependencyResolver()->registerDependency<IAuthenticationService>(static_cast<std::shared_ptr<IAuthenticationService>>(authService));
	}

	void AuthenticationPlugin::destroy()
	{
		delete this;
	}
};
