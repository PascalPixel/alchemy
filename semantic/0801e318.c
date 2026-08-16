#include "types.h"

extern u8 *Data_03001e8c;

void Func_0801e318(void)
{
    u16 *entry;
    u8 enabled;
    s32 rows;
    s32 width;
    u32 value;
    u32 reduced;
    s32 index;
    u8 *marker;
    s32 cleanup;

    enabled = (Data_03001e8c)[0xEA2];
    entry = (u16 *)(Data_03001e8c);
    width = 30;
    rows = 20;
    do {
        while ((width) != 0) {
            value = *entry++ & 0x3FF;
            reduced = value - 0x80;
            if (reduced <= 0x7F ||
                (enabled != 0 && value > 0x1FF && value <= 0x27F)) {
                index = ((value & 0xFF) ^ 0x80) + 0xDA0;
                (Data_03001e8c)[index] |= 2;
            }
            (width)--;
        }
        rows--;
    } while (rows != 0);

    cleanup = 0xFF;
    marker = (Data_03001e8c) + 0xDA0;
    do {
        if (*marker == 1)
            *marker = 0;
        cleanup--;
        marker++;
    } while (cleanup >= 0);
}
