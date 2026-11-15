/// @file
/// The idle script and the default placements of the leader.

#include "field/scene_tables.h"
#include "field/actor_script.h"
#include "types.h"

/// The script of an object that does nothing.
const s32 gActor_IdleScript[] = {
    SC_END,
};

/// Two rows the field copies into its work as it starts, one idle object at
/// (300, 0, 300) and the end mark.
// The Broken Seal's gField_DefaultPlacements.
const struct ActorPlacement gField_DefaultPlacements[] = {
    {0, CONDITION_ALWAYS, (s32)gActor_IdleScript, PIXELS(300), 0, PIXELS(300),
     0x5bb0, 0, 0},
    {SCENE_TABLE_END, 0, 0, 0, 0, 0, 0, 0, 0},
};
