#include <stdio.h>
#include <string.h>
#include "CUnit/Basic.h"
#include "../src/lookup.h"

int init_suite1(void){
	  return 0;
}

int clean_suite1(void){
	  return 0;
}

void test_lookup_keycode(){
	//this really can't be tested before initialization
}

void test_lookup_init(){
	int err;
	err = lookup_init("dat/test-keymap-minimal.dat");
	CU_ASSERT(0 == err);

	CU_ASSERT(0 == lookup_keycode(0));
	CU_ASSERT('h' == lookup_keycode(0x30));
	CU_ASSERT('e' == lookup_keycode(0x04));
	CU_ASSERT('l' == lookup_keycode(0x60));
	CU_ASSERT('o' == lookup_keycode(0x40));
}



int main(){
	CU_pSuite pSuite = NULL;

	/* initialize the CUnit test registry */
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();

	/* add a suite to the registry */
	pSuite = CU_add_suite("Suite_1", init_suite1, clean_suite1);
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* add the tests to the suite */
	if (
		(NULL == CU_add_test(pSuite, "test lookup_keycode", test_lookup_keycode)) ||
		(NULL == CU_add_test(pSuite, "test lookup_init"   , test_lookup_init))
	){
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* Run all tests using the CUnit Basic interface */
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error();
}
