#ifdef _WIN32
#include "stdafx.h"
#endif

#include "EntityManager.h"

namespace WATGE
{
	EntityManager::EntityManager()
	{

	}

	bool EntityManager::hasEntity(EntityID_t eid)
	{
		GUIDComponent* guidc = getComponent<GUIDComponent>(eid);
		return guidc != nullptr;
	}

	void EntityManager::makeEntity(EntityID_t& eid, EntityGUID_t& guid)
	{
		eid = id_manager_.takeID();
		addComponent<GUIDComponent>(eid);
		GUIDComponent* guidc = getComponent<GUIDComponent>(eid);
		guidc->guid_ = GUIDComponent::getNextGUID();
		guid = guidc->guid_;
	}	
	bool EntityManager::deleteEntity(EntityID_t eid)
	{
		bool success = removeComponent<GUIDComponent>(eid);

		GUIDComponent* guidc = getComponent<GUIDComponent>(eid);
		if (guidc == nullptr)
		{
			return false;
		}
		guidc->guid_ = -1;
		id_manager_.returnID(eid);
		return true;
	}
}
