#ifdef _WIN32
#include "stdafx.h"
#endif

#include "System.h"

namespace WATGE
{
	void ISystem::executeAll()
	{
	}

	bool ISystem::addManager(EntityManager* em, eWATError& error)
	{
		return false;
	}

	bool ISystem::removeManager(EntityManager* em, eWATError& error)
	{
		return true;
	}
}
