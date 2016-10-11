#pragma once

#include "WATCore.h"
#include "EntityHandle.h"

namespace WATGE
{
	template <class... Ts>
	class EntityIterator
	{
	public:
		void operator++();
		bool operator==(const EntityIterator& other);
		bool operator!=(const EntityIterator& other);
		EntityHandle operator*();

		static EntityIterator begin();
		static EntityIterator end();

	private:
		EntityID_t eid_;
		EntityGUID_t guid_;
		size_t manager_id_;

		EntityIterator(EntityID_t eid, EntityGUID_t guid, size_t manager_id);
	};

	template <class... Ts>
	class EntityIteratorHelper;

	template <>
	class EntityIteratorHelper<>
	{
	};

	template <class T, class... Ts>
	class EntityIteratorHelper<T, Ts...>
	{

	};
}