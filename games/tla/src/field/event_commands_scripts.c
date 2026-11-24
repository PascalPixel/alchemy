/// @file
/// The particle scripts of the field commands after the rope and vine
/// pictures, Mind Read's palette cycle and the pad's headings, in the order of
/// The Broken Seal's field_sequence_tables.c.

#include "fixed_math.h"
#include "types.h"
#include "field/actor_script.h"
#include "field/event_commands.h"
#include "field/field.h"
#include "field/scene_tables.h"

/// Unused: one word, 17, which no code names.
const s32 gFieldEffect_UnusedWord[1] = {17};

/// [0] only releases the actor, +1 waits for it to arrive and releases it.
const s32 gFieldEffect_ReleaseScript[] = {
    SC_RELEASE,
    SC_SYNC,
    SC_RELEASE,
};

/// Faces the watched actor every frame, forever: the caster holding its
/// target while Move, Carry or Lift has it (each gives the caster its target
/// first).
const s32 gFieldEffect_SourceHoldScript[] = {
    SC_FACE_WATCHED,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// A shard or particle: waits to arrive, slows to 1/16 pixel a frame, and
/// releases itself 10 frames later.
const s32 gFieldEffect_FragmentScript[] = {
    SC_SYNC, SC_SPEED(0.0625), SC_ANIMATION(0), SC_WAIT(10), SC_RELEASE,
};

/// The sprite palettes Actor_SetLayersColorMode cycles through for value 0
/// ((gFrameCount >> 1) & 7): 1 to 4 and back to 0, two frames each; The Broken
/// Seal's gMindRead_CasterPaletteCycle.
const u8 gAnim_ChildPaletteCycle[8] = {1, 2, 3, 4, 3, 2, 1, 0};

// clang-format off

/// An angle for each value of the right, left, up and down bits of the +Control
/// Pad; DPAD_ANGLE_NONE for none (Input_GetDpadAngle, event_commands_mind_read.c).
const u16 gInput_DpadAngles[16] = {
    DPAD_ANGLE_NONE, FACING_EAST,     FACING_WEST,     DPAD_ANGLE_NONE,
    FACING_NORTH,    DPAD_ANGLE_NONE, DPAD_ANGLE_NONE, FACING_NORTH,
    FACING_SOUTH,    DPAD_ANGLE_NONE, DPAD_ANGLE_NONE, FACING_SOUTH,
    DPAD_ANGLE_NONE, FACING_EAST,     FACING_WEST,     DPAD_ANGLE_NONE,
};
// clang-format on
