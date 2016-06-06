/// \file The responsibility of this object is to answer the question
///       "What chord was pressed?"
/* It implements a simple accumulated touches algorithm. The chord will be
 * a sum of touches between the start of the first touch until the end of
 * the last one. This means that the order of touches is irrelevant, as
 * well as their diration.
 */

#ifndef chord_h
#define chord_h

enum touch{
	TOUCH_A = 0,
	TOUCH_S,
	TOUCH_E,
	TOUCH_T,
	TOUCH_N,
	TOUCH_I,
	TOUCH_O,
	TOUCH_P,
	TOUCH_SHIFT,
	TOUCH_SPACE,
	NUM_TOUCHES //this is just to avoid hardcoding 10 everywhere
};

typedef struct chord{
	/// The state of all the touches
	int touch_state[10];

	/// The accumulated touches since last zeroing.
	int touch_accumulator[10];
}chord;

/// Sets all touch states and accumulator to not touching.
/* Used to initialize the chord struct or to re-initialize the touch
 * accumulator between chords.
 */
void chord_reset(struct chord*);

/// Singal that a touch was started.
/* Tells the state that a signal representing the start of a touch
 * occured. This could be a physical sensor, an event signal, or a touch
 * on a screen.
 */ 
void chord_touch_start(struct chord*, enum touch);

/// Singal that a touch action ended.
/* Tells the state that a signal representing the end of a touch
 * occured.
 */
void chord_touch_end(struct chord*, enum touch);

/// Checks if any touch is ongoing.
/* If no touch gestures are ongoing, this will return true. This would
 * be the state we are in when the user is not interacting with the
 * device.
 */
int chord_state_is_empty(struct chord*);

/// Get the bitmap that the currently touched chord corresponds to.
int chord_state_bitmap(struct chord*);

/// Get the keycode that the currently accumulated chord corresponds to.
int chord_accumulator_bitmap(struct chord*);

#endif/*chord_h*/
