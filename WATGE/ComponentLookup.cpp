#include "stdafx.h"

#include "ComponentLookup.h"

namespace WATGE
{
	ComponentLookup::ComponentLookup(EntityID_t initial_capacity, EntityID_t page_size) : page_size_(page_size)
	{
		EntityID_t page, entry;
		getPageEntry(initial_capacity, page, entry);
		// Allocate enough pages to hold initial_capacity
		while (page >= lookup_.size())
		{
			addPage();
		}
	}

	ComponentLookup::~ComponentLookup()
	{
		// Free all pages
		for (auto it = lookup_.begin(); it != lookup_.end(); ++it)
		{
			if (*it != nullptr)
			{
				free(*it);
			}
		}
	}

	ComponentID_t ComponentLookup::getEntry(EntityID_t eid)
	{
		EntityID_t page, entry;
		getPageEntry(eid, page, entry);
		// Check range
		return page < lookup_.size() ? lookup_[page][entry] : -1;
	}

	bool ComponentLookup::hasEntry(EntityID_t eid)
	{
		EntityID_t page, entry;
		getPageEntry(eid, page, entry);
		// Check range
		return page < lookup_.size() && lookup_[page][entry] != -1;
	}

	void ComponentLookup::editEntry(EntityID_t eid, ComponentID_t cid)
	{
		EntityID_t page, entry;
		getPageEntry(eid, page, entry);
		// Add pages if out of range
		while (page >= lookup_.size())
		{
			addPage();
		}
		lookup_[page][entry] = cid;
	}

	void ComponentLookup::clearEntry(EntityID_t eid)
	{
		// Set entry to -1
		editEntry(eid, -1);
	}

	void ComponentLookup::getPageEntry(EntityID_t eid, EntityID_t& page, EntityID_t& entry)
	{
		page = eid / page_size_;
		entry = eid % page_size_;
	}

	void ComponentLookup::addPage()
	{
		ComponentID_t* page = (ComponentID_t*)malloc(sizeof(page) * page_size_);
		for (EntityID_t i = 0; i < page_size_; ++i)
		{
			page[i] = -1;
		}
		lookup_.push_back(page);
	}
}