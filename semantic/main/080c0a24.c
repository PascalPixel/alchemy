#include "types.h"

/*
 * Rebuild one affine layer's transform and its 160-entry per-scanline row
 * table, then flip the layer to the buffer that was just written.
 *
 * The layer, the mode block and the camera all hang off the pointer table at
 * 0x03001f00: the layer is the word 136 bytes below it and the camera the
 * word 128 bytes below it.  With no layer there is nothing to do.  Depths of
 * 0x10000 and above select the far preset - row tint 0x2000 and a vertical
 * bias of 0x6800 less three times the camera's halfword at +0x36 - and
 * anything nearer uses a bias of 0x800 and no tint.
 *
 * While the mode word at +0x08 or +0x0c is 1 and the word at +0x10 is clear,
 * the bias is also published as the halfword at 0x03001ad4.  Only mode 2
 * rebuilds the layer; every other mode returns here.
 *
 * The transform is the reciprocal of the depth: 0x10000/depth, kept as the
 * scale, with its top eight bits dropped for the affine pair, written as
 * (pa, 0, 0, pd).  The reference point is the caller's x and y scaled by
 * that reciprocal and by depth - 0x10000, rounded with +0x7fff before the
 * shift, and offset by the caller's origin, the bias for x and -0x1000 for y.
 * The first row the layer covers follows from the ratio of the remaining
 * height 0x4000 - y to pa.
 *
 * The row table is written in four runs over 160 scanlines: rows 0..15 get
 * the sky word 0x3f8e unless the flag at 363 is set, rows up to the computed
 * first row - never past 136 - get the tint over 0x478a, rows up to 135 get
 * the tint over 0x478e, and rows up to 159 get 0x3f8e again.  The runs share
 * the same cursor, so a run that starts past its own end writes nothing.
 *
 * Relocated helpers, none of them retained assembly: `bl Func_080072ec` at
 * 0x080c0aba and 0x080c0b2a is the runtime's `_call_via_r2` thunk inside
 * 0x080072e4, reaching the pooled 0x0300013c, the ratio helper - which
 * returns its second argument over its first; and the four `mov ip, pc ;
 * bx r4` sequences from 0x080c0ae0 to 0x080c0af4 call the pooled 0x03000118,
 * the IWRAM-relocated ARM fixed-point multiply, in the same frame, the
 * reading established for 0x0800ebec.  Both are written as ordinary calls
 * through function pointers.
 *
 * The epilogue at 0x080c0bd2 ends `pop {r0} ; bx r0`, so the owner is void.
 * Seven direct callers, the first at 0x080b593e.
 */

/* 0x0300013c, the IWRAM ratio helper: second argument over first. */
typedef s32 (*Ratio_080c0a24)(s32 denominator, s32 numerator);

/* 0x03000118, the IWRAM-relocated ARM fixed-point multiply. */
typedef s32 (*Multiply_080c0a24)(s32 a, s32 b);

s32 Func_080770c0(s32 flag);

struct Mode_080c0a24 {
    u8 unknown_00[8];
    s32 mode;
    s32 alternate_mode;
    s32 inhibit;
};

struct Camera_080c0a24 {
    u8 unknown_00[0x36];
    s16 height;
};

struct Layer_080c0a24 {
    u32 buffer;
    u8 unknown_04[0xc];
    s16 pa;
    s16 pb;
    s16 pc;
    s16 pd;
    s32 x;
    s32 y;
    u16 rows[2][160];
};

void Func_080c0a24(s32 origin_x, s32 origin_y, s32 offset_x, s32 offset_y,
                   s32 depth)
{
    Ratio_080c0a24 ratio = (Ratio_080c0a24)0x0300013c;
    Multiply_080c0a24 multiply = (Multiply_080c0a24)0x03000118;
    struct Layer_080c0a24 *layer;
    struct Mode_080c0a24 *mode;
    struct Camera_080c0a24 *camera;
    u16 *cursor;
    u16 tint;
    s32 bias;
    s32 scale;
    s16 affine;
    s32 mode_value;
    s32 first_row;
    u32 row;
    u16 value;

    layer = *(struct Layer_080c0a24 **)(0x03001f00 - 136);
    mode = *(struct Mode_080c0a24 **)0x03001f00;
    camera = *(struct Camera_080c0a24 **)(0x03001f00 - 128);
    bias = 0x800;
    tint = 0;
    if (depth >= 0x10000) {
        tint = 0x2000;
        bias = 0x6800 + -(s32)camera->height * 3;
    }

    if (layer == NULL) {
        return;
    }

    mode_value = mode->mode;
    if (mode_value == 1 || mode->alternate_mode == 1) {
        if (mode->inhibit == 0) {
            *(s16 *)(0x03001ad0 + 4) = (s16)(bias >> 8);
        }
    }
    if (mode_value != 2) {
        return;
    }

    cursor = layer->rows[layer->buffer ^ 1];

    scale = ratio(depth, 0x10000);
    affine = (s16)(scale >> 8);
    layer->pa = affine;
    layer->pb = 0;
    layer->pc = 0;
    layer->pd = affine;

    layer->x = ((multiply(scale, multiply(origin_x, depth - 0x10000)) + 0x7fff)
                >> 8) + offset_x + bias;
    layer->y = ((multiply(scale, multiply(origin_y, depth - 0x10000)) + 0x7fff)
                >> 8) + offset_y - 0x1000;

    first_row = (ratio(affine, 0x4000 - layer->y) >> 16) + 1;

    row = 0;
    if (Func_080770c0(363) == 0) {
        do {
            row++;
            *cursor++ = 0x3f8e;
        } while (row <= 15);
    }

    if ((u32)first_row > 136) {
        first_row = 136;
    }
    if (row < (u32)first_row) {
        value = (u16)(tint | 0x478a);
        do {
            row++;
            *cursor++ = value;
        } while (row < (u32)first_row);
    }

    if (row <= 135) {
        value = (u16)(tint | 0x478e);
        do {
            row++;
            *cursor++ = value;
        } while (row <= 135);
    }

    if (row <= 159) {
        do {
            row++;
            *cursor++ = 0x3f8e;
        } while (row <= 159);
    }

    layer->buffer ^= 1;
}
