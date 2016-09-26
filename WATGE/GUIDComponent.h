#include <cstdint>

namespace WATGE
{
	typedef uint64_t EntityGUID_t;

	struct GUIDComponent
	{
		EntityGUID_t guid_;

		static EntityGUID_t getNextGUID();
	};
}