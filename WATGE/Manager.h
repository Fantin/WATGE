#pragma once

#include <cstdint>
#include <vector>
#include <array>

namespace WATGE
{
	template <class Data, class Index = size_t, Index PageSize = 50>
	class Manager
	{
		static_assert(std::is_class<Data>::value, "Components must be classes.");
		static_assert(!std::is_copy_constructible<Data>::value, "Components must not be copyable.");
		static_assert(std::is_unsigned<Index>::value, "Index must be an unsigned integer.");

	public:
		void addObject(Data*& object, Index& index);
		void getObject(Index index, Data*& object);
		void removeObject(Index index);


		Manager();
		~Manager();

	private:
		struct Container
		{
			bool valid_;
			Data obj_;

			Container(const Container&) = delete;
		};

		std::vector<Index> free_;
		void addFree(Index index);
		Index removeFree();

		std::vector<Container*> objects_;
		void addPage();
		void addPages(Index pages);

		void getPageEntry(Index index, Index& page, Index& entry);
		Container* getContainer(Index index);
	};

	template<class Data, class Index, Index PageSize>
	inline void Manager<Data, Index, PageSize>::addObject(Data*& object, Index& index)
	{
		index = removeFree();
		Container* c = getContainer(index);
		c->valid_ = true;
	}

	template<class Data, class Index, Index PageSize>
	inline void Manager<Data, Index, PageSize>::getObject(Index index, Data*& object)
	{
		Container* p = getContainer(index);
		if (p == nullptr || !p->valid_)
		{
			object = nullptr;
			return;
		}
		object = &(p->obj_);
	}

	template<class Data, class Index, Index PageSize>
	inline void Manager<Data, Index, PageSize>::removeObject(Index index)
	{
		Container* p = getContainer(index);
		p->valid_ = false;
		free_.push_back(index);
	}

	template<class Data, class Index, Index PageSize>
	inline Manager<Data, Index, PageSize>::Manager()
	{
	}

	template<class Data, class Index, Index PageSize>
	inline Manager<Data, Index, PageSize>::~Manager()
	{
		for (auto it = objects_.begin(); it != objects_.end(); ++it)
		{
			free(*it);
		}
	}


	template<class Data, class Index, Index PageSize>
	inline void Manager<Data, Index, PageSize>::addFree(Index index)
	{
		free_.push_back(index);
	}

	template<class Data, class Index, Index PageSize>
	inline Index Manager<Data, Index, PageSize>::removeFree()
	{
		if (free_.empty())
		{
			addPage();
		}
		Index i = free_.back();
		free_.pop_back();
		return i;
	}

	template<class Data, class Index, Index PageSize>
	inline void Manager<Data, Index, PageSize>::addPage()
	{
		Index old_size = objects_.size() * PageSize;
		Index new_size = old_size + PageSize;

		Container* arr = (Container*)malloc(sizeof(Container) * PageSize);
		for (Index i = 0; i < PageSize; ++i)
		{
			arr[i].valid_ = false;
		}
		objects_.push_back(arr);

		for (Index i = old_size; i < new_size; ++i)
		{
			addFree(i);
		}
	}

	template<class Data, class Index, Index PageSize>
	inline void Manager<Data, Index, PageSize>::addPages(Index pages)
	{
		for (Index i = 0; i < pages; ++i)
		{
			addPage();
		}
	}

	template<class Data, class Index, Index PageSize>
	inline void Manager<Data, Index, PageSize>::getPageEntry(Index index, Index& page, Index& entry)
	{
		page = index / PageSize;
		entry = index % PageSize;
	}

	template<class Data, class Index, Index PageSize>
	inline typename Manager<Data, Index, PageSize>::Container* Manager<Data, Index, PageSize>::getContainer(Index index)
	{
		if (index > objects_.size())
		{
			return nullptr;
		}
		Index page, entry;
		getPageEntry(index, page, entry);
		return objects_[page] + entry;
	}
}