#pragma once

#include "WATCore.h"

#include "Entity.h"
#include "EntityIDManager.h"
#include "GUIDComponent.h"
#include "ComponentManager.h"
#include <cstdint>
#include <vector>

namespace WATGE
{
	struct EntityHandle
	{
		EntityGUID_t guid_;
		EntityID_t eid_;
	};
	
	class EntityManager
	{
	public:
		template <class... Ts>
		static eWATError makeManager(EntityManager*& em);

		eWATError supportsEntity(EntityHandle eh, bool& status);
		eWATError makeEntity(EntityHandle& eh);
		eWATError removeEntity(EntityHandle eh);

		template <class T>
		eWATError supportsComponentClass(bool& status);
		template <class T>
		eWATError getComponent(EntityHandle eh, T*& component);
		template <class T>
		eWATError addComponent(EntityHandle eh, T*& component);
		template <class T>
		eWATError removeComponent(EntityHandle eh);

		template <class T>
		eWATError addComponentClass();
	private:
		template <class... Ts>
		struct MakeManagerHelper;
		template <>
		struct MakeManagerHelper<>
		{
			static void makeManagerHelper(EntityManager* em);
		};
		template <class T, class... Ts>
		struct MakeManagerHelper<T, Ts...>
		{
			static void makeManagerHelper(EntityManager* em);
		};

		template <class T>
		eWATError getComponent(EntityID_t eid, T*& component);
		template <class T>
		eWATError addComponent(EntityID_t eid, T*& component);
		template <class T>
		eWATError removeComponent(EntityID_t eid);

		EntityIDManager id_manager_;
		std::vector<IComponentManager*> managers_;
	};

	template <class... Ts>
	eWATError EntityManager::makeManager(EntityManager*& em)
	{
		em = new EntityManager();
		em->addComponentClass<GUIDComponent>();
		MakeManagerHelper<Ts...>::makeManagerHelper(em);
		return eNoError;
	}

	template <class T>
	eWATError EntityManager::supportsComponentClass(bool& status)
	{
		ComponentClassID_t class_id = ComponentManager<T>::getID();
		status = (managers_.size() > class_id) || (managers_[class_id] != nullptr);
		return eNoError;
	}

	template <class T>
	eWATError EntityManager::getComponent(EntityHandle eh, T*& component)
	{
		bool support;
		eWATError e = supportsEntity(eh, support);
		if (!support)
		{
			component == nullptr;
			return e;
		}
		return getComponent(eh.eid_, component);
	}
	
	template <class T>
	eWATError EntityManager::addComponent(EntityHandle eh, T*& component)
	{
		bool support;
		supportsEntity(eh, support);
		if (!support)
		{
			component == nullptr;
			return eEntityDoesNotExist;
		}
		return addComponent(eh.eid_, component);
	}

	template <class T>
	eWATError EntityManager::removeComponent(EntityHandle eh)
	{
		bool support;
		supportsEntity(eh, support);
		if (!support)
		{
			component == nullptr;
			return eEntityDoesNotExist;
		}
		return removeComponent(eh.eid_);
	}

	template <class T, class... Ts>
	void EntityManager::MakeManagerHelper<T, Ts...>::makeManagerHelper(EntityManager* em)
	{
		em->addComponentClass<T>();
		MakeManagerHelper<Ts...>();
	}

	template <class T>
	eWATError EntityManager::getComponent(EntityID_t eid, T*& component)
	{
		bool support;
		eWATError e = supportsComponentClass<T>(support);
		if (!support)
		{
			return e;
		}
		ComponentManager<T>* cm = 
			static_cast<ComponentManager<T>*> (managers_[ComponentManager<T>::getID()]);
		return cm->getComponent(eid, component);
	}

	template <class T>
	eWATError EntityManager::addComponent(EntityID_t eid, T*& component)
	{
		bool support;
		eWATError e = supportsComponentClass<T>(support);
		if (!support)
		{
			return e;
		}
		ComponentManager<T>* cm =
			static_cast<ComponentManager<T>*>(managers_[ComponentManager<T>::getID()]);
		return cm->addComponent(eid, component);
	}

	template <class T>
	eWATError EntityManager::removeComponent(EntityID_t eid)
	{
		bool support;
		eWATError e = supportsComponentClass<T>(support);
		if (!support)
		{
			return e;
		}
		ComponentManager<T>* cm =
			static_cast<ComponentManager<T>*> managers_[ComponentManager<T>::getID()];
		return cm->removeComponent(eid);
	}

	template <class T>
	eWATError EntityManager::addComponentClass()
	{
		ComponentClassID_t class_id = ComponentManager<T>::getID();
		while (managers_.size() <= class_id)
		{
			managers_.push_back(nullptr);
		}
		if (managers_[class_id] != nullptr)
		{
			return eComponentClassAlreadySupported;
		}
		managers_[class_id] = new ComponentManager<T>();
		return eNoError;
	}
}
