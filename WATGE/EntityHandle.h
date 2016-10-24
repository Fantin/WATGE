#pragma once

#include "WATCore.h"
#include "EntityManager.h"

namespace WATGE
{
	class EntityHandle
	{
	public:
		const EntityID_t eid_;
		const EntityGUID_t guid_;
		const EntityManagerID_t emid_;

		bool isValid(eWATError& error) const;
		EntityManager* getManager(eWATError& error) const;
	private:
		EntityHandle(EntityID_t eid, EntityGUID_t guid, EntityManagerID_t emid);
	};

	template <class T>
	bool hasComponent(const EntityHandle& eh, eWATError& error);
	template <class T>
	T* getComponent(const EntityHandle& eh, eWATError& error);
	template <class T>
	bool addComponent(const EntityHandle& eh, eWATError& error);
	template <class T, class... Args>
	T* addCreateComponent(const EntityHandle& eh, eWATError& error, Args... args);
	template <class T>
	bool removeComponent(const EntityHandle& eh, eWATError& error);

	static EntityHandle makeEntity(eWATError& error, EntityManagerID_t emid = 0);
	bool deleteEntity(const EntityHandle& eh, eWATError& error);

	template <class T>
	bool hasComponent(const EntityHandle& eh, eWATError& error)
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
		bool status = em->hasComponent<T>(eh.eid_);
		error = status ? eNoError : eComponentAlreadyExists;
		return status;
	}

	template <class T>
	T* getComponent(const EntityHandle& eh, eWATError& error)
	{
		EntityManager* em = eh.getManager(error);
		if(em == nullptr)
		{
			return false;
		}
		if (!eh.isValid(error))
		{
			return nullptr;
		}
		T* component = em->getComponent<T>(eh.eid_);
		error = component != nullptr ? eNoError : eComponentDoesNotExist;
		return component;
	}

	template <class T>
	bool addComponent(const EntityHandle& eh, eWATError& error)
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
		bool success = em->addComponent<T>(eh.eid_);
		error = success ? eNoError : eComponentAlreadyExists;
		return success;
	}

	template <class T, class... Args>
	T* addCreateComponent(const EntityHandle& eh, eWATError& error, Args... args)
	{
		EntityManager* em = eh.getManager(error);
		if(em == nullptr)
		{
			return false;
		}
		if (!eh.isValid(error))
		{
			return nullptr;
		}
		T* component = em->addCreateComponent<T>(eh.eid_, args...);
		error = component != nullptr ? eNoError : eComponentAlreadyExists;
		return component;
	}

	template <class T>
	bool removeComponent(const EntityHandle& eh, eWATError& error)
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
		bool success = em->removeComponent<T>(eh.eid_);
		error = success ? eNoError : eComponentDoesNotExist;
		return success;
	}
}
