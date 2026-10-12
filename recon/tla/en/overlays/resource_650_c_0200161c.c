/* Near miss, differ 14 (registers only): the listing keeps the palette in r0
 * and counts in r4; this draft keeps the palette in r4 and counts in r0.
 * Indexing colors[i] and colors[128 + i] scores 22 and pools the zero. */
#include "types.h"
#include "ram_buffer.h"

void Func_020032f0(s32, s32);

struct FadePalette {
    u16 colors[256];
};

void Func_0200161c(void)
{
    struct FadePalette *palette = Ram_HeapSlots->block_32;
    u16 *to = palette->colors;
    u16 *from = palette->colors + 128;
    u32 i;

    for (i = 0; i < 64; i++)
        *to++ = *from++;
    palette->colors[0] = 0;
    Func_020032f0(0x10000, 0);
}
