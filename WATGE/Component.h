#pragma once

#include "WATCore.h"

#include <cstdint>
#include <type_traits>

namespace WATGE
{
	typedef uint32_t ComponentClassID_t;

	typedef int32_t ComponentID_t;
	static_assert(std::is_signed<ComponentID_t>::value, "ComponentID must be a signed integer.");
}