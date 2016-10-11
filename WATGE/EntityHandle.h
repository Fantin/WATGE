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
		T* getComponent(eWATError& error);
		template <class T>
		bool addComponent(eWATError& error);
		template <class T>
		bool removeComponent(eWATError& error);

		static EntityHandle makeEntity(EntityManager* em, eWATError& error);
		void deleteEntity(eWATError& error);
	private:
		EntityHandle(EntityID_t eid, EntityGUID_t guid, EntityManager* em);
	};

	template <class T>
	T* EntityHandle::getComponent(eWATError& error)
	{
		eWATError e;
		if (!em_->supportsComponentClass<T>(e))
		{
			error = eComponentClassNotSupported;
			return nullptr;
		}
		if (!em_->supportsEntity(eid_, guid_, e))
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
		eWATError e;
		if (!em_->supportsComponentClass<T>(e))
		{
			error = eComponentClassNotSupported;
			return false;
		}
		if (!em_->supportsEntity(eid_, guid_, e))
		{
			error = eEntityDoesNotExist;
			return false;
		}
		bool success = em_->addComponent<T>(eid_);
		error = success ? eNoError : eComponentAlreadyExists;
		return success;
	}

	template <class T>
	bool EntityHandle::removeComponent(eWATError& error)
	{
		eWATError e;
		if (!em_->supportsComponentClass<T>(e))
		{
			error = eComponentClassNotSupported;
			return false;
		}
		if (!em_->supportsEntity(eid_, guid_, e))
		{
			error = eEntityDoesNotExist;
			return false;
		}
		bool success = em_->removeComponent(eh.eid_);
		error = success ? eNoError : eComponentDoesNotExist;
		return success;
	}
}