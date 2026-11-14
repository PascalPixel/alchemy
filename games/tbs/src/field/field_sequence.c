/// @file
/// The field sequence's object: random encounters, a field scene's actors, the
/// leader's walking and the requests they raise, FieldSequence_Run, the
/// scene's triggers, using items and Psynergy on the field, the map's treasure
/// spots, and the screen transitions and palette fade that the field and
/// battles share. field_sequence_internal.h holds the includes and the few
/// macros and types more than one part uses; the code is in the files under
/// field_sequence/, included here in link order, each opening with what it
/// holds (head -2 field_sequence/*.c). One family to a part. The main routine,
/// the first code of the same bank, is its own object linked just before this
/// one
/// (../system/main_routine.c).
///
/// README.md in this folder gives the words, prefixes and spellings the parts
/// share.

#include "field_sequence_internal.h"

// The parts, in ROM order (src/README.md, "Objects and parts").
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
