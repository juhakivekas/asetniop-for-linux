/// \file The responsibility of this object is to answer the question
///       "What does that chord mean?"
/* Looking up meanings of chords is something that will change over time.
 * This is why it important that it is well abstracted away from sensor
 * processing. Ideally this is a single function interface so autocorrect
 * and prediction will belong elsewhere.
 */ 

/// A chord is a bitmask of touch signals.
typedef int chord;

/// Initialize the lookup structure.
/* The initialisation is done from a file to discourage hardcoding. The
 * format will probably change drastically over time.
 */
void init_lookup(char* filename);

/// Get the keycode corresponding to a certain chord.
/* Whatever the chord type is, it will always correspond to a character,
 * key, or set of characters. This is also an interface that will change
 * over time.
 */
int get_keycode(chord);
