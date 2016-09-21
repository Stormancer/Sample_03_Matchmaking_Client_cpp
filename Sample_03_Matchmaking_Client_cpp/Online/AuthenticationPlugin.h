#pragma once
#include "stdafx.h"
#include "IAuthenticationService.h"

namespace Stormancer
{
	class AuthenticationPlugin : public IPlugin
	{
	public:
		AuthenticationPlugin();
		virtual ~AuthenticationPlugin();

		void clientCreated(Client* client);

		void destroy();
	};
};
