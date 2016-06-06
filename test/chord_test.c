#include <stdio.h>
#include <string.h>
#include "CUnit/Basic.h"
#include "../src/chord.h"

struct chord state;

int init_suite1(void)
{
	chord_touch_reset(&state);		
	return 0;
}

int clean_suite1(void)
{
	return 0;
}

void test_chord_touch_reset(void)
{
	int i;	
	for(i=0; i<NUM_TOUCHES; i++){
		CU_ASSERT(0 == state.touch_state[i]);
		CU_ASSERT(0 == state.touch_accumulator[i]);
	}
}

int main()
{
	CU_pSuite pSuite = NULL;

	/* initialize the CUnit test registry */
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();

	/* add a suite to the registry */
	pSuite = CU_add_suite("Suite 1", init_suite1, clean_suite1);
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* add the tests to the suite */
	if (
		(NULL == CU_add_test(pSuite, "test of chord_touch_reset", test_chord_touch_reset))
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
