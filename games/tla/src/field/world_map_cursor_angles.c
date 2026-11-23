/// @file
/// Which way the cursor of the world map screen moves for the +Control Pad
/// (WorldPicture_UpdateMarkers).

#include "fixed_math.h"
#include "types.h"
#include "field/event_commands.h"
#include "field/scene_tables.h"

// clang-format off

/// A heading for each value of the right, left, up and down bits of the
/// +Control Pad, in eight ways; DPAD_ANGLE_NONE when none is held, or right
/// and left or up and down together. The same table as
/// gBattleEffect_EightWayHeadingWords, which The Broken Seal keeps once.
const u16 gWorldMap_CursorDirectionAngles[16] = {
    DPAD_ANGLE_NONE, FACING_EAST,      FACING_WEST,      DPAD_ANGLE_NONE,
    FACING_NORTH,    FACING_NORTHEAST, FACING_NORTHWEST, FACING_NORTH,
    FACING_SOUTH,    FACING_SOUTHEAST, FACING_SOUTHWEST, FACING_SOUTH,
    DPAD_ANGLE_NONE, FACING_EAST,      FACING_WEST,      DPAD_ANGLE_NONE,
};
// clang-format on
