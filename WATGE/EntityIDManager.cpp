#include "stdafx.h"

#include "EntityIDManager.h"

namespace WATGE
{
	eWATError EntityIDManager::takeID(EntityID_t& eid)
	{
		// Assign and increment if none are available
		if (free_id_count_ == 0)
		{
			eid = next_id_++;
			return eNoError;
		}
		// Otherwise remove and use the last one that was returned
		size_t page, entry;
		getPageEntry(free_id_count_ - 1, page, entry);
		eid = free_ids_[page][entry];
		--free_id_count_;
		return eNoError;
	}

	eWATError EntityIDManager::returnID(EntityID_t eid)
	{
		// Add a page if no room
		if(free_id_count_ == free_ids_.size() * page_size_)
		{
			addPage();
		}
		size_t page, entry;
		getPageEntry(free_id_count_, page, entry);
		free_ids_[page][entry] = eid;
		++free_id_count_;
		return eNoError;
	}

	// TODO: Initial page count
	EntityIDManager::EntityIDManager(size_t page_size) : page_size_(page_size)
	{
		next_id_ = 0;
		free_id_count_ = 0;
	}

	EntityIDManager::~EntityIDManager()
	{
		// Free each page
		for (auto it = free_ids_.begin(); it != free_ids_.end(); ++it)
		{
			free(*it);
		}
	}

	void EntityIDManager::getPageEntry(size_t index, size_t & page, size_t & entry)
	{
		page = index / page_size_;
		entry = index % page_size_;
	}

	void EntityIDManager::addPage()
	{
		// No need to initialize anything here
		EntityID_t* page = (EntityID_t*)malloc(sizeof(EntityID_t) * page_size_);
		free_ids_.push_back(page);
	}

	void EntityIDManager::addPages(size_t pages)
	{
		for (size_t i = 0; i < pages; ++i)
		{
			addPage();
		}
	}
}