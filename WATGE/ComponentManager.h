#pragma once

#include "WATCore.h"

#include "ComponentLookup.h"
#include "ComponentStore.h"

#include <cstdint>
#include <type_traits>
#include <vector>

namespace WATGE
{
	class IComponentManager
	{
	public:
		virtual ~IComponentManager();
		virtual bool removeComponent(EntityID_t eid) = 0;
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

		T* getComponent(EntityID_t eid);
		bool addComponent(EntityID_t eid);
		bool removeComponent(EntityID_t eid) override;

	private:
		ComponentLookup lookup_;
		ComponentStore store_;
	};

	template <class T>
	inline ComponentManager<T>::ComponentManager() :
		lookup_(), store_(sizeof(T))
	{
	}

	template<class T>
	inline const ComponentClassID_t ComponentManager<T>::getID()
	{
		static const ComponentClassID_t class_id = next_id++;
		return class_id;
	}

	template<class T>
	inline T* ComponentManager<T>::getComponent(EntityID_t eid)
	{
		// Search the lookup table
		ComponentID_t cid = lookup_.getEntry(eid);
		return cid != -1 ? (T*) store_.getComponent(cid) : nullptr;
	}

	template<class T>
	inline bool ComponentManager<T>::addComponent(EntityID_t eid)
	{
		// Check if component already exists
		if (lookup_.hasEntry(eid))
		{
			// Operation failed
			return false;
		}
		// Add component
		ComponentID_t cid = store_.addComponent(eid);
		// Add id to lookup
		lookup_.editEntry(eid, cid);
		// Operation succeeded
		return true;
	}

	template<class T>
	inline bool ComponentManager<T>::removeComponent(EntityID_t eid)
	{
		//// Search the lookup table
		//ComponentID_t cid;
		//lookup_.getEntry(eid, cid);
		//if (cid == -1)
		//{
		//	return eComponentDoesNotExist;
		//}
		//// Remove from the store
		//EntityID_t moved_eid;
		//store_.removeComponent(cid, moved_eid);
		//// Update the lookup table
		//lookup_.clearEntry(eid);
		//// Check if last was removed
		//if (eid != moved_eid)
		//{
		//	lookup_.editEntry(moved_eid, cid);
		//}
		//return eNoError;

		// Check if component exists
		if (!lookup_.hasEntry(eid))
		{
			// Operation failed
			return false;
		}
		ComponentID_t cid = lookup_.getEntry(eid);
		// Remove component based off lookup id
		EntityID_t moved_eid = store_.removeComponent(lookup_.getEntry(eid));
		// Edit moved component entry
		lookup_.editEntry(moved_eid, cid);
		// Clear old entry
		lookup_.clearEntry(eid);
		// Operation succeeded
		return true;
	}
}