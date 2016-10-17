#include <cstdio>
#include <vector>

#include <cstdlib>
#include <dlfcn.h>

#include "Tests.h"

int main()
{
	printf("Start.\n");

	void *handle;
	int (*doStuff)();
	char *error;
	handle = dlopen ("./libwataddon.so", RTLD_LAZY);
	if (!handle) {
	   fprintf (stderr, "%s\n", dlerror());
	   return 1;
	}
	dlerror();    /* Clear any existing error */
	doStuff = (int(*)()) dlsym(handle, "doStuff");
	if ((error = dlerror()) != NULL)  {
	   fprintf (stderr, "%s\n", error);
	   return 1;
	}
	printf ("%d\n", doStuff());
	dlclose(handle);

	Test::ComponentManagerAddGet1();
	Test::ComponentManagerAddGet2();
	Test::ComponentManagerAddRemoveAddGet1();
	Test::ComponentManagerAddRemoveAddGet2();
	Test::ComponentManagerRandomUsage();
	Test::EntityManagerMake();
	Test::EntityManagerAddRemove();
	Test::EntityManagerAddComponent1();
	printf("Done.\n");
	return 0;
}

//printf("Hello world!\n");
//HINSTANCE lib = LoadLibrary(TEXT("WATAddon.dll"));
//if (lib == NULL)
//{
//	printf("DLL not found\n");
//	return 1;
//}

//Test* (*fun)() = (Test*(*)()) GetProcAddress(lib, "getTest");
//if (fun == NULL)
//{
//	printf("Function not found\n");
//	FreeLibrary(lib);
//	return 1;
//}

//Test* p = fun();
/*
WATGE::ComponentManager<Test> cmt;

for (int i = 0; i < 10; ++i)
{
Test* t;
WATGE::ComponentID_t cid;
cmt.addComponent(i, t, cid);
t->a_ = i;

printf("Added cid: %d\n", cid);
}

for (int i = 0; i < 10; ++i)
{
Test* t;
if (cmt.getComponent(i, t) == WATGE::eNoError)
printf("cid: %d had data: %d\n", i, t->a_);
else
printf("Error getting cid: %d\n", i);
}

for (int i = 0; i < 10; ++i)
{
WATGE::EntityID_t eid;
if (cmt.removeComponent(0, eid) == WATGE::eNoError)
printf("removed cid: %d. eid: %d was placed in its spot\n", i, eid);
else
printf("Error removing cid: %d\n", i);
}

printf("ID: %d\n", cmt.getID());
printf("ID: %d\n", WATGE::ComponentManager<Test2>::getID());
printf("ID: %d\n", cmt.getID());
printf("ID: %d\n", WATGE::ComponentManager<Test2>::getID());
*/
/*
WATGE::EntityIDManager eidm;

using WATGE::EntityID_t;

EntityID_t arr[10];

for (int i = 0; i < 10; ++i)
{
WATGE::EntityID_t eid;
eidm.takeID(eid);
arr[i] = eid;
printf("Taking: %d\n", arr[i]);
}

for (int i = 0; i < 10; ++i)
{
WATGE::EntityID_t eid = arr[i];
WATGE::EntityGUID_t guid;
eidm.getGUID(eid, guid);
printf("eid: %d, guid: %d\n", arr[i], guid);
}

for (int i = 0; i < 10; ++i)
{
printf("Returning: %d\n", arr[i]);
eidm.returnID(arr[i]);
}


for (int i = 0; i < 10; ++i)
{
WATGE::EntityID_t eid;
eidm.takeID(eid);
arr[i] = eid;
printf("Taking: %d\n", arr[i]);
}

for (int i = 0; i < 10; ++i)
{
WATGE::EntityID_t eid = arr[i];
WATGE::EntityGUID_t guid;
eidm.getGUID(eid, guid);
printf("eid: %d, guid: %d\n", arr[i], guid);
}

for (int i = 0; i < 10; ++i)
{
printf("Returning: %d\n", arr[i]);
eidm.returnID(arr[i]);
}*/
