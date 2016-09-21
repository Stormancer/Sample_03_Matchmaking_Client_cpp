#pragma once
#include "pplx\pplx.h"
#include "pplx\pplxinterface.h"
namespace Stormancer
{
	class SameThreadScheduler : public pplx::scheduler_interface
	{
		// Inherited via scheduler_interface
		virtual void schedule(pplx::TaskProc_t, void *);
	};
}