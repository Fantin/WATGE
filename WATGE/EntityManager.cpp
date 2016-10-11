#include "stdafx.h"

#include "EntityManager.h"

namespace WATGE
{
	void EntityManager::MakeManagerHelper<>::makeManagerHelper(EntityManager* em)
	{
		return;
	}

	bool EntityManager::supportsEntity(EntityID_t eid, EntityGUID_t guid, eWATError& error)
	{
		GUIDComponent* guidc = getComponent<GUIDComponent>(eid);
		// Check component existence and match
		error = eNoError;
		return guidc != nullptr && guidc->guid_ == guid;
	}

	void EntityManager::makeEntity(EntityID_t& eid, EntityGUID_t& guid)
	{
		eid = id_manager_.takeID();
		addComponent<GUIDComponent>(eid);
		GUIDComponent* guidc = getComponent<GUIDComponent>(eid);
		guidc->guid_ = GUIDComponent::getNextGUID();
		guid = guidc->guid_;
	}	
	bool EntityManager::removeEntity(EntityID_t eid, EntityGUID_t guid)
	{
		GUIDComponent* guidc = getComponent<GUIDComponent>(eid);
		if (guidc == nullptr || guidc->guid_ != guid)
		{
			return false;
		}
		for (auto it = managers_.begin(); it != managers_.end(); ++it)
		{
			if ((*it) != nullptr)
			{
				(*it)->removeComponent(eid);
			}
		}
		id_manager_.returnID(eid);
		return true;
	}
}