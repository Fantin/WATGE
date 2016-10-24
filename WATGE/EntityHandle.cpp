#include "EntityHandle.h"

namespace WATGE
{
	bool EntityHandle::isValid(eWATError& error) const
	{
		EntityManager* em = getManager(error);
		if(em == nullptr)
		{
			return false;
		}
		GUIDComponent* guidc = em->getComponent<GUIDComponent>(eid_);
		if(guidc == nullptr || guidc->guid_ != guid_)
		{
			error = eEntityDoesNotExist;
			return false;
		}
		return true;
	}

	EntityManager* EntityHandle::getManager(eWATError& error) const
	{
			EntityManager* em = emid_ < EntityManager::entity_managers.size() ?
				EntityManager::entity_managers[emid_] : nullptr;
			if(em == nullptr)
			{
				error = eEntityManagerDoesNotExist;
				return nullptr;
			}
			error = eNoError;
			return em;
	}

	EntityHandle::EntityHandle(EntityID_t eid, EntityGUID_t guid, EntityManagerID_t emid)
		: guid_(guid), eid_(eid), emid_(emid)
	{
	}

	EntityHandle makeEntity(eWATError& error, EntityManagerID_t emid)
	{
		EntityID_t eid;
		EntityGUID_t guid;
		em->makeEntity(eid, guid);

		error = eNoError;
		return EntityHandle(eid, guid, emid);
	}

	bool deleteEntity(const EntityHandle& eh, eWATError& error)
	{
		EntityManager* em = eh.getManager(error);
		if(em == nullptr)
		{
			return false;
		}
		if (!eh.isValid(error))
		{
			return false;
		}
		bool success = em->deleteEntity(eh.eid_);
		error = success ? eNoError : eEntityDoesNotExist;
		return success;
	}
}
