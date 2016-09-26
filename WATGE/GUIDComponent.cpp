#include "stdafx.h"

#include "GUIDComponent.h"

namespace WATGE
{
	EntityGUID_t GUIDComponent::getNextGUID()
	{
		static EntityGUID_t next_id = 0;
		return next_id++;
	}
}