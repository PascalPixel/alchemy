#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "ram_buffer.h"

// Copies a width by height block of colour indices onto the battle canvas
// through its cell map at x, y, skipping zeros; nothing is drawn when the block
// starts off the map or its far edge passes cell 63.
void Func_080eb4a0(const u8 *src, s32 x, s32 y, s32 width, s32 height)
{
    u16 *map = Ram_HeapSlots->canvasMap;
    u8 *canvas = Ram_HeapSlots->battleFxWork[1];
    s32 row;
    s32 n;

    if (x < 0 || x + width > 63 || y < 0 || y + height > 63)
        return;
    for (row = 0; row < height; row++) {
        for (n = 0; n < width; n++) {
            if (*src != 0)
                canvas[map[((y + row) << 6) + x + n]] = *src;
            src++;
        }
    }
}

void ColorBuffer_MapToFadeStep1(u8 *buffer, s32 count)
{
    if (count > 0) {
        const u8 *table = ColorBuffer_FadeStep1Map;

        do {
            if (*buffer != 0)
                *buffer = table[*buffer & 31];
            count--;
            buffer++;
        } while (count != 0);
    }
}

void ColorBuffer_MapToFadeStep2(u8 *buffer, s32 count)
{
    if (count > 0) {
        const u8 *table = ColorBuffer_FadeStep2Map;

        do {
            if (*buffer != 0)
                *buffer = table[*buffer & 31];
            count--;
            buffer++;
        } while (count != 0);
    }
}

void ColorBuffer_MapToFadeStep3(u8 *buffer, s32 count)
{
    if (count > 0) {
        const u8 *table = ColorBuffer_FadeStep3Map;

        do {
            if (*buffer != 0)
                *buffer = table[*buffer & 31];
            count--;
            buffer++;
        } while (count != 0);
    }
}
