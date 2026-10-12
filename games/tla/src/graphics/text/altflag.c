#include "types.h"
#include "ram_buffer.h"

struct WindowTileWork {
    u8 unknown_000[2];
    u8 alt;
    u8 unknown_003[0xe35];
    u8 table[256];
};

void UiWork_SetAltFlagAndClearTable(s32 alt)
{
    struct WindowTileWork *work = (struct WindowTileWork *)Ram_HeapSlots->window_tiles;
    s32 i;
    s32 fill;
    u8 *p;

    if (alt) {
        work->alt = 1;
        for (i = 128; i < 256; i++)
            work->table[i] = 0;
    } else {
        work->alt = 0;
        fill = 0;
        p = work->table + 128;
        for (i = 0; i < 128; i++)
            *p++ = fill;
    }
}

void Func_08041c50(void)
{
}
