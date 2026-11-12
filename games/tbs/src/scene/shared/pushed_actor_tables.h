/// @file
/// The tables of staged_actor.h, which every scene with pushed things holds:
/// the cell ahead for each of sixteen headings, and the six footprints.
///
/// An includer whose tables the ROM keeps in .rodata defines
/// PUSHED_ACTOR_TABLE_SECTION first.

#include "types.h"
#include "pushed_actor.h"

#ifndef PUSHED_ACTOR_TABLE_SECTION
#define PUSHED_ACTOR_TABLE_SECTION
#endif

s32 gPushedActor_DirectionSteps[] PUSHED_ACTOR_TABLE_SECTION = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

u32 gPushedActor_FootprintKinds[] PUSHED_ACTOR_TABLE_SECTION = {
    207, 205, 228, 229, 298, 297,
};

// clang-format off
s32 gPushedActor_FootprintBounds[] PUSHED_ACTOR_TABLE_SECTION = {
    -32, -8, 32, 8,
    -8, -32, 8, 32,
    -32, -16, 32, 0,
    -8, -32, 8, 32,
    -32, -8, 32, 8,
    -8, -32, 8, 32,
};
// clang-format on
