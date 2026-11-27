/* @file */
/* The field sequence's object: random encounters, a field scene's actors, the
 */
/* leader's walking and the requests they raise, fdSequenceRun, the */
/* scene's triggers, using items and Psynergy on the field, the map's treasure
 */
/* spots, and the screen transitions and palette fade that the field and */
/* battles share. field_sequence_internal.h holds the includes and the few */
/* macros and types more than one part uses; the code is in the */
/* field_sequence_*.c parts beside it, included here in link order, one group */
/* to a part, each opening with what it holds. The main routine, the first code
 */
/* of the same module, is its own object linked just before this one */
/* (../system/main_routine.c). */
/* */
/* README.md in this folder gives the words, prefixes and spellings the parts */
/* share. */

#include "field_sequence_internal.h"

/* The parts, in ROM order (src/README.md, "Objects and parts"). */
#include "field_sequence_encounter.c"
#include "field_sequence_actors.c"
#include "field_sequence_walking.c"
#include "field_sequence_run.c"
#include "field_sequence_palette_editor.c"
#include "field_sequence_triggers.c"
#include "field_sequence_targets.c"
#include "field_sequence_items.c"
#include "field_sequence_psynergy.c"
#include "field_sequence_treasure_spots.c"
#include "field_sequence_item_display.c"
#include "field_sequence_screen_transition.c"
#include "field_sequence_palette_fade.c"
#include "field_sequence_ground_mask.c"
#include "field_sequence_speakers.c"
