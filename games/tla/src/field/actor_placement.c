/// @file
/// The idle script, and the placement table the field starts with.

#include "field/scene_tables.h"
#include "field/actor_script.h"
#include "types.h"

/// The script of an actor that does nothing.
const s32 gActor_IdleScript[] = {
    SC_END,
};

/// The two rows the field copies into its state as it starts, before
/// Actor_ResetForLeader places the leader: one idle actor at (300, 0, 300) and
/// the end of the table.
const struct ActorPlacement gField_DefaultPlacements[] = {
    ACTOR_PLACEMENT(0, CONDITION_ALWAYS, (s32)gActor_IdleScript, 300, 0, 300,
                    23472, 0, 0),
    {SCENE_TABLE_END, 0, 0, 0, 0, 0, 0, 0, 0},
};
