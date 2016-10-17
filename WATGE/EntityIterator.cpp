#ifdef _WIN32
#include "stdafx.h"
#endif

#include "EntityIterator.h"

namespace WATGE
{
	template <class... Ts>
	void EntityIterator<Ts...>::operator++()
	{
		if (*this == end())
			return;
	}

	template <class... Ts>
	bool EntityIterator<Ts...>::operator==(const EntityIterator& other)
	{
		return guid_ == other.guid_;
	}

	template <class... Ts>
	bool EntityIterator<Ts...>::operator!=(const EntityIterator& other)
	{
		return guid_ != other.guid_;
	}

	template <class... Ts>
	EntityHandle EntityIterator<Ts...>::operator*()
	{
		return EntityHandle(eid_, guid_, );
	}

	template <class... Ts>
	EntityIterator<Ts...> EntityIterator<Ts...>::begin()
	{
		return EntityIterator(0, 0, 0);
	}

	template <class... Ts>
	EntityIterator<Ts...> EntityIterator<Ts...>::end()
	{
		static EntityIterator end(0, -1, 0);
		return end;
	}

	template <class... Ts>
	EntityIterator<Ts...>::EntityIterator(EntityID_t eid, EntityGUID_t guid, size_t manager_id) :
		eid_(eid), guid_(guid_), manager_id_(manager_id)
	{

	}
}
