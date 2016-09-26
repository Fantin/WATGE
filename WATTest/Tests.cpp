#pragma once
#include "stdafx.h"

#include "Tests.h"

#include "ComponentManager.h"
#include "EntityManager.h"

#include <cstdio>
#include <cassert>
#include <array>

#define PRINT_ERROR(condition) if(!(condition)) {printf("FAILED: %s\n", __FUNCTION__);} 

using namespace WATGE;

namespace Test
{

	struct TestObj
	{
		int info;
	};

	void ComponentStoreAddGet1()
	{
		ComponentStore<TestObj> cs;
		TestObj* objAdd;
		ComponentID_t cid;
		cs.addComponent(0, objAdd, cid);
		objAdd->info = 123;
		TestObj* objGet;
		cs.getComponent(cid, objGet);
		PRINT_ERROR(objGet->info == 123);
	}

	void ComponentStoreAddGet2()
	{
		ComponentStore<TestObj> cs;
		const size_t test_size = 100;
		std::array<ComponentID_t, test_size> cids;

		for (size_t i = 0; i < test_size; ++i)
		{
			TestObj* p;
			ComponentID_t cid;
			cs.addComponent(i, p, cid);
			p->info = i;
			cids[i] = cid;
		}

		bool noError = true;
		for (uint8_t i = 0; i < test_size; ++i)
		{
			TestObj* p;
			cs.getComponent(cids[i], p);
			noError &= p->info == i;
		}
		PRINT_ERROR(noError);
	}

	void ComponentLookupAddGet1()
	{
		ComponentLookup cl;
		cl.addEntry(0, 123);

		ComponentID_t cid;
		cl.getEntry(0, cid);

		PRINT_ERROR(cid == 123);
	}

	void ComponentLookupAddGet2()
	{
		ComponentLookup cl;
		const size_t test_size = 100;

		for (size_t i = 0; i < test_size; ++i)
		{
			cl.addEntry(i, i);
		}

		bool noError = true;
		for (size_t i = 0; i < test_size; ++i)
		{
			ComponentID_t cid;
			cl.getEntry(i, cid);
			noError &= cid == i;
		}
		PRINT_ERROR(noError);
	}

	void ComponentLookupAddChangeGet1()
	{
		ComponentLookup cl;
		cl.addEntry(0, 123);

		cl.changeEntry(0, 123 * 2);

		ComponentID_t cid;
		cl.getEntry(0, cid);

		PRINT_ERROR(cid == 123 * 2);
	}

	void ComponentLookupAddChangeGet2()
	{
		ComponentLookup cl;
		const size_t test_size = 100;

		for (size_t i = 0; i < test_size; ++i)
		{
			cl.addEntry(i, i);
		}

		for (size_t i = 0; i < test_size; ++i)
		{
			cl.changeEntry(i, i * 2);
		}

		bool noError = true;
		for (size_t i = 0; i < test_size; ++i)
		{
			ComponentID_t cid;
			cl.getEntry(i, cid);
			noError &= cid == i * 2;
		}
		PRINT_ERROR(noError);
	}

	void ComponentManagerAddGet1()
	{
		ComponentManager<TestObj> cm;
		TestObj* objAdd;
		cm.addComponent(0, objAdd);
		objAdd->info = 123;
		TestObj* objGet;
		cm.getComponent(0, objGet);
		PRINT_ERROR(objGet->info == 123);
	}

	void ComponentManagerAddGet2()
	{
		ComponentManager<TestObj> cm;

		const size_t test_size = 100;
		for (size_t i = 0; i < test_size; ++i)
		{
			TestObj* objAdd;
			cm.addComponent(i, objAdd);
			objAdd->info = i;
		}
		bool noError = true;
		for (size_t i = 0; i < test_size; ++i)
		{
			TestObj* objGet;
			cm.getComponent(i, objGet);
			noError &= objGet->info == i;
		}
		PRINT_ERROR(noError);
	}

	void ComponentManagerAddRemoveAddGet1()
	{
		ComponentManager<TestObj> cm;
		TestObj* objAdd;
		cm.addComponent(0, objAdd);
		objAdd->info = 123;
		cm.removeComponent(0);
		cm.addComponent(0, objAdd);
		objAdd->info = 123 * 2;
		TestObj* objGet;
		cm.getComponent(0, objGet);
		PRINT_ERROR(objGet->info == 123 * 2);
	}

	void ComponentManagerAddRemoveAddGet2()
	{
		ComponentManager<TestObj> cm;

		const size_t test_size = 100;
		for (size_t i = 0; i < test_size; ++i)
		{
			TestObj* objAdd;
			cm.addComponent(i, objAdd);
			objAdd->info = i;
		}
		for (size_t i = 0; i < test_size; ++i)
		{
			cm.removeComponent(i);
		}
		for (size_t i = 0; i < test_size; ++i)
		{
			TestObj* objAdd;
			cm.addComponent(i, objAdd);
			objAdd->info = i * 2;
		}
		bool noError = true;
		for (size_t i = 0; i < test_size; ++i)
		{
			TestObj* objGet;
			cm.getComponent(i, objGet);
			noError &= objGet->info == i * 2;
		}
		PRINT_ERROR(noError);
	}

	void ComponentManagerRandomUsage()
	{
		ComponentManager<TestObj> cm;

		const size_t test_size = 100;
		const size_t iterations = 1000;

		// Initialize array
		std::array<int, test_size> values;
		for (int i = 0; i < test_size; ++i)
		{
			values[i] = -1;
		}
		// Initialize components
		for (int i = 0; i < test_size; i++)
		{
			TestObj* objAdd;
			int value = rand();
			if (value < 0)
			{
				value = -value;
			}
			cm.addComponent(i, objAdd);
			objAdd->info = value;
			values[i] = value;
		}
		// Randomly
		for (int i = 0; i < iterations; i++)
		{
			// Get a positive integer
			int value = rand();
			if (value < 0)
			{
				value = -value;
			}
			// Select an entity
			int obj = value % test_size;
			// Replace if removed
			if (values[obj] == -1)
			{
				TestObj* addObj;
				cm.addComponent(obj, addObj);
				addObj->info = value;
				values[obj] = value;
			}
			else
			{
				// Maybe change if present
				if (value % 2 == 0)
				{
					TestObj* getObj;
					cm.getComponent(obj, getObj);
					getObj->info = value;
					values[obj] = value;
				}
				// Maybe remove if present
				else
				{
					cm.removeComponent(obj);
					values[obj] = -1;
				}
			}
		}

		// Check values
		bool noError = true;
		for (int i = 0; i < test_size; i++)
		{
			TestObj* getObj;
			cm.getComponent(i, getObj);
			if (getObj == nullptr)
			{
				noError &= values[i] == -1;
			}
			else
			{
				noError &= values[i] == getObj->info;
			}
		}
		PRINT_ERROR(noError);
	}

	void EntityManagerMake()
	{
		EntityManager* em;
		EntityManager::makeManager<>(em);
		EntityHandle eh1;
		em->makeEntity(eh1);
		bool noError1 = (eh1.eid_ == 0) && (eh1.guid_ == 0);
		EntityHandle eh2;
		em->makeEntity(eh2);
		bool noError2 = (eh2.eid_ == 1) && (eh2.guid_ == 1);
		bool noError = noError1 && noError2;
		PRINT_ERROR(noError);
		delete em;
	}

	void EntityManagerAddRemove()
	{
		EntityManager* em;
		EntityManager::makeManager<>(em);
		EntityHandle eh1;
		em->makeEntity(eh1);
		em->removeEntity(eh1);
		EntityHandle eh2;
		em->makeEntity(eh2);
		PRINT_ERROR(eh2.guid_ = eh2.guid_ + 1);
		delete em;
	}

	void EntityManagerAddComponent1()
	{
		EntityManager* em;
		EntityManager::makeManager<TestObj>(em);
		EntityHandle eh;
		em->makeEntity(eh);
		TestObj* obj;
		em->addComponent(eh, obj);
		obj->info = 123;
		TestObj* getObj;
		em->getComponent(eh, getObj);
		PRINT_ERROR(getObj->info == 123);
		delete em;
	}
}