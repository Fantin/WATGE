#pragma once

#include "Entity.h"
#include "Component.h"
#include "ComponentLookup.h"
#include "ComponentStore.h"

#include "Error.h"

#include <cstdint>
#include <type_traits>
#include <vector>

namespace WATGE
{
	class IComponentManager
	{
	public:
		virtual ~IComponentManager();
		virtual eWATError removeComponent(EntityID_t eid) = 0;
	protected:
		static ComponentClassID_t next_id;
	};

	template <class T>
	class ComponentManager : public IComponentManager
	{
	public:
		ComponentManager();
		ComponentManager(const ComponentManager&) = delete;

		static const ComponentClassID_t getID();

		eWATError getComponent(EntityID_t eid, T*& component);
		eWATError addComponent(EntityID_t eid, T*& component);
		eWATError removeComponent(EntityID_t eid) override;

	private:
		ComponentLookup lookup_;
		ComponentStore<T> store_;
	};

	template <class T>
	inline ComponentManager<T>::ComponentManager() :
		lookup_(), store_()
	{
	}

	template<class T>
	inline const ComponentClassID_t ComponentManager<T>::getID()
	{
		static const ComponentClassID_t class_id = next_id++;
		return class_id;
	}

	template<class T>
	inline eWATError ComponentManager<T>::getComponent(EntityID_t eid, T *& component)
	{
		// Search the lookup table
		ComponentID_t cid;
		lookup_.getEntry(eid, cid);
		if (cid == -1)
		{
			component = nullptr;
			return eComponentDoesNotExist;
		}
		store_.getComponent(cid, component);
		return eNoError;
	}

	template<class T>
	inline eWATError ComponentManager<T>::addComponent(EntityID_t eid, T *& component)
	{
		bool hasComponent;
		// Search the lookup table
		lookup_.hasEntry(eid, hasComponent);
		// Return if component already exists
		if (hasComponent)
		{
			component = nullptr;
			return eComponentAlreadyExists;
		}
		// Add the component to the store
		ComponentID_t cid;
		store_.addComponent(eid, component, cid);
		// Add the component to the lookup table
		lookup_.addEntry(eid, cid);
		return eNoError;
	}

	template<class T>
	inline eWATError ComponentManager<T>::removeComponent(EntityID_t eid)
	{
		// Search the lookup table
		ComponentID_t cid;
		lookup_.getEntry(eid, cid);
		if (cid == -1)
		{
			return eComponentDoesNotExist;
		}
		// Remove from the store
		EntityID_t moved_eid;
		store_.removeComponent(cid, moved_eid);
		// Update the lookup table
		lookup_.removeEntry(eid);
		// Check if last was removed
		if (eid != moved_eid)
		{
			lookup_.changeEntry(moved_eid, cid);
		}
		return eNoError;
	}
}