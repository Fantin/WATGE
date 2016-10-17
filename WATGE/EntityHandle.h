#pragma once

#include "WATCore.h"
#include "EntityManager.h"

namespace WATGE
{
	class EntityHandle
	{
	public:
		EntityID_t const eid_;
		EntityGUID_t const guid_;
		EntityManager* const em_;

		template <class T>
		bool hasComponent(eWATError& error);
		template <class T>
		T* getComponent(eWATError& error);
		template <class T>
		bool addComponent(eWATError& error);
		template <class T, class... Args>
		T* addCreateComponent(eWATError& error, Args... args);
		template <class T>
		bool removeComponent(eWATError& error);

		static EntityHandle makeEntity(EntityManager* em, eWATError& error);
		bool deleteEntity(eWATError& error);
	private:
		bool validGUID();
		EntityHandle(EntityID_t eid, EntityGUID_t guid, EntityManager* em);
	};

	template <class T>
	bool EntityHandle::hasComponent(eWATError& error)
	{
		if (!validGUID())
		{
			error = eEntityDoesNotExist;
			return false;
		}
		bool status = em_->hasComponent<T>(eid_);
		error = status ? eNoError : eComponentAlreadyExists;
		return status;
	}

	template <class T>
	T* EntityHandle::getComponent(eWATError& error)
	{
		if (!validGUID())
		{
			error = eEntityDoesNotExist;
			return nullptr;
		}
		T* component = em_->getComponent<T>(eid_);
		error = component != nullptr ? eNoError : eComponentDoesNotExist;
		return component;
	}

	template <class T>
	bool EntityHandle::addComponent(eWATError& error)
	{
		if (!validGUID())
		{
			error = eEntityDoesNotExist;
			return false;
		}
		bool success = em_->addComponent<T>(eid_);
		error = success ? eNoError : eComponentAlreadyExists;
		return success;
	}

	template <class T, class... Args>
	T* EntityHandle::addCreateComponent(eWATError& error, Args... args)
	{
		if (!validGUID())
		{
			error = eEntityDoesNotExist;
			return nullptr;
		}
		T* component = em_->addCreateComponent<T>(eid_, args...);
		error = component != nullptr ? eNoError : eComponentAlreadyExists;
		return component;
	}

	template <class T>
	bool EntityHandle::removeComponent(eWATError& error)
	{
		if (!validGUID())
		{
			error = eEntityDoesNotExist;
			return false;
		}
		bool success = em_->removeComponent<T>(eid_);
		error = success ? eNoError : eComponentDoesNotExist;
		return success;
	}
}
