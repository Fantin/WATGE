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
		EntityManager();
		
		bool hasEntity(EntityID_t eid);
		void makeEntity(EntityID_t& eid, EntityGUID_t& guid);
		bool deleteEntity(EntityID_t eid);

		template <class T>
		bool hasComponent(EntityID_t eid);
		template <class T>
		T* getComponent(EntityID_t eid);
		template <class T>
		bool addComponent(EntityID_t eid);
		template <class T, class... Args>
		T* addCreateComponent(EntityID_t eid, Args... args);
		template <class T>
		bool removeComponent(EntityID_t eid);

	private:
		EntityIDManager id_manager_;
		std::vector<IComponentManager*> managers_;
	};

	template <class T>
	bool EntityManager::hasComponent(EntityID_t eid)
	{
		ComponentClassID_t class_id = ComponentManager<T>::getID();
		if (class_id >= managers_.size())
		{
			return false;
		}
		ComponentManager<T>* cm =
			static_cast<ComponentManager<T>*> (managers_[class_id]);
		return cm->hasComponent(eid);
	}

	template <class T>
	T* EntityManager::getComponent(EntityID_t eid)
	{
		ComponentClassID_t class_id = ComponentManager<T>::getID();
		if (class_id >= managers_.size())
		{
			return false;
		}
		ComponentManager<T>* cm = 
			static_cast<ComponentManager<T>*> (managers_[class_id]);
		return cm->getComponent(eid);
	}

	template <class T>
	bool EntityManager::addComponent(EntityID_t eid)
	{
		ComponentClassID_t class_id = ComponentManager<T>::getID();
		if (class_id >= managers_.size())
		{
			managers_.resize(class_id + 1, nullptr);
			managers_[class_id] = new ComponentManager<T>();
		}
		ComponentManager<T>* cm =
			static_cast<ComponentManager<T>*> (managers_[class_id]);
		return cm->addComponent(eid);
	}

	template <class T, class... Args>
	T* EntityManager::addCreateComponent(EntityID_t eid, Args... args)
	{
		ComponentClassID_t class_id = ComponentManager<T>::getID();
		if (class_id >= managers_.size())
		{
			managers_.resize(class_id + 1, nullptr);
			managers_[class_id] = new ComponentManager<T>();
		}
		ComponentManager<T>* cm =
			static_cast<ComponentManager<T>*> (managers_[class_id]);
		return cm->addCreateComponent(eid, args...);
	}

	template <class T>
	bool EntityManager::removeComponent(EntityID_t eid)
	{
		ComponentClassID_t class_id = ComponentManager<T>::getID();
		if (class_id >= managers_.size())
		{
			return false;
		}
		ComponentManager<T>* cm =
			static_cast<ComponentManager<T>*> (managers_[class_id]);
		return cm->removeComponent(eid);
	}
}