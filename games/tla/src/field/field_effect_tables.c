/// @file
/// Effect tables of the field sequence bank, in the order of The Broken Seal's
/// field_sequence_tables.c: the pad's headings, Move's hand animations, four
/// effect scripts and the three of gFieldEffect_DurationScripts, a paused
/// actor's flicker, the arrow's wobble, the cells round a whirlwind and Blaze's
/// tints.

#include "fixed_math.h"
#include "types.h"
#include "field/actor_script.h"
#include "field/event_commands.h"
#include "field/field.h"
#include "field/field_psynergy.h"
#include "field/scene_tables.h"
#include "field/sprite.h"
#include "battle/battle.h"
#include "system/io_reg.h"

// clang-format off

/// A heading for each value of the right, left, up and down bits of the
/// +Control Pad, in eight ways (Input_GetDpadAngleEightWay); the same
/// as The Broken Seal's gWorldMap_CursorDirectionAngles.
const u16 gInput_DpadAnglesEightWay[16] = {
    DPAD_ANGLE_NONE, FACING_EAST,      FACING_WEST,      DPAD_ANGLE_NONE,
    FACING_NORTH,    FACING_NORTHEAST, FACING_NORTHWEST, FACING_NORTH,
    FACING_SOUTH,    FACING_SOUTHEAST, FACING_SOUTHWEST, FACING_SOUTH,
    DPAD_ANGLE_NONE, FACING_EAST,      FACING_WEST,      DPAD_ANGLE_NONE,
};

/// Move's hand animation by the quarter turns from its side of the target to
/// the push (FieldPsynergy_SteerPushedTarget): pulling the target toward its
/// side, it grips (MOVE_HAND_ANIMATION_GRIP, 3); pushing it away (2) or across
/// (MOVE_HAND_ANIMATION_OPEN, 1), it is open, both animations holding the open
/// hand's picture; The Broken Seal's gFieldEffect_QuadrantModes.
const u8 gMoveHand_PushPoses[4] = {3, 1, 2, 1};

/// Waits 60 frames and releases the actor; The Broken Seal's
/// gFieldEffect_BurstParticleActorScript.
const s32 gGrowth_LeafScript[] = {
    SC_WAIT(60),
    SC_RELEASE,
};

/// Unused: the same four bytes as gMoveHand_PushPoses, as in The Broken
/// Seal; nothing reads them.
const u8 gFieldEffect_UnusedQuadrantModes[4] = {3, 1, 2, 1};

/// Force's fist: waits to arrive, sets off FieldEffect_SpawnHeavyImpact and
/// releases the actor.
const s32 gForce_FistScript[] = {
    SC_SYNC,
    SC_CALL(FieldEffect_SpawnHeavyImpact),
    SC_RELEASE,
};

/// The three scripts of gFieldEffect_DurationScripts.
const s32 gFieldEffect_ShortEffectScript[] = {
    SC_WAIT(24),
    SC_RELEASE,
};

const s32 gFieldEffect_MediumEffectScript[] = {
    SC_WAIT(46),
    SC_RELEASE,
};

const s32 gFieldEffect_LongEffectScript[] = {
    SC_WAIT(128),
    SC_RELEASE,
};

/// The colour modes a halted actor flickers through at random
/// (FieldPsynergy_UpdateHaltedActor): its own colours twice as often as each
/// of the others.
const s8 gHalt_FlickerColorModes[8] = {
    SPRITE_COLOR_NORMAL, SPRITE_COLOR_NORMAL, SPRITE_COLOR_GRB,
    SPRITE_COLOR_RBG,    SPRITE_COLOR_BGR,    SPRITE_COLOR_GBR,
    SPRITE_COLOR_BRG,    SPRITE_COLOR_GREY,
};

/// The arrow's wobble once it has struck, a step a frame over eight frames
/// (FieldPsynergy_UpdateArrow): turns from its heading, scaled by
/// gArrow_WobbleFades.
const s32 gArrow_WobbleAngles[8] = {
    TRIG_TURN / 16, 0, -TRIG_TURN / 16, -TRIG_TURN / 8,
    -TRIG_TURN * 3 / 16, -TRIG_TURN / 8, -TRIG_TURN / 16, 0,
};

/// The scale of the arrow's wobble, a step every eight frames.
const s32 gArrow_WobbleFades[4] = {
    FIXED_ONE * 13 / 16, FIXED_ONE * 9 / 16, FIXED_ONE * 5 / 16,
    FIXED_ONE * 3 / 16,
};

/// The 21 {x, z} cell offsets within two cells of a whirlwind's owner, the
/// corners left out, that FieldPsynergy_GatherNearbyCells looks at.
const s8 gFieldPsynergy_NearbyCellOffsets[42] = {
            -1, -2,  0, -2,  1, -2,
    -2, -1, -1, -1,  0, -1,  1, -1,  2, -1,
    -2,  0, -1,  0,  0,  0,  1,  0,  2,  0,
    -2,  1, -1,  1,  0,  1,  1,  1,  2,  1,
            -1,  2,  0,  2,  1,  2,
};

/// The tints FieldPsynergy_Blaze passes to Field_StartColorTransition, one for
/// each look of the flame.
const s32 gBlaze_TintColors[4] = {
    RGB(16, 4, 4), RGB(12, 2, 12), RGB(12, 12, 2), RGB(2, 10, 10),
};
// clang-format on
