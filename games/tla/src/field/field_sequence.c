/* @file */
/* The field sequence's object, as The Broken Seal's field_sequence.c begins: */
/* random encounters, a field scene's actors, the leader's walking and the */
/* requests it raises, fdSequenceRun, the debug palette editor, the scene's */
/* triggers, the actor a field Psynergy or item aims at, using items and */
/* Psynergy on the field, and the map's treasure spots. */
/* field_sequence_internal.h holds the includes and the declarations the parts
 */
/* share; the code is in the field_sequence_*.c files beside it, included here
 */
/* in ROM order. The ROM shows one object; the parts are ours, cut where The */
/* Broken Seal's are. The main routine, the first code of the same module, is */
/* its own object linked just before this one (../system/main_routine.c). After
 */
/* the treasure spots follow the screen transitions and, as The Broken Seal's */
/* event_commands.c begins, the event commands. */

#include "field_sequence_internal.h"

/* The parts, in ROM order (The Broken Seal's src/README.md, "Objects and */
/* parts"). */
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
#include "field_sequence_heading.c"

/* The object's functions its parts call before their definitions. */

/* The event commands' parts, as The Broken Seal's event_commands.c begins. */
#include "field_sequence_event.c"
#include "field_sequence_requests.c"
#include "field_sequence_actor_commands.c"

#undef syGameFlagGetByte
#undef fdActorSetHorizontalPosWithTerrain
