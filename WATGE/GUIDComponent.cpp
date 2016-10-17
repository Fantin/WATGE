#ifdef _WIN32
#include "stdafx.h"
#endif

#include "GUIDComponent.h"

namespace WATGE
{
	EntityGUID_t GUIDComponent::getNextGUID()
	{
		static EntityGUID_t next_id = 0;
		return next_id++;
	}
}
