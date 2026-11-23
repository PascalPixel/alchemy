/// @file
/// The tables of staged_actor.c, which every scene with pushed things holds:
/// the cell ahead for each of sixteen headings, and the six rectangles.

#include "pushed_actor_direction_steps.c"

u32 gPushedActor_RectangleKinds[] PUSHED_ACTOR_TABLE_SECTION = {
    207, 205, 228, 229, 298, 297,
};

/// Each footprint's left, top, right and bottom edges, in pixels from the
/// thing's position: four cells by one, or a pipe's four by one north of it.
// clang-format off
s32 gPushedActor_Rectangles[] PUSHED_ACTOR_TABLE_SECTION = {
    -32, -8, 32, 8,
    -8, -32, 8, 32,
    -32, -16, 32, 0,
    -8, -32, 8, 32,
    -32, -8, 32, 8,
    -8, -32, 8, 32,
};
// clang-format on
