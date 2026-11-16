/// @file
/// The facing tables the animated sprites choose their pictures by.

#include "field/actor_script.h"
#include "types.h"
#include "field/object.h"
#include "resource_ids.h"

//--------------------------------------------------------------------------
// Field object tables: the animation picture for each facing, the tile
// dissolve order, the fixed camera, and the small scripts the field and
// the world map start by themselves. Most are The Broken Seal's, byte for
// byte; its names for them are in the comments.
//--------------------------------------------------------------------------

// The frame for each facing of an animation kind (0x80 = mirrored).

/// gAnimationFacing_Kind1
// clang-format off
const u8 gAnimationFacing_OnePicture[16] = {
    0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00,
    0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80,
};

/// gAnimationFacing_Kind22
const u8 gAnimationFacing_FrontBackSwapped[8] = {
    0x80, 0x80, 0x00, 0x00, 0x01, 0x01, 0x81, 0x81,
};

/// gAnimationFacing_Kind2
const u8 gAnimationFacing_FrontBack[8] = {
    0x00, 0x00, 0x80, 0x80, 0x01, 0x01, 0x81, 0x81,
};

/// gAnimationFacing_Kind3
const u8 gAnimationFacing_ThreePictures[16] = {
    0x01, 0x01, 0x00, 0x00, 0x80, 0x80, 0x81, 0x81,
    0x81, 0x81, 0x82, 0x82, 0x02, 0x02, 0x02, 0x01,
};

/// gAnimationFacing_Kind5: nothing in The Lost Age reads it.
const u8 gAnimationFacing_FivePictures[16] = {
    0x01, 0x00, 0x00, 0x03, 0x03, 0x80, 0x80, 0x81,
    0x81, 0x82, 0x82, 0x04, 0x04, 0x02, 0x02, 0x01,
};

/// Sixty-four facings, The Lost Age's own.
const u8 gAnimationFacing_SixPictures[64] = {
    0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x83, 0x80, 0x80, 0x80, 0x80,
    0x80, 0x80, 0x80, 0x80, 0x81, 0x81, 0x81, 0x81,
    0x81, 0x81, 0x81, 0x81, 0x82, 0x82, 0x82, 0x82,
    0x82, 0x82, 0x82, 0x82, 0x84, 0x04, 0x04, 0x04,
    0x04, 0x04, 0x04, 0x04, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01, 0x01,
};

/// gAnimationFacing_Kind8
const u8 gAnimationFacing_EightPictures[8] = {
    0x02, 0x01, 0x00, 0x07, 0x06, 0x05, 0x04, 0x03,
};

/// The order a tile's 64 pixels dissolve in.
const u8 gSpriteDissolve_PixelOrder[64] = {
    4, 63, 12, 10, 44, 5, 1, 60, 46, 41, 3, 23, 29, 55, 51, 15,
    50, 25, 26, 57, 19, 49, 16, 61, 8, 58, 37, 17, 27, 31, 59, 53,
    11, 56, 33, 7, 30, 35, 36, 32, 42, 62, 21, 13, 2, 22, 45, 14,
    6, 48, 52, 9, 43, 28, 0, 40, 24, 34, 20, 47, 54, 39, 38, 18,
};

const u16 gSpriteDissolve_PixelOrderTail = 0x01ff;

/// Two 1.0s (16.16), The Lost Age's own.
const s32 gActorSprite_FullScale[2] = {
    0x10000, 0x10000,
};

/// The fixed view matrix (16.16), as The Broken Seal's.
const FIXED gCamera_FixedViewMatrix[12] = {
    0x10000, 0, 0,
    -0xcccc, 0x10000, 0,
    0, 0, 0x10000,
    0, 0, 0,
};

/// gScriptObject_CycleTable: a triangle wave.
const s32 gActorScript_CycleTable[32] = {
    1, 2, 3, 4, 5, 6, 7, 8,
    7, 6, 5, 4, 3, 2, 1, 0,
    -1, -2, -3, -4, -5, -6, -7, -8,
    -7, -6, -5, -4, -3, -2, -1, 0,
};

/// The script an object returns to when its own one ends.
const s32 gScript_MainScript[1] = {
    SC_END,
};

const s16 gActorScript_EndScriptTail[8] = {
    0x1000, -0x2000, 0x3000, -0x4000, 0x6000, -0x8000, 0x4000, 0x0,
};

/// The heading for each direction pad state (DPAD_INDEX), or
/// WORLD_MAP_KEY_HEADING_NONE.
const u16 gWorldMap_KeyHeadings[16] = {
    0xffff, 0x0000, 0x8000, 0xffff, 0xc000, 0xe000, 0xa000, 0xc000,
    0x4000, 0x2000, 0x6000, 0x4000, 0xffff, 0x0000, 0x8000, 0xffff,
};

/// Wait, then release the object.
// As The Broken Seal's gWorldMap_FootprintActions do.
const s32 gActor_FootprintScript[] = {
    SC_WAIT(120),
    SC_RELEASE,
};

const s32 gActor_SplashScript[] = {
    SC_WAIT(20),
    SC_RELEASE,
};

const s32 gActor_RippleScript[] = {
    SC_WAIT(32),
    SC_RELEASE,
};

/// The turn for each direction pad state.
const s32 gWorldMap_KeyYaws[16] = {
    0, -512, 512, 0,
    0, -384, 384, 0,
    0, -384, 384, 0,
    0, 0, 0, 0,
};
// clang-format on

/// Set the actor's riseEnabled to 15 and lower it by one every 5 frames, then
/// clear the horizontal scale and release the object.
const s32 gActor_WakeScript[] = {
    SC_SET(SW_RISE_ENABLED, 15),
    SC_LABEL(1),
    SC_WAIT(5),
    SC_ADD(SW_RISE_ENABLED, -1),
    SC_LOOP(13, SC_TO(1)),
    SC_SET(SW_SCALE_X, 0),
    SC_WAIT(1),
    SC_RELEASE,
};

const s32 gSand_GrainScript[] = {
    SC_WAIT(60),
    SC_RELEASE,
};

//--------------------------------------------------------------------------
// World map graphics in four sets, each a palette, two more resources and
// six tile sets, by resource id.
//--------------------------------------------------------------------------

const u32 gWorldMap_GraphicsResources[4][9] = {
    {
        RESOURCE_WORLD_MAP_PALETTE_A,
        RESOURCE_DEFAULT_MAP_ANIMATION,
        RESOURCE_DEFAULT_CELL_PICTURE_ATTRIBUTES,
        RESOURCE_WORLD_MAP_TILES_A1,
        RESOURCE_WORLD_MAP_TILES_A2,
        RESOURCE_WORLD_MAP_TILES_A3,
        RESOURCE_WORLD_MAP_TILES_A4,
        RESOURCE_WORLD_MAP_TILES_A5,
        RESOURCE_WORLD_MAP_TILES_A6,
    },
    {
        RESOURCE_WORLD_MAP_PALETTE_C,
        RESOURCE_WORLD_MAP_ANIMATION_C,
        RESOURCE_WORLD_MAP_CELL_PICTURE_ATTRIBUTES_C,
        RESOURCE_WORLD_MAP_TILES_C1,
        RESOURCE_WORLD_MAP_TILES_C2,
        RESOURCE_WORLD_MAP_TILES_C3,
        RESOURCE_WORLD_MAP_TILES_C4,
        RESOURCE_WORLD_MAP_TILES_C5,
        RESOURCE_WORLD_MAP_TILES_C6,
    },
    {
        RESOURCE_WORLD_MAP_PALETTE_B,
        RESOURCE_WORLD_MAP_ANIMATION_B,
        RESOURCE_WORLD_MAP_CELL_PICTURE_ATTRIBUTES_B,
        RESOURCE_WORLD_MAP_TILES_B1,
        RESOURCE_WORLD_MAP_TILES_B2,
        RESOURCE_WORLD_MAP_TILES_B3,
        RESOURCE_WORLD_MAP_TILES_B4,
        RESOURCE_WORLD_MAP_TILES_B5,
        RESOURCE_WORLD_MAP_TILES_B6,
    },
    {
        RESOURCE_WORLD_MAP_PALETTE_D,
        RESOURCE_WORLD_MAP_ANIMATION_D,
        RESOURCE_WORLD_MAP_CELL_PICTURE_ATTRIBUTES_D,
        RESOURCE_WORLD_MAP_TILES_D1,
        RESOURCE_WORLD_MAP_TILES_D2,
        RESOURCE_WORLD_MAP_TILES_D3,
        RESOURCE_WORLD_MAP_TILES_D4,
        RESOURCE_WORLD_MAP_TILES_D5,
        RESOURCE_WORLD_MAP_TILES_D6,
    },
};
