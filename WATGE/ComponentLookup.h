#pragma once

#include "WATCore.h"
#include <vector>

namespace WATGE
{
	class ComponentLookup
	{
	public:
		ComponentLookup(EntityID_t initial_capacity = 100, EntityID_t page_size = 50);
		ComponentLookup(const ComponentLookup&) = delete;
		~ComponentLookup();

		ComponentID_t getEntry(EntityID_t eid);
		bool hasEntry(EntityID_t eid);
		void editEntry(EntityID_t eid, ComponentID_t cid);
		void clearEntry(EntityID_t eid);

	private:

		const EntityID_t page_size_;
		std::vector<ComponentID_t*> lookup_;

		void getPageEntry(EntityID_t eid, EntityID_t& page, EntityID_t& entry);
		void addPage();
	};
}