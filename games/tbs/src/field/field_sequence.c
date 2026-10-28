/// @file
/// The field sequence's object: random encounters, a field scene's actors, the
/// leader's walking and the requests they raise, FieldSequence_Run, the
/// scene's triggers, using items and Psynergy on the field, the map's treasure
/// spots, and the screen transitions and palette fade that the field and
/// battles share. field_sequence_internal.h holds the includes and the few
/// macros and types more than one part uses; the code is in the files under
/// field_sequence/, included here in ROM order, each opening with what it holds
/// (head -2 field_sequence/*.c). The ROM shows one object; the parts are ours,
/// cut where one family ends and the next begins. The main routine, the first
/// code of the same bank, is its own object linked just before this one
/// (../system/main_routine.c).
///
/// README.md in this folder gives the words, prefixes and spellings the parts
/// share.

#include "field_sequence_internal.h"

// The parts, in ROM order (src/README.md, "Objects and parts").
#include "field_sequence/encounter.c"
#include "field_sequence/actors.c"
#include "field_sequence/walking.c"
#include "field_sequence/run.c"
#include "field_sequence/palette_editor.c"
#include "field_sequence/triggers.c"
#include "field_sequence/targets.c"
#include "field_sequence/items.c"
#include "field_sequence/psynergy.c"
#include "field_sequence/treasure_spots.c"
#include "field_sequence/item_display.c"
#include "field_sequence/screen_transition.c"
#include "field_sequence/palette_fade.c"
#include "field_sequence/ground_mask.c"
#include "field_sequence/speakers.c"
