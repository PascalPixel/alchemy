/* The field mode's object, as The Broken Seal's field_mode.c begins:
   random encounters, a field scene's actors, the leader's walking and the
   requests it raises, fdModeRun, the debug palette editor, the scene's cues,
   the actor a field Psynergy or item aims at, using items and Psynergy on the
   field, and the map's treasure spots. field_mode_internal.h holds the includes
   and the declarations the parts share; the code is in the field_mode_*.c files
   beside it, included here in ROM order. The ROM shows one object; the parts
   are ours, cut where The Broken Seal's are. The main routine, the first code
   of the same module, is its own object linked just before this one
   (../system/main_routine.c). After the treasure spots follow the screen
   transitions and, as The Broken Seal's script_commands.c begins, the script
   commands. */

#include "field_mode_internal.h"

/* The parts, in ROM order (The Broken Seal's src/README.md, "Objects and
   parts"). */
#include "field_mode_encounter.c"
#include "field_mode_actors.c"
#include "field_mode_walking.c"
#include "field_mode_run.c"
#include "field_mode_palette_editor.c"
#include "field_mode_cues.c"
#include "field_mode_targets.c"
#include "field_mode_items.c"
#include "field_mode_psynergy.c"
#include "field_mode_treasure_spots.c"

#include "field_mode_item_display.c"
#include "field_mode_screen_transition.c"
#include "field_mode_palette_fade.c"
#include "field_mode_ground_mask.c"
#include "field_mode_speakers.c"
#include "field_mode_heading.c"

/* The object's functions its parts call before their definitions. */

/* The script commands' parts, as The Broken Seal's script_commands.c begins. */
#include "field_mode_script.c"
#include "field_mode_requests.c"
#include "field_mode_actor_commands.c"

#undef syGameFlagGetByte
#undef fdActorSetHorizontalPosWithTerrain
