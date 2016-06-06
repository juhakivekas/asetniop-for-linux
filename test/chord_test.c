#include <stdio.h>
#include <string.h>
#include "CUnit/Basic.h"
#include "../src/chord.h"

struct chord state;

int init_suite1(void){
	chord_reset(&state);		
	return 0;
}

int clean_suite1(void){
	return 0;
}

void test_chord_reset(void){
	int i;	
	for(i=0; i<NUM_TOUCHES; i++){
		CU_ASSERT(0 == state.touch_state[i]);
		CU_ASSERT(0 == state.touch_accumulator[i]);
	}
}

void test_chord_touch_start(){
	chord_touch_start(&state, TOUCH_T);
	CU_ASSERT(1 == state.touch_state[TOUCH_T]);
	CU_ASSERT(1 == state.touch_accumulator[TOUCH_T]);
}

void test_chord_touch_end(){
	chord_touch_start(&state, TOUCH_T);
	chord_touch_end(&state, TOUCH_T);
	CU_ASSERT(0 == state.touch_state[TOUCH_T]);
	CU_ASSERT(1 == state.touch_accumulator[TOUCH_T]);
}

void test_chord_state_is_empty(){
	CU_ASSERT(1 == chord_state_is_empty(&state));

	chord_touch_start(&state, TOUCH_T);
	CU_ASSERT(0 == chord_state_is_empty(&state));

	chord_touch_end(&state, TOUCH_T);
	CU_ASSERT(1 == chord_state_is_empty(&state));
}

void test_chord_state_bitmap(){
	chord_touch_start(&state, TOUCH_A);
	CU_ASSERT( (1<<TOUCH_A) == chord_state_bitmap(&state));

	chord_touch_start(&state, TOUCH_T);
	CU_ASSERT( ((1<<TOUCH_A) | (1<<TOUCH_T)) == chord_state_bitmap(&state))

	chord_touch_end(&state, TOUCH_T);	
	CU_ASSERT( (1<<TOUCH_A) == chord_state_bitmap(&state));
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
		(NULL == CU_add_test(pSuite, "test chord_reset"         , test_chord_reset))         ||
		(NULL == CU_add_test(pSuite, "test chord_touch_start"   , test_chord_touch_start))   ||
		(NULL == CU_add_test(pSuite, "test chord_touch_end"     , test_chord_touch_end))     ||
		(NULL == CU_add_test(pSuite, "test chord_state_is_empty", test_chord_state_is_empty))||
		(NULL == CU_add_test(pSuite, "test chord_state_bitmap"  , test_chord_state_bitmap))
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
