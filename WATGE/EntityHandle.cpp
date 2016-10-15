#include "stdafx.h"
#include "EntityHandle.h"

namespace WATGE
{
	bool EntityHandle::validGUID()
	{
		GUIDComponent* guidc = em_->getComponent<GUIDComponent>(eid_);
		return guidc != nullptr && guidc->guid_== guid_;
	}

	EntityHandle::EntityHandle(EntityID_t eid, EntityGUID_t guid, EntityManager* em)
		: guid_(guid), eid_(eid), em_(em)
	{
	}

	EntityHandle EntityHandle::makeEntity(EntityManager* em, eWATError& error)
	{
		EntityID_t eid;
		EntityGUID_t guid;
		em->makeEntity(eid, guid);
		error = eNoError;
		return EntityHandle(eid, guid, em);
	}

	bool EntityHandle::deleteEntity(eWATError& error)
	{
		if (!validGUID())
		{
			error = eEntityDoesNotExist;
			return false;
		}
		bool success = em_->deleteEntity(eid_);
		error = success ? eNoError : eEntityDoesNotExist;
		return success;
	}
}