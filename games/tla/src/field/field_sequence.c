/// @file
/// The field sequence's object, as The Broken Seal's field_sequence.c begins:
/// random encounters, a field scene's actors, the leader's walking and the
/// requests it raises, FieldSequence_Run, the debug palette editor, the scene's
/// triggers, the actor a field Psynergy or item aims at, using items and
/// Psynergy on the field, and the map's treasure spots.
/// field_sequence_internal.h holds the includes and the declarations the parts
/// share; the code is in the files under field_sequence/, included here in ROM
/// order. The ROM shows one object; the parts are ours, cut where The Broken
/// Seal's are. The main routine, the first code of the same bank, is its own
/// object linked just before this one
/// (../system/main_routine.c). The bank's code after the treasure spots is
/// in objects of its own: two of them (field_sequence/emitter.c and
/// field_sequence/rising.c) read a sprite's
/// third attribute in two ways, as tile bit fields and as its high byte, and no
/// one struct gives both with the same code.

#include "field_sequence_internal.h"

// The parts, in ROM order (The Broken Seal's src/README.md, "Objects and
// parts").
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
