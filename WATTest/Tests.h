#ifdef TEST_CALL
#undef TEST_FUN
#define TEST_FUN(returnType, functionName) Test::##functionName##()
#else
#undef TEST_FUN
#define TEST_FUN(returnType, functionName) returnType functionName##()
#endif

#ifndef TEST_CALL
namespace Test
{
#endif
	TEST_FUN(void, ComponentManagerAddGet1);
	TEST_FUN(void, ComponentManagerAddGet2);
	TEST_FUN(void, ComponentManagerAddRemoveAddGet1);
	TEST_FUN(void, ComponentManagerAddRemoveAddGet2);
	TEST_FUN(void, ComponentManagerRandomUsage);
	TEST_FUN(void, EntityManagerMake);
	TEST_FUN(void, EntityManagerAddRemove);
	TEST_FUN(void, EntityManagerAddComponent1);
#ifndef TEST_CALL
};
#endif

#ifdef TEST_CALL
#undef TEST_CALL
#endif