#pragma once

#include "WATCore.h"

#include <cstdint>
#include <cstdlib>
#include <atomic>
#include <type_traits>
#include <vector>

namespace WATGE
{
	class IComponentManager
	{
	public:
		virtual ~IComponentManager();
	protected:
		static ComponentClassID_t next_id();
	};

	template <class T>
	class ComponentManager : public IComponentManager
	{
	public:
		ComponentManager();
		ComponentManager(const ComponentManager&) = delete;

		static const ComponentClassID_t getID();

		bool hasComponent(EntityID_t eid);
		T* getComponent(EntityID_t eid);
		bool addComponent(EntityID_t eid);
		template <class... Args>
		T* addCreateComponent(EntityID_t eid, Args... args);
		bool removeComponent(EntityID_t eid);

	private:
		bool getBit(EntityID_t eid);
		bool setBit(EntityID_t eid);
		bool clearBit(EntityID_t eid);
		static const std::size_t page_size = 64;

		std::vector<uint64_t> component_usage_;
		std::vector<std::size_t> entry_count_;
		std::vector<T*> components_;
	};

	template<class T>
	ComponentManager<T>::ComponentManager()
	{
	}

	template<class T>
	const ComponentClassID_t ComponentManager<T>::getID()
	{
		static const ComponentClassID_t id = next_id();
		return id;
	}

	template <class T>
	bool ComponentManager<T>::hasComponent(EntityID_t eid)
	{
		return getBit(eid);
	}

	template <class T>
	T* ComponentManager<T>::getComponent(EntityID_t eid)
	{
		if (!getBit(eid))
		{
			return nullptr;
		}
		EntityID_t page, entry;
		page = eid / page_size;
		entry = eid % page_size;
		return components_[page] + entry;
	}

	template <class T>
	bool ComponentManager<T>::addComponent(EntityID_t eid)
	{
		if (getBit(eid))
		{
			return false;
		}
		setBit(eid);
		EntityID_t page = eid / page_size;
		if (entry_count_[page] == 0)
		{
			components_[page] = static_cast<T*>(malloc(sizeof(T) * page_size));
		}
		entry_count_[page]++;
		return true;
	}

	template <class T>
	template <class... Args>
	T* ComponentManager<T>::addCreateComponent(EntityID_t eid, Args... args)
	{
		if (getBit(eid))
		{
			return nullptr;
		}
		setBit(eid);
		EntityID_t page = eid / page_size;
		EntityID_t entry = eid % page_size;
		if (entry_count_[page] == 0)
		{
			components_[page] = static_cast<T*>(malloc(sizeof(T) * page_size));
		}
		entry_count_[page]++;
		return new(components_[page] + entry) T(args...);
	}

	template<class T>
	bool ComponentManager<T>::removeComponent(EntityID_t eid)
	{
		if (!getBit(eid))
		{
			return false;
		}
		clearBit(eid);
		EntityID_t page = eid / page_size;
		entry_count_[page]--;
		if (entry_count_[page] == 0)
		{
			free(components_[page]);
			components_[page] = nullptr;
		}
		return true;
	}

	template <class T>
	bool ComponentManager<T>::getBit(EntityID_t eid)
	{
		std::size_t chunk = eid / 64;
		std::size_t bit = eid % 64;
		if (chunk >= component_usage_.size())
		{
			return false;
		}
		return (component_usage_[chunk] >> bit) & 1ULL;
	}

	template <class T>
	bool ComponentManager<T>::setBit(EntityID_t eid)
	{
		std::size_t chunk = eid / 64;
		std::size_t bit = eid % 64;
		if (chunk >= component_usage_.size())
		{
			component_usage_.resize(chunk + 1, 0);
			entry_count_.resize(chunk + 1, 0);
			components_.resize(chunk + 1, nullptr);
		}
		bool old = (component_usage_[chunk] >> bit) & 1ULL;
		component_usage_[chunk] |= (1ULL << bit);
		return old;
	}

	template <class T>
	bool ComponentManager<T>::clearBit(EntityID_t eid)
	{
		std::size_t chunk = eid / 64;
		std::size_t bit = eid % 64;
		if (chunk >= component_usage_.size())
		{
			return false;
		}
		bool old = (component_usage_[chunk] >> bit) & 1ULL;
		component_usage_[chunk] &= ~(1ULL << bit);
		return old;
	}


}
