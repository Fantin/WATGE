#include "stdafx.h"
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