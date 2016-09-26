#pragma once

#include "Entity.h"
#include "Component.h"

#include "Error.h"

#include <vector>

namespace WATGE
{
	template <class T>
	class ComponentStore
	{
	public:
		ComponentStore(ComponentID_t initial_capacity = 100, ComponentID_t page_size = 50);
		ComponentStore(const ComponentStore&) = delete;
		~ComponentStore();

		eWATError getComponent(ComponentID_t cid, T*& component);
		eWATError addComponent(EntityID_t eid, T*& component, ComponentID_t& cid);
		eWATError removeComponent(ComponentID_t cid, EntityID_t& moved_eid);

	private:
		struct ComponentContainer
		{
			EntityID_t owner_;
			T component_;

			ComponentContainer() = delete;
			ComponentContainer(const ComponentContainer&) = delete;
		};
		//static_assert(std::is_pod<ComponentContainer>::value, "Container must be POD.");

		ComponentID_t component_count_;
		const ComponentID_t page_size_;
		std::vector<ComponentContainer*> store_;

		void getPageEntry(ComponentID_t cid, ComponentID_t& page, ComponentID_t& entry);
		void addPage();
		void addPages(ComponentID_t pages);
	};

	template<class T>
	inline ComponentStore<T>::ComponentStore(ComponentID_t initial_capacity, ComponentID_t page_size) : page_size_(page_size)
	{
		component_count_ = 0;
		// Allocate enough pages to hold initial_capacity
		ComponentID_t pagesToAdd = initial_capacity / page_size;
		if (initial_capacity % page_size != 0)
		{
			++pagesToAdd;
		}
		addPages(pagesToAdd);
	}

	template<class T>
	inline ComponentStore<T>::~ComponentStore()
	{
		for (auto it = store_.begin(); it != store_.end(); ++it)
		{
			if (*it != nullptr)
			{
				free(*it);
			}
		}
	}

	template<class T>
	inline eWATError ComponentStore<T>::getComponent(ComponentID_t cid, T*& component)
	{
		// Check if out of range
		if (cid >= component_count_)
		{
			// This shouldn't happen in production (safeguarded by ComponentManager)
			component = nullptr;
			return eGeneralError;
		}
		ComponentID_t page, entry;
		getPageEntry(cid, page, entry);
		T obj = store_[page][entry].component_;
		component = &(store_[page][entry].component_);
		return eNoError;
	}

	template<class T>
	inline eWATError ComponentStore<T>::addComponent(EntityID_t eid, T*& component, ComponentID_t& cid)
	{
		// Check if full
		if (component_count_ == store_.size() * page_size_)
		{
			addPage();
		}
		ComponentID_t page, entry;
		getPageEntry(component_count_, page, entry);
		store_[page][entry].owner_ = eid;
		component = &(store_[page][entry].component_);
		cid = component_count_;
		++component_count_;
		return eNoError;
	}

	template<class T>
	inline eWATError ComponentStore<T>::removeComponent(ComponentID_t cid, EntityID_t& moved_eid)
	{
		// Check if out of range
		if (cid >= component_count_)
		{
			// This shouldn't happen in production (safeguarded by ComponentManager)
			moved_eid = -1;
			return eGeneralError;
		}
		ComponentID_t removePage, removeEntry;
		getPageEntry(cid, removePage, removeEntry);
		ComponentID_t endPage, endEntry;
		getPageEntry(component_count_ - 1, endPage, endEntry);
		// Move final element to removed element
		store_[removePage][removeEntry] = store_[endPage][endEntry];
		// Return moved eid to update lookup
		moved_eid = store_[removePage][removeEntry].owner_;
		--component_count_;
		return eNoError;
	}

	template<class T>
	inline void ComponentStore<T>::getPageEntry(ComponentID_t cid, ComponentID_t& page, ComponentID_t& entry)
	{
		page = cid / page_size_;
		entry = cid % page_size_;
	}

	template<class T>
	inline void ComponentStore<T>::addPage()
	{
		ComponentContainer* page = (ComponentContainer*)malloc(sizeof(ComponentContainer) * page_size_);
		store_.push_back(page);
	}

	template<class T>
	inline void ComponentStore<T>::addPages(ComponentID_t pages)
	{
		for (ComponentID_t i = 0; i < pages; ++i)
		{
			addPage();
		}
	}
}