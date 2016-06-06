#include "chord.h"

void chord_touch_reset(struct chord* state){
	int i;
	for(i=0; i<NUM_TOUCHES; i++){
		state->touch_state[i]       = 0;
		state->touch_accumulator[i] = 0;
	}
}
