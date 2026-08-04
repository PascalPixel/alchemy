#include "types.h"

static inline void CopyEntry(
    s32 mapIndex,
    s32 screenY,
    s32 screenX,
    u8 *screenBase)
{
    u32 entry;
    u32 top;
    volatile u32 *destination;

    entry = ((u32 *)0x02010000)[mapIndex];
    entry = (entry << 20) >> 17;
    top = *(u32 *)(0x02020000 + entry);
    destination =
        (volatile u32 *)(screenBase + ((screenY + screenX) << 1));
    destination[0] = top;
    destination[16] = *(u32 *)(0x02020004 + entry);
}

void Func_0800fec8(s32 screen, s32 x, s32 y)
{
    u8 *screenBase;
    s32 mapX;
    s32 mapY;
    s32 screenX;
    s32 screenY;
    u32 count;

    screenBase = (u8 *)0x06002800 + screen * 0x800;
    mapY = ((y / 2) & 0x7f) << 7;
    screenY = (y & 0x1e) << 5;
    mapX = (x / 2) & 0x7f;
    screenX = x & 0x1e;
    count = 0;
    do {
        CopyEntry(mapY + mapX, screenY, screenX, screenBase);
        mapX = (mapX + 1) & 0x7f;
        screenX = (screenX + 2) & 0x1e;
        count++;
    } while (count <= 15);
}
