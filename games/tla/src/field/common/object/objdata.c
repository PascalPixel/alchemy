#include "objcmd.h"
#include "types.h"

//--------------------------------------------------------------------------
// Field object tables: the animation frame for each facing, the tile
// dissolve order, the fixed camera, and the small scripts the field and
// the world map start by themselves. Most are The Broken Seal's, byte for
// byte; its names for them are in the comments.
//--------------------------------------------------------------------------

// The frame for each facing of an animation kind (0x80 = mirrored).

// gAnimationFacing_Kind1
const u8 gData_0802eac4[16] =
{
    0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00,
    0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80,
};

// gAnimationFacing_Kind22
const u8 gData_0802ead4[8] =
{
    0x80, 0x80, 0x00, 0x00, 0x01, 0x01, 0x81, 0x81,
};

// gAnimationFacing_Kind2
const u8 gData_0802eadc[8] =
{
    0x00, 0x00, 0x80, 0x80, 0x01, 0x01, 0x81, 0x81,
};

// gAnimationFacing_Kind3
const u8 gData_0802eae4[16] =
{
    0x01, 0x01, 0x00, 0x00, 0x80, 0x80, 0x81, 0x81,
    0x81, 0x81, 0x82, 0x82, 0x02, 0x02, 0x02, 0x01,
};

// gAnimationFacing_Kind5: nothing in The Lost Age reads it.
const u8 gData_0802eaf4[16] =
{
    0x01, 0x00, 0x00, 0x03, 0x03, 0x80, 0x80, 0x81,
    0x81, 0x82, 0x82, 0x04, 0x04, 0x02, 0x02, 0x01,
};

// Sixty-four facings, The Lost Age's own.
const u8 gData_0802eb04[64] =
{
    0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x83, 0x80, 0x80, 0x80, 0x80,
    0x80, 0x80, 0x80, 0x80, 0x81, 0x81, 0x81, 0x81,
    0x81, 0x81, 0x81, 0x81, 0x82, 0x82, 0x82, 0x82,
    0x82, 0x82, 0x82, 0x82, 0x84, 0x04, 0x04, 0x04,
    0x04, 0x04, 0x04, 0x04, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01, 0x01,
};

// gAnimationFacing_Kind8
const u8 gData_0802eb44[8] =
{
    0x02, 0x01, 0x00, 0x07, 0x06, 0x05, 0x04, 0x03,
};

// The order a tile's 64 pixels dissolve in.
const u8 gMap_TileDissolveOrder[64] =
{
    4, 63, 12, 10, 44, 5, 1, 60, 46, 41, 3, 23, 29, 55, 51, 15,
    50, 25, 26, 57, 19, 49, 16, 61, 8, 58, 37, 17, 27, 31, 59, 53,
    11, 56, 33, 7, 30, 35, 36, 32, 42, 62, 21, 13, 2, 22, 45, 14,
    6, 48, 52, 9, 43, 28, 0, 40, 24, 34, 20, 47, 54, 39, 38, 18,
};

const u16 gMap_TileDissolveTail = 0x01ff;

// Two 1.0s (Q16), The Lost Age's own.
const s32 gData_0802eb90[2] =
{
    0x10000, 0x10000,
};

// gCamera_FixedViewMatrix (Q16)
const s32 gData_0802eb98[12] =
{
    0x10000, 0, 0,
    -0xcccc, 0x10000, 0,
    0, 0, 0x10000,
    0, 0, 0,
};

// gScriptObject_CycleTable: a triangle wave.
const s32 gData_0802ebc8[32] =
{
    1, 2, 3, 4, 5, 6, 7, 8,
    7, 6, 5, 4, 3, 2, 1, 0,
    -1, -2, -3, -4, -5, -6, -7, -8,
    -7, -6, -5, -4, -3, -2, -1, 0,
};

// The script an object returns to when its own one ends.
const s32 gScript_MainScript[1] =
{
    SC_END,
};

const s16 gScript_MainScriptTail[8] =
{
    0x1000, -0x2000, 0x3000, -0x4000, 0x6000, -0x8000, 0x4000, 0x0,
};

// gWorldMap_KeyHeadings: the heading for each direction pad state
// (0xffff = none).
const u16 gData_0802ec5c[16] =
{
    0xffff, 0x0000, 0x8000, 0xffff, 0xc000, 0xe000, 0xa000, 0xc000,
    0x4000, 0x2000, 0x6000, 0x4000, 0xffff, 0x0000, 0x8000, 0xffff,
};

// Wait, then release the object, as The Broken Seal's
// gWorldMap_FootprintActions do.
const s32 gData_0802ec7c[] =
{
    SC_WAIT(120),
    SC_FREE,
};

const s32 gData_0802ec88[] =
{
    SC_WAIT(20),
    SC_FREE,
};

const s32 gData_0802ec94[] =
{
    SC_WAIT(32),
    SC_FREE,
};

// gWorldMap_KeyYaws: the turn for each direction pad state.
const s32 gData_0802eca0[16] =
{
    0, -512, 512, 0,
    0, -384, 384, 0,
    0, -384, 384, 0,
    0, 0, 0, 0,
};

// Set the work byte at 0x63 to 15 and lower it by one every 5 frames, then
// clear the horizontal scale and release the object.
const s32 gData_0802ece0[] =
{
    SC_SET(SW_BYTE_63, 15),
    SC_LABEL(1),
    SC_WAIT(5),
    SC_ADD(SW_BYTE_63, -1),
    SC_LOOP(13, SC_TO(1)),
    SC_SET(SW_SCALE_X, 0),
    SC_WAIT(1),
    SC_FREE,
};

const s32 gData_0802ed28[] =
{
    SC_WAIT(60),
    SC_FREE,
};
