// Effect tables of the field mode module, in the order of The Broken Seal's field_mode_tables.c:
// the pad's headings, Move's hand animations, four effect scripts and the three of
// FieldEffectDurationScripts, a paused actor's flicker, the arrow's wobble, the metatiles round a
// whirlwind and Blaze's tints.

#include "field/actor_script.h"
#include "field/field.h"
#include "field/scene_tables.h"

// clang-format off

// A heading for each value of the right, left, up and down bits of the +Control Pad, in eight ways (fdInputGetDpadAngleEightWay); the same as The Broken Seal's WorldMapCursorDirectionAngles.
const Uint16 InputDpadAnglesEightWay[16] = {
	DPAD_ANGLE_NONE, FACING_EAST,      FACING_WEST,      DPAD_ANGLE_NONE,
	FACING_NORTH,    FACING_NORTHEAST, FACING_NORTHWEST, FACING_NORTH,
	FACING_SOUTH,    FACING_SOUTHEAST, FACING_SOUTHWEST, FACING_SOUTH,
	DPAD_ANGLE_NONE, FACING_EAST,      FACING_WEST,      DPAD_ANGLE_NONE,
};

// Move's hand animation by the quarter turns from its side of the target to the push (fdPsynergySteerPushedTarget): pulling the target toward its side, it grips; pushing it away, it shows its palm; pushing it across, it is open; The Broken Seal's FieldEffectQuadrantModes.
const Uint8 MoveHandPushPoses[4] = {
	MOVE_HAND_ANIMATION_GRIP,
	MOVE_HAND_ANIMATION_OPEN,
	MOVE_HAND_ANIMATION_PALM,
	MOVE_HAND_ANIMATION_OPEN,
};

// Waits 60 ticks and releases the actor; The Broken Seal's FieldEffectBurstParticleActorScript.
const Sint32 GrowthLeafScript[] = {
	SC_WAIT(60),
	SC_RELEASE,
};

// Unused: the same four bytes as MoveHandPushPoses, as in The Broken Seal; nothing reads them.
const Uint8 FieldEffectUnusedQuadrantModes[4] = {3, 1, 2, 1};

// Force's fist: waits to arrive, sets off fdEffectSpawnHeavyImpact and releases the actor.
const Sint32 ForceFistScript[] = {
	SC_SYNC,
	SC_CALL(fdEffectSpawnHeavyImpact),
	SC_RELEASE,
};

// The three scripts of FieldEffectDurationScripts.
const Sint32 FieldEffectShortEffectScript[] = {
	SC_WAIT(24),
	SC_RELEASE,
};

const Sint32 FieldEffectMediumEffectScript[] = {
	SC_WAIT(46),
	SC_RELEASE,
};

const Sint32 FieldEffectLongEffectScript[] = {
	SC_WAIT(128),
	SC_RELEASE,
};

// The colour modes a halted actor flickers through at random (fdPsynergyUpdateHaltedActor): its own colours twice as often as each of the others.
const Sint8 HaltFlickerColorModes[8] = {
	SPRITE_COLOR_NORMAL, SPRITE_COLOR_NORMAL, SPRITE_COLOR_GRB,
	SPRITE_COLOR_RBG,    SPRITE_COLOR_BGR,    SPRITE_COLOR_GBR,
	SPRITE_COLOR_BRG,    SPRITE_COLOR_GREY,
};

// The arrow's wobble once it has struck, a step a tick over eight ticks (fdPsynergyUpdateArrow): turns from its heading, scaled by ArrowWobbleFades.
const Sint32 ArrowWobbleAngles[8] = {
	TRIG_TURN / 16, 0, -TRIG_TURN / 16, -TRIG_TURN / 8,
	-TRIG_TURN * 3 / 16, -TRIG_TURN / 8, -TRIG_TURN / 16, 0,
};

// The scale of the arrow's wobble, a step every eight ticks.
const Sint32 ArrowWobbleFades[4] = {
	toFIXED(13) / 16, toFIXED(9) / 16, toFIXED(5) / 16,
	toFIXED(3) / 16,
};

// The 21 {x, z} metatile offsets within two metatiles of a whirlwind's owner, the corners left out, that fdPsynergyGatherNearbyMetatiles looks at.
const Sint8 FieldPsynergyNearbyMetatileOffsets[42] = {
			-1, -2,  0, -2,  1, -2,
	-2, -1, -1, -1,  0, -1,  1, -1,  2, -1,
	-2,  0, -1,  0,  0,  0,  1,  0,  2,  0,
	-2,  1, -1,  1,  0,  1,  1,  1,  2,  1,
			-1,  2,  0,  2,  1,  2,
};

// The tints fdPsynergyBlaze passes to fdStartColorTransition, one for each look of the flame.
const Sint32 BlazeTintColors[4] = {
	RGB(16, 4, 4), RGB(12, 2, 12), RGB(12, 12, 2), RGB(2, 10, 10),
};
// clang-format on
