/// @file
/// The map screen's cursor headings (MapScreen_UpdateMarkers).

#include "fixed_math.h"
#include "types.h"
#include "field/event_commands.h"
#include "field/scene_tables.h"

// clang-format off

/// An angle for each +Control Pad direction (eight of them); DPAD_ANGLE_NONE
/// for none. A second copy of The Broken Seal's table, beside
/// gBattleEffect_EightWayHeadingWords.
const u16 gWorldMap_CursorDirectionAngles[16] = {
    DPAD_ANGLE_NONE, FACING_EAST,      FACING_WEST,      DPAD_ANGLE_NONE,
    FACING_NORTH,    FACING_NORTHEAST, FACING_NORTHWEST, FACING_NORTH,
    FACING_SOUTH,    FACING_SOUTHEAST, FACING_SOUTHWEST, FACING_SOUTH,
    DPAD_ANGLE_NONE, FACING_EAST,      FACING_WEST,      DPAD_ANGLE_NONE,
};
