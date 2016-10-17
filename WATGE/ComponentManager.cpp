#ifdef _WIN32
#include "stdafx.h"
#endif

#include "ComponentManager.h"
#include <atomic>

namespace WATGE
{
	ComponentClassID_t IComponentManager::next_id()
	{
		static ComponentClassID_t next_id = 0;
		return next_id++;
	}

	IComponentManager::~IComponentManager()
	{
	}
}
