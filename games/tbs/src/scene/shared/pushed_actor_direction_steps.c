/// @file
/// The cell ahead of a pushed thing for each of sixteen headings, one cell
/// (16 pixels) along x or z, which every scene with pushed things holds.

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
