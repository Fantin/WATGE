#pragma once

#include "Entity.h"
#include "Component.h"

#include "Error.h"

#include <vector>

namespace WATGE
{
	class ComponentLookup
	{
	public:
		ComponentLookup(EntityID_t initial_capacity = 100, EntityID_t page_size = 50);
		ComponentLookup(const ComponentLookup&) = delete;
		~ComponentLookup();

		eWATError getEntry(EntityID_t eid, ComponentID_t& cid);
		eWATError hasEntry(EntityID_t eid, bool& status);
		eWATError addEntry(EntityID_t eid, ComponentID_t cid);
		eWATError removeEntry(EntityID_t eid);
		eWATError changeEntry(EntityID_t eid, ComponentID_t cid);

	private:

		const EntityID_t page_size_;
		std::vector<ComponentID_t*> lookup_;

		void getPageEntry(EntityID_t eid, EntityID_t& page, EntityID_t& entry);
		void addPage();
		void addPages(EntityID_t pages);
	};
}