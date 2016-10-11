#pragma once

#include "EntityManager.h"

namespace WATGE
{
	class ISystem
	{
	public:
		void executeAll();
		bool addManager(EntityManager* em, eWATError& error);
		bool removeManager(EntityManager* em, eWATError& error);
	private:
	};
}