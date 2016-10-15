#include "stdafx.h"

#include "ComponentManager.h"
#include <atomic>

namespace WATGE
{
	ComponentClassID_t IComponentManager::next_id()
	{
		static std::atomic<ComponentClassID_t> next_id = 0;
		return next_id++;
	}

	IComponentManager::~IComponentManager()
	{
	}
}
