#pragma once
#include "stdafx.h"

#include "Tests.h"

#include "ComponentManager.h"
#include "EntityManager.h"
#include "EntityHandle.h"

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
		ComponentStore cs(sizeof(TestObj));
		ComponentID_t cid = cs.addComponent(0);
		TestObj* objAdd = (TestObj*) cs.getComponent(cid);
		objAdd->info = 123;

		TestObj* objGet = (TestObj*) cs.getComponent(cid);
		PRINT_ERROR(objGet->info == 123);
	}

	void ComponentStoreAddGet2()
	{
		ComponentStore cs(sizeof(TestObj));
		const size_t test_size = 100;
		std::array<ComponentID_t, test_size> cids;

		for (size_t i = 0; i < test_size; ++i)
		{
			ComponentID_t cid = cs.addComponent(i);
			TestObj* p = (TestObj*) cs.getComponent(cid);
			p->info = i;
			cids[i] = cid;
		}

		bool noError = true;
		for (uint8_t i = 0; i < test_size; ++i)
		{
			TestObj* p = (TestObj*) cs.getComponent(cids[i]);
			noError &= p->info == i;
		}
		PRINT_ERROR(noError);
	}

	void ComponentLookupAddGet1()
	{
		ComponentLookup cl;
		cl.editEntry(0, 123);

		ComponentID_t cid = cl.getEntry(0);

		PRINT_ERROR(cid == 123);
	}

	void ComponentLookupAddGet2()
	{
		ComponentLookup cl;
		const size_t test_size = 100;

		for (size_t i = 0; i < test_size; ++i)
		{
			cl.editEntry(i, i);
		}

		bool noError = true;
		for (size_t i = 0; i < test_size; ++i)
		{
			ComponentID_t cid = cl.getEntry(i);
			noError &= cid == i;
		}
		PRINT_ERROR(noError);
	}

	void ComponentLookupAddChangeGet1()
	{
		ComponentLookup cl;
		cl.editEntry(0, 123);

		cl.editEntry(0, 123 * 2);

		ComponentID_t cid = cl.getEntry(0);
		PRINT_ERROR(cid == 123 * 2);
	}

	void ComponentLookupAddChangeGet2()
	{
		ComponentLookup cl;
		const size_t test_size = 100;

		for (size_t i = 0; i < test_size; ++i)
		{
			cl.editEntry(i, i);
		}

		for (size_t i = 0; i < test_size; ++i)
		{
			cl.editEntry(i, i * 2);
		}

		bool noError = true;
		for (size_t i = 0; i < test_size; ++i)
		{
			ComponentID_t cid = cl.getEntry(i);
			noError &= cid == i * 2;
		}
		PRINT_ERROR(noError);
	}

	void ComponentManagerAddGet1()
	{
		ComponentManager<TestObj> cm;
		cm.addComponent(0);
		TestObj* objAdd = cm.getComponent(0);
		objAdd->info = 123;
		TestObj* objGet = cm.getComponent(0);
		PRINT_ERROR(objGet->info == 123);
	}

	void ComponentManagerAddGet2()
	{
		ComponentManager<TestObj> cm;

		const size_t test_size = 100;
		for (size_t i = 0; i < test_size; ++i)
		{
			cm.addComponent(i);
			TestObj* objAdd = cm.getComponent(i);
			objAdd->info = i;
		}
		bool noError = true;
		for (size_t i = 0; i < test_size; ++i)
		{
			TestObj* objGet = cm.getComponent(i);
			noError &= objGet->info == i;
		}
		PRINT_ERROR(noError);
	}

	void ComponentManagerAddRemoveAddGet1()
	{
		ComponentManager<TestObj> cm;
		cm.addComponent(0);
		TestObj* objAdd;
		objAdd = cm.getComponent(0);
		objAdd->info = 123;
		cm.removeComponent(0);
		cm.addComponent(0);
		objAdd = cm.getComponent(0);
		objAdd->info = 123 * 2;
		TestObj* objGet = cm.getComponent(0);
		PRINT_ERROR(objGet->info == 123 * 2);
	}

	void ComponentManagerAddRemoveAddGet2()
	{
		ComponentManager<TestObj> cm;

		const size_t test_size = 100;
		for (size_t i = 0; i < test_size; ++i)
		{
			cm.addComponent(i);
			TestObj* objAdd = cm.getComponent(i);
			objAdd->info = i;
		}
		for (size_t i = 0; i < test_size; ++i)
		{
			cm.removeComponent(i);
		}
		for (size_t i = 0; i < test_size; ++i)
		{
			cm.addComponent(i);
			TestObj* objAdd = cm.getComponent(i);
			objAdd->info = i * 2;
		}
		bool noError = true;
		for (size_t i = 0; i < test_size; ++i)
		{
			TestObj* objGet = cm.getComponent(i);
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
			cm.addComponent(i);
			objAdd = cm.getComponent(i);
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
				cm.addComponent(obj);
				TestObj* addObj = cm.getComponent(obj);
				addObj->info = value;
				values[obj] = value;
			}
			else
			{
				// Maybe change if present
				if (value % 2 == 0)
				{
					TestObj* getObj = cm.getComponent(obj);
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
			TestObj* getObj = cm.getComponent(i);
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
		eWATError e;
		EntityManager* em = EntityManager::makeManager<>(e);
		EntityHandle eh1 = EntityHandle::makeEntity(em, e);
		bool noError1 = (eh1.eid_ == 0) && (eh1.guid_ == 0);
		EntityHandle eh2 = EntityHandle::makeEntity(em, e);
		EntityHandle eh3 = EntityHandle::makeEntity(em, e);
		bool noError2 = (eh2.eid_ == 1) && (eh2.guid_ == 1);
		bool noError = noError1 && noError2;
		PRINT_ERROR(noError);
		delete em;
	}

	void EntityManagerAddRemove()
	{
		eWATError e;
		EntityManager* em = EntityManager::makeManager<>(e);
		EntityHandle eh1 = EntityHandle::makeEntity(em, e);
		eh1.deleteEntity(e);
		EntityHandle eh2 = EntityHandle::makeEntity(em, e);
		PRINT_ERROR(eh2.guid_ == eh1.guid_ + 1);
		delete em;
	}

	void EntityManagerAddComponent1()
	{
		eWATError e;
		EntityManager* em= EntityManager::makeManager<TestObj>(e);
		EntityHandle eh = EntityHandle::makeEntity(em, e);
		eh.addComponent<TestObj>(e);
		TestObj* obj = eh.getComponent<TestObj>(e);
		obj->info = 123;
		TestObj* getObj = eh.getComponent<TestObj>(e);
		PRINT_ERROR(getObj->info == 123);
		delete em;
	}
}