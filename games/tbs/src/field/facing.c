#include "unit/field/facing.h"
#include "types.h"
#include "objcmd.h"

/// Facing tables: for each enum AnimationFacingKind, the frame to draw for
/// each sector of the facing angle. Sprite_ComposeFrame adds the low
/// three bits to the animation's base frame, and bit 7 (0x80) mirrors the
/// sprite. ANIM_FACING_TWO_PART uses ANIM_FACING_FRONT_BACK's table.
/// gAnimationFacing_OneFrame has 16 entries, but a 16-bit angle shifted down
/// 13 reads only the first eight. Sectors count from angle
/// 0, which faces right; 0x4000 faces down the screen (gWorldMap_KeyHeadings).
// clang-format off
const u8 gAnimationFacing_OneFrame[16] = {
    0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00,
    0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80,
};

const u8 gAnimationFacing_FrontBackSwapped[8] = {
    0x80, 0x80, 0x00, 0x00, 0x01, 0x01, 0x81, 0x81,
};

const u8 gAnimationFacing_FrontBack[8] = {
    0x00, 0x00, 0x80, 0x80, 0x01, 0x01, 0x81, 0x81,
};

const u8 gAnimationFacing_ThreeFrames[16] = {
    0x01, 0x01, 0x00, 0x00, 0x80, 0x80, 0x81, 0x81,
    0x81, 0x81, 0x82, 0x82, 0x02, 0x02, 0x02, 0x01,
};

const u8 gAnimationFacing_FiveFrames[16] = {
    0x01, 0x00, 0x00, 0x03, 0x03, 0x80, 0x80, 0x81,
    0x81, 0x82, 0x82, 0x04, 0x04, 0x02, 0x02, 0x01,
};

const u8 gAnimationFacing_EightFrames[8] = {
    0x02, 0x01, 0x00, 0x07, 0x06, 0x05, 0x04, 0x03,
};

const u8 gAnimationFacing_EightFramesMirrored[8] = {
    0x01, 0x00, 0x03, 0x85, 0x86, 0x87, 0x04, 0x02,
};

const u8 gAnimationFacing_FourFrames[64] = {
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
    0x80, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81,
    0x81, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x83,
    0x83, 0x83, 0x83, 0x83, 0x83, 0x83, 0x83, 0x83,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x01,
};

const u8 gAnimationFacing_SixFrames[64] = {
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04,
    0x04, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
    0x80, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81,
    0x81, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x83,
    0x83, 0x83, 0x83, 0x83, 0x83, 0x83, 0x83, 0x05,
    0x05, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x01,
};

/// The order in which the 64 pixels of a sprite's tile in OBJ VRAM dissolve;
/// read by SpriteDissolve_Step (objects.c).
const u8 gSpriteDissolve_PixelOrder[64] = {
    4, 63, 12, 10, 44, 5, 1, 60, 46, 41, 3, 23, 29, 55, 51, 15,
    50, 25, 26, 57, 19, 49, 16, 61, 8, 58, 37, 17, 27, 31, 59, 53,
    11, 56, 33, 7, 30, 35, 36, 32, 42, 62, 21, 13, 2, 22, 45, 14,
    6, 48, 52, 9, 43, 28, 0, 40, 24, 34, 20, 47, 54, 39, 38, 18,
};

/// Two bytes after the dissolve order; no code in The Broken Seal names them.
const u16 gSpriteDissolve_PixelOrderTail = 0x01ff;

/// The fixed camera's view matrix (Q16): identity with a -0.8 shear of y by x.
/// ActorSystem_ProjectSprites applies it while battle commands are chosen.
const s32 gCamera_FixedViewMatrix[12] = {
    0x10000, 0, 0,
    -0xcccc, 0x10000, 0,
    0, 0, 0x10000,
    0, 0, 0,
};

/// One period of a triangle wave, which bobs a floating object up and down.
/// Actor_UpdateAllThumb reads it at half the object's phase, scaled by its
/// gravity.
const s32 gActorScript_CycleTable[32] = {
    1, 2, 3, 4, 5, 6, 7, 8,
    7, 6, 5, 4, 3, 2, 1, 0,
    -1, -2, -3, -4, -5, -6, -7, -8,
    -7, -6, -5, -4, -3, -2, -1, 0,
};

/// The idle script an object returns to when its own one ends: a lone SC_END,
/// which reloads it. ActorScript_End (SC_END) points an object here.
const s32 gActorScript_EndScript[1] = {
    SC_END,
};

/// Halfwords that follow the main script; no code in The Broken Seal names
/// them.
const s16 gActorScript_EndScriptTail[8] = {
    0x1000, -0x2000, 0x3000, -0x4000, 0x6000, -0x8000, 0x4000, 0x0,
};

/// The heading for each state of the direction pad (bits 4-7 of the keys:
/// Right, Left, Up, Down), in 1/65536 turns: Right is 0, Down 0x4000, Left
/// 0x8000 and Up 0xc000; 0xffff means no direction is held, or only an
/// opposite pair. Read by Actor_UpdatePlayerControl,
/// Actor_MoveOnWorldMap and Actor_MoveByKeys.
const u16 gWorldMap_KeyHeadings[16] = {
    0xffff, 0x0000, 0x8000, 0xffff, 0xc000, 0xe000, 0xa000, 0xc000,
    0x4000, 0x2000, 0x6000, 0x4000, 0xffff, 0x0000, 0x8000, 0xffff,
};

/// Two scripts for the world map's footprint and dust effects: wait 120 or 30
/// frames, then release the object. Actor_UpdatePlayerControl starts
/// the first and Actor_MoveOnWorldMap the second (three words in).
const s32 gWorldMap_FootprintActions[6] = {
    SC_WAIT(120), SC_RELEASE,
    SC_WAIT(30), SC_RELEASE,
};

/// The yaw the world map camera leans to for each state of the direction pad,
/// in 1/65536 turns: Right -512 and Left 512, or -384 and 384 with Up or Down
/// but not both; 0 otherwise. It is a target, not a rate: each frame
/// Actor_MoveOnWorldMap turns the camera an eighth of the way there, at most
/// 0x200, and the whole way once that step is under 16, so with the pad
/// released it returns to 0.
const s32 gWorldMap_KeyYaws[16] = {
    0, -512, 512, 0,
    0, -384, 384, 0,
    0, -384, 384, 0,
    0, 0, 0, 0,
};
// clang-format on
