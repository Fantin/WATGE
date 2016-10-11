#include "stdafx.h"

#include "ComponentStore.h"

namespace WATGE
{
	ComponentStore::ComponentStore(size_t object_size, ComponentID_t initial_capacity, ComponentID_t page_size)
		: component_count_(0), page_size_(page_size), object_size_(object_size)
	{
		ComponentID_t page, entry;
		getPageEntry(initial_capacity, page, entry);
		// Allocate enough pages to hold initial_capacity
		while (page >= store_.size())
		{
			addPage();
		}
	}

	ComponentStore::~ComponentStore()
	{
		for (auto it = store_.begin(); it != store_.end(); ++it)
		{
			if (*it != nullptr)
			{
				free(*it);
			}
		}
	}

	void* ComponentStore::getComponent(ComponentID_t cid)
	{
		ComponentID_t page, entry;
		getPageEntry(cid, page, entry);
		// Check range
		return store_[page] + (entry * object_size_);
	}

	ComponentID_t ComponentStore::addComponent(EntityID_t eid)
	{
		// Check if full
		if (component_count_ == store_.size() * page_size_)
		{
			addPage();
		}
		ComponentID_t page, entry;
		getPageEntry(component_count_, page, entry);
		eids_[page][entry] = eid;
		// Increment count after returning previous count
		return component_count_++;
	}

	EntityID_t ComponentStore::removeComponent(ComponentID_t cid)
	{
		// Get pointer to final component
		ComponentID_t removePage, removeEntry;
		getPageEntry(cid, removePage, removeEntry);
		void* removeP = store_[removePage] + (removeEntry * object_size_);
		// Get pointer to target component
		ComponentID_t endPage, endEntry;
		getPageEntry(component_count_ - 1, endPage, endEntry);
		void* endP = store_[endPage] + (endEntry * object_size_);
		// Move final component to removed component
		memcpy(removeP, endP, object_size_);
		eids_[removePage][removeEntry] = eids_[endPage][endEntry];
		// Reduce count
		--component_count_;
		// Return moved eid to update lookup
		return eids_[removePage][removeEntry];
	}

	void ComponentStore::getPageEntry(ComponentID_t cid, ComponentID_t& page, ComponentID_t& entry)
	{
		page = cid / page_size_;
		entry = cid % page_size_;
	}

	void ComponentStore::addPage()
	{
		uint8_t* storePage = (uint8_t*)malloc(page_size_ * object_size_);
		EntityID_t* eidsPage = (EntityID_t*)malloc(page_size_ * sizeof(EntityID_t));
		store_.push_back(storePage);
		eids_.push_back(eidsPage);
	}
}