#include "stdafx.h"
#include "EntityHandle.h"

namespace WATGE
{
	EntityHandle::EntityHandle(EntityID_t eid, EntityGUID_t guid, EntityManager* em)
		: guid_(guid), eid_(eid), em_(em)
	{
	}

	EntityHandle EntityHandle::makeEntity(EntityManager* em, eWATError& error)
	{
		if (em == nullptr)
		{
			error = eManagerNotSupported;
			return EntityHandle(-1, 0, nullptr);
		}
		EntityID_t eid;
		EntityGUID_t guid;
		em->makeEntity(eid, guid);
		error = eNoError;
		return EntityHandle(eid, guid, em);
	}

	void EntityHandle::deleteEntity(eWATError& error)
	{
		error = em_->removeEntity(eid_, guid_) ? eNoError : eEntityDoesNotExist;
	}
}