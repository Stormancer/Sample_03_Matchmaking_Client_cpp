#pragma once
#include "stormancer.h"
namespace Stormancer
{

	template<class T>
	class PlayerProfileService<T>
	{
	public:
		PlayerProfileService(Scene* scene)
		{
			_scene = scene;
		}
	public:
		pplx::task<std::shared_ptr<Result<T>>> Get()
		{
			auto rpc = _scene->dependencyResolver()->resolve<IRpcService>();
			return rpc->rpc<T>("profiles.getsingle");
		}

		pplx::task<std::shared_ptr<Result<void>>> Reset()
		{
			auto rpc = _scene->dependencyResolver()->resolve<IRpcService>();
			return rpc->rpcVoid("profiles.delete");
		}

	private:
		Scene* _scene;
	};


	template<class T>
	class PlayerProfilePlugin<T> : IPlugin
	{
		void registerSceneDependencies(Scene* scene)
		{
			if (scene)
			{
				auto name = scene->getHostMetadata("WindJammers.profiles");

				if (name && std::strlen(name) > 0)
				{
					auto service = std::make_shared<PlayerProfileService<T>>(scene);
					scene->dependencyResolver()->registerDependency(service);
				}
			}
			
		};

		
	};

	
}