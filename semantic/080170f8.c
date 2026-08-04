#include "types.h"

u16 *Func_080170c4(u16 *, u16, s32);
void Func_0801e260(u16, u16, u16, u16);

/*
 * Draw a bordered rectangular window into the 32-column screen tilemap.
 * The alternate display mode substitutes the four corner tiles while sharing
 * the normal horizontal and vertical edge tiles.
 */
u16 *Func_080170f8(u16 x, u16 y, u16 width, u16 height) {
    u8 *engine;
    u16 *row;
    u16 *cursor;
    u16 top_left;
    u16 top_right;
    u16 bottom_left;
    u16 bottom_right;
    s32 interior_width;
    s32 row_advance;
    u32 interior_row;

    engine = *(u8 **)0x03001E8C;
    row = (u16 *)engine + y * 32 + x;
    if (width <= 1 || height <= 1 || width > 30 || height > 30) {
        return row;
    }

    Func_0801e260(x, y, width, height);
    if (engine[0xEA4] != 0) {
        top_left = 0xF01C;
        top_right = 0xF41C;
        bottom_left = 0xF81C;
        bottom_right = 0xFC1C;
    } else {
        top_left = 0xF010;
        top_right = 0xF012;
        bottom_left = 0xF013;
        bottom_right = 0xF015;
    }

    interior_width = width - 2;
    row_advance = 32 - width;

    *row++ = top_left;
    cursor = Func_080170c4(row, 0xF011, interior_width);
    *cursor++ = top_right;
    row = cursor + row_advance;

    for (interior_row = 1; interior_row < height - 1; interior_row++) {
        *row++ = 0xF016;
        cursor = Func_080170c4(row, 0xF020, interior_width);
        *cursor++ = 0xF017;
        row = cursor + row_advance;
    }

    *row++ = bottom_left;
    cursor = Func_080170c4(row, 0xF014, interior_width);
    *cursor = bottom_right;
    engine[0xEA3] = 1;
    return cursor;
}
