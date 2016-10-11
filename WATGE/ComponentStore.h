#pragma once

#include "WATCore.h"
#include <vector>

namespace WATGE
{
	class ComponentStore
	{
	public:
		ComponentStore(size_t object_size, ComponentID_t initial_capacity = 100, ComponentID_t page_size = 50);
		ComponentStore(const ComponentStore&) = delete;
		~ComponentStore();

		void* getComponent(ComponentID_t cid);
		ComponentID_t addComponent(EntityID_t eid);
		EntityID_t removeComponent(ComponentID_t cid);

	private:
		ComponentID_t component_count_;
		const ComponentID_t page_size_;
		const size_t object_size_;
		std::vector<uint8_t*> store_;
		std::vector<EntityID_t*> eids_;

		void getPageEntry(ComponentID_t cid, ComponentID_t& page, ComponentID_t& entry);
		void addPage();
	};
}