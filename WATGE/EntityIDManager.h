#pragma once

#include "WATCore.h"

#include <vector>

namespace WATGE
{
	class EntityIDManager
	{
	public:
		EntityID_t takeID();
		void returnID(EntityID_t eid);

		EntityIDManager(std::size_t page_size = 50);
		EntityIDManager(const EntityIDManager&) = delete;
		~EntityIDManager();

	private:
		EntityID_t next_id_;
		EntityID_t free_id_count_;
		const std::size_t page_size_;
		std::vector<EntityID_t*> free_ids_;

		void getPageEntry(std::size_t index, std::size_t& page, std::size_t& entry);
		void addPage();
		void addPages(std::size_t pages);
	};
}
