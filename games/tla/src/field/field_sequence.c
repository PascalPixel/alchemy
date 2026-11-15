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
/// object linked just before this one (../system/main_routine.c). After the
/// treasure spots follow the screen transitions and, as The Broken Seal's
/// event_commands.c begins, the event commands.

#include "field_sequence_internal.h"

// The parts, in ROM order (The Broken Seal's src/README.md, "Objects and
// parts").
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

#undef PartyInventory_GiveItem

#include "field_sequence_screen_transition.c"

/* VIEW(event_commands): routine GameFlag_GetByte as u8 GameFlag_GetByte(s32
 * flag); field/event.h has s32 GameFlag_GetByte(s32 flag); tried as the
 * definition's u8 GameFlag_GetByte(s32 flag), which the object already
 * declares, it does not compile in tla-ja:
 * games/tla/include/game/game_flags.h:13: conflicting types for
 * `GameFlag_GetByte'. */
u8 GameFlag_GetByte__event_commands(s32 flag) __asm__("GameFlag_GetByte");
#define GameFlag_GetByte GameFlag_GetByte__event_commands
// The event commands read the game flags through the view above.
#include "game/game_flags.h"

// The object's functions its parts call before their definitions.
void Actor_SetHorizontalPositionWithTerrain(u32 objectId, s32 x, s32 z);
void Actor_MoveToObject(u32 objectId, u32 otherId);

// The parts, in ROM order (The Broken Seal's src/README.md, "Objects and
// parts").
#include "field_sequence_event.c"
#include "field_sequence_requests.c"
#include "field_sequence_actor_commands.c"

#undef Field_RequestDjinnBattle
#undef EventWork1cc
#undef Actor_SetHorizontalPositionAndAngleWithTerrain
#undef GameFlag_GetByte
#undef Actor_SetHorizontalPositionWithTerrain
