#include "stdafx.h"

#include "ComponentManager.h"

namespace WATGE
{
	ComponentClassID_t IComponentManager::next_id = 0;

	IComponentManager::~IComponentManager()
	{
	}
}
