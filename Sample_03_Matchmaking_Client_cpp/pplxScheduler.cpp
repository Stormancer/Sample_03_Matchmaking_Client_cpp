#include "stdafx.h"
#include "pplxScheduler.h"

void Stormancer::SameThreadScheduler::schedule(pplx::TaskProc_t task, void * param)
{
	task(param);
}
