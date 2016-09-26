#include "stdafx.h"

#include "EntityManager.h"

namespace WATGE
{
	eWATError EntityManager::supportsEntity(EntityHandle eh, bool& status)
	{
		GUIDComponent* guidc;
		eWATError e = getComponent(eh.eid_, guidc);
		if (e != eNoError)
		{
			status = false;
			return eEntityDoesNotExist;
		}
		if (guidc->guid_ != eh.guid_)
		{
			status = false;
			return eEntityDoesNotExist;
		}
		status = true;
		return eNoError;
	}

	eWATError EntityManager::makeEntity(EntityHandle& eh)
	{
		EntityID_t eid;
		eWATError e = id_manager_.takeID(eid);
		if (e != eNoError)
		{
			return e;
		}
		GUIDComponent* guidc;
		e = addComponent<GUIDComponent>(eid, guidc);
		if (e != eNoError)
		{
			return e;
		}
		guidc->guid_ = GUIDComponent::getNextGUID();

		eh.eid_ = eid;
		eh.guid_ = guidc->guid_;

		return eNoError;
	}

	eWATError EntityManager::removeEntity(EntityHandle eh)
	{
		bool status;
		eWATError e = supportsEntity(eh, status);
		if (e != eNoError)
		{
			return e;
		}
		for (auto it = managers_.begin(); it != managers_.end(); ++it)
		{
			(*it)->removeComponent(eh.eid_);
		}
		id_manager_.returnID(eh.eid_);
		return eNoError;
	}

	void EntityManager::MakeManagerHelper<>::makeManagerHelper(EntityManager* em)
	{
		return;
	}
}