#include "stdafx.h"

#include "ComponentLookup.h"

namespace WATGE
{
	ComponentLookup::ComponentLookup(EntityID_t initial_capacity, EntityID_t page_size) : page_size_(page_size)
	{
		// Allocate enough pages to hold initial_capacity
		EntityID_t pagesToAdd = initial_capacity / page_size;
		if (initial_capacity % page_size != 0)
		{
			++pagesToAdd;
		}
		addPages(pagesToAdd);
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

	eWATError ComponentLookup::getEntry(EntityID_t eid, ComponentID_t& cid)
	{
		EntityID_t page, entry;
		getPageEntry(eid, page, entry);
		// Check range and state
		if (page >= lookup_.size() ||
			lookup_[page][entry] == -1)
		{
			cid = -1;
			return eComponentDoesNotExist;
		}
		cid = lookup_[page][entry];
		return eNoError;
	}

	eWATError ComponentLookup::hasEntry(EntityID_t eid, bool& status)
	{
		EntityID_t page, entry;
		getPageEntry(eid, page, entry);
		// Check range
		if (page >= lookup_.size() ||
			lookup_[page][entry] == -1)
		{
			status = false;
			return eNoError;
		}
		status = true;
		return eNoError;
	}

	eWATError ComponentLookup::addEntry(EntityID_t eid, ComponentID_t cid)
	{
		EntityID_t page, entry;
		getPageEntry(eid, page, entry);
		// Add pages if out of range
		if (page >= lookup_.size())
		{
			addPages(1 + page - lookup_.size());
		}
		// Check if already has component
		if (lookup_[page][entry] != -1)
		{
			return eComponentAlreadyExists;
		}
		lookup_[page][entry] = cid;
		return eNoError;
	}

	eWATError ComponentLookup::removeEntry(EntityID_t eid)
	{
		EntityID_t page, entry;
		getPageEntry(eid, page, entry);
		// Check range and state
		if (page >= lookup_.size() ||
			lookup_[page][entry] == -1)
		{
			return eComponentDoesNotExist;
		}
		lookup_[page][entry] = -1;
		return eNoError;
	}

	eWATError ComponentLookup::changeEntry(EntityID_t eid, ComponentID_t cid)
	{
		EntityID_t page, entry;
		getPageEntry(eid, page, entry);
		// Check range and state
		if (page >= lookup_.size() ||
			lookup_[page][entry] == -1)
		{
			return eComponentDoesNotExist;
		}
		lookup_[page][entry] = cid;
		return eNoError;
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

	void ComponentLookup::addPages(EntityID_t pages)
	{
		for (EntityID_t i = 0; i < pages; ++i)
		{
			addPage();
		}
	}
}