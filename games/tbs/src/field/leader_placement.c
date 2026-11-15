/// @file
/// The party leader's actor placement, which Actor_ResetForLeader copies
/// before a scene places its own actors, and the script it gives the leader.

#include "field/actor_script.h"
#include "field/scene_tables.h"
#include "sprite_ids.h"
#include "field_sequence_internal.h"

/// Unused: the camera following the leader, a frame at a time, forever; no
/// code names it.
const s32 gActor_UnusedCameraScript[] = {
    SC_CAMERA,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

/// A lone SC_END: the leader's script, which the player moves.
const s32 gField_LeaderScript[] = {SC_END};

/// The leader's row and the end of the table; Actor_ResetForLeader puts the
/// leader's sprite, spot and heading over the row's.
const struct ActorPlacement gField_DefaultActorPlacements[2] = {
    ACTOR_PLACEMENT(SPRITE_ISAAC, CONDITION_ALWAYS, (s32)gField_LeaderScript,
                    300, 0, 300, 23472, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};
