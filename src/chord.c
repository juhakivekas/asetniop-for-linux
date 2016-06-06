#include "chord.h"

void chord_reset(struct chord* state){
	int i;
	for(i=0; i<NUM_TOUCHES; i++){
		state->touch_state[i]       = 0;
		state->touch_accumulator[i] = 0;
	}
}

void chord_touch_start(struct chord* state, enum touch t){
	state->touch_state[t] = 1;
	state->touch_accumulator[t] = 1;
}

void chord_touch_end(struct chord* state, enum touch t){
	state->touch_state[t] = 0;
}

int chord_state_is_empty(struct chord* state){
	int i;
	for(i=0; i<NUM_TOUCHES; i++){
		if(0 != state->touch_state[i]){
			return 0;
		}
	}
	return 1;
}

static int array_as_int(int* array){
	int i;
	int res = 0;
	// TOUCH_A will corespond to the lowest bit!
	for(i=NUM_TOUCHES-1; i>= 0; i--){
		res <<= 1;
		if(0 != array[i]) res++;
	}
	return res;
}

int chord_state_bitmap(struct chord* state){
	return array_as_int(state->touch_state);
}

int chord_accumulator_bitmap(struct chord* state){
	return array_as_int(state->touch_accumulator);
}

