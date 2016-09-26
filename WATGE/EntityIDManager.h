#include "Error.h"
#include "Entity.h"

#include <vector>

namespace WATGE
{
	class EntityIDManager
	{
	public:
		eWATError takeID(EntityID_t& eid);
		eWATError returnID(EntityID_t eid);

		EntityIDManager(size_t page_size = 50);
		EntityIDManager(const EntityIDManager&) = delete;
		~EntityIDManager();

	private:
		EntityID_t next_id_;
		EntityID_t free_id_count_;
		const size_t page_size_;
		std::vector<EntityID_t*> free_ids_;

		void getPageEntry(size_t index, size_t& page, size_t& entry);
		void addPage();
		void addPages(size_t pages);
	};
}