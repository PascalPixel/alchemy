#include "types.h"

struct WindowFrame_0801ef68 {
    u8 unknown_00[8];
    u16 width;
    u16 height;
    u16 x;
    u16 y;
};

/*
 * Draw the menu's vertical separators and, when the alternate display mode
 * is active, its bottom border into the 32-column background tilemap.
 */
void Func_0801ef68(struct WindowFrame_0801ef68 *window, s32 flags)
{
    u16 *tilemap = *(u16 **)0x03001e8c;
    u8 *display_state = (u8 *)tilemap;
    const s8 *separator_offsets = (const s8 *)0x080371c4;
    u32 right_edge = window->width - 1;
    s32 first_separator = 1;
    s32 separator_adjustment = 0;
    s32 separator;

    if ((flags & 1) == 0) {
        flags &= ~2;
    }
    if ((flags & 2) != 0) {
        separator_adjustment = 5;
        first_separator = 0;
    }

    separator = first_separator;
    while (separator_offsets[separator] >= 0) {
        u32 column =
            (u32)(separator_offsets[separator] + separator_adjustment);

        if (column < right_edge) {
            u32 row;

            for (row = 0; row < window->height; row++) {
                u16 tile;

                if (row == 0) {
                    tile = 0xf018;
                } else if (row == window->height - 1) {
                    tile = 0xf019;
                } else {
                    tile = 0xf00f;
                }

                tilemap[(window->y + row) * 32 + window->x + column] = tile;
            }
        }
        separator++;
    }

    if (display_state[0x0ea5] != 0) {
        u16 *border =
            &tilemap[(window->y + window->height - 1) * 32 + window->x];
        u32 column = 1;

        *border++ = 0xf080;
        while (column < right_edge) {
            *border++ = 0xf081;
            column++;
        }
        *border = 0xf082;
    }

    display_state[0x0ea3] = 1;
}
