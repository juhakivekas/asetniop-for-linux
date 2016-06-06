/// \file The responsibility of this object is to answer the question
///       "What chord was pressed?"
/* It implements a simple accumulated touches algorithm. The chord will be
 * a sum of touches between the start of the first touch until the end of
 * the last one. This means that the order of touches is irrelevant, as
 * well as their diration.
 */

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
	TOUCH_SPACE
};

struct asetniop{
	/// The state of all the touches
	int touch_state[10];

	/// The accumulated touches since last zeroing.
	int touch_accumulator[10];
}

/// Singal that a touch was started.
/* Tells the state that a signal representing the start of a touch
 * occured. This could be a physical sensor, an event signal, or a touch
 * on a screen.
 */ 
void asetniop_touch_start(struct *asetniop, enum touch);

/// Singal that a touch action ended.
/* Tells the state that a signal representing the end of a touch
 * occured.
 */
void asetniop_touch_end(struct *asetniop, enum touch);

/// Checks if any touch is ongoing.
/* If no touch gestures are ongoing, this will return true. This would
 * be the state we are in when the user is not interacting with the
 * device.
 */
void asetniop_touch_is_empty(struct *asetniop);

/// Sets all touch states and accumulator to not touching.
/* Used to initialize the asetniop struct or to re-initialize the touch
 * accumulator between chords.
 */
void asetniop_touch_reset(struct *asetniop);

/// Get the keycode that the current accumulated chord corresponds to.
/* Gets an EV_KEY code that corresponds to the chord accumulated since
 * the last call to touch_empty.
 */
int asetniop_get_keycode(struct *asetniop);

/// Get a bitmap of the state.
static int state_as_int(struct *asetniop);

/// Get a bitmap of the accumulator.
static int state_as_int(struct *asetniop);


