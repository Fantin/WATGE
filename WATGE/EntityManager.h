#pragma once

#include "WATCore.h"

#include "EntityIDManager.h"
#include "GUIDComponent.h"
#include "ComponentManager.h"
#include <cstdint>
#include <vector>

namespace WATGE
{
	class EntityManager
	{
	public:
		template <class... Ts>
		static EntityManager* makeManager(eWATError& error);

		bool supportsEntity(EntityID_t eid, EntityGUID_t guid, eWATError& error);
		template <class T>
		bool supportsComponentClass(eWATError& error);
		template <class T>
		bool addComponentClass(eWATError& error);

	private:
		friend class EntityHandle;
		template <class T>
		T* getComponent(EntityID_t eid);
		template <class T>
		bool addComponent(EntityID_t eid);
		template <class T>
		bool removeComponent(EntityID_t eid);

		void makeEntity(EntityID_t& eid, EntityGUID_t& guid);
		bool removeEntity(EntityID_t eid, EntityGUID_t guid);

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
		
		EntityIDManager id_manager_;
		std::vector<IComponentManager*> managers_;
	};

	template <class... Ts>
	EntityManager* EntityManager::makeManager(eWATError& error)
	{
		eWATError e;
		EntityManager* em = new EntityManager();
		em->addComponentClass<GUIDComponent>(e);
		MakeManagerHelper<Ts...>::makeManagerHelper(em);
		error = eNoError;
		return em;
	}

	template <class T, class... Ts>
	void EntityManager::MakeManagerHelper<T, Ts...>::makeManagerHelper(EntityManager* em)
	{
		eWATError e;
		em->addComponentClass<T>(e);
		MakeManagerHelper<Ts...>::makeManagerHelper(em);
	}

	template <class T>
	bool EntityManager::supportsComponentClass(eWATError& error)
	{
		ComponentClassID_t class_id = ComponentManager<T>::getID();
		error = eNoError;
		return (class_id < managers_.size()) && (managers_[class_id] != nullptr);
	}

	template <class T>
	bool EntityManager::addComponentClass(eWATError& error)
	{
		ComponentClassID_t class_id = ComponentManager<T>::getID();
		while (class_id >= managers_.size())
		{
			managers_.push_back(nullptr);
		}
		if (managers_[class_id] != nullptr)
		{
			error = eComponentClassAlreadySupported;
			return false;
		}
		managers_[class_id] = new ComponentManager<T>();
		error = eNoError;
		return true;
	}

	template <class T>
	T* EntityManager::getComponent(EntityID_t eid)
	{
		ComponentManager<T>* cm = 
			static_cast<ComponentManager<T>*> (managers_[ComponentManager<T>::getID()]);
		return cm->getComponent(eid);
	}

	template <class T>
	bool EntityManager::addComponent(EntityID_t eid)
	{
		ComponentManager<T>* cm =
			static_cast<ComponentManager<T>*>(managers_[ComponentManager<T>::getID()]);
		return cm->addComponent(eid);
	}

	template <class T>
	bool EntityManager::removeComponent(EntityID_t eid)
	{
		ComponentManager<T>* cm =
			static_cast<ComponentManager<T>*>(managers_[ComponentManager<T>::getID()]);
		return cm->removeComponent(eid);
	}
}