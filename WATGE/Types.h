#pragma once


#include <type_traits>
#include <cstdint>
#include <vector>

namespace WATGE
{
	typedef uint32_t EntityID_t;
	typedef uint32_t ComponentClassID_t;
	typedef int32_t ComponentID_t;

	static_assert(std::is_signed<ComponentID_t>::value, "ComponentID must be a signed integer.");
}