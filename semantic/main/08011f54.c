#include "types.h"

extern u8 *Data_03001e70;
extern u8 Data_0202c000[];
extern u8 Data_0202c001[];
extern const void *Data_080134fc[];

void Func_080072f0(const void *, s32, s32, const void *);

void Func_08011f54(s32 index, s32 x, s32 y)
{
    u8 *table;
    u8 *map;
    s32 roundedX;
    s32 roundedY;
    s32 offset;
    u32 entryOffset;

    table = Data_03001e70;
    x >>= 16;
    y >>= 16;
    map = (u8 *)0x02010000;
    if (table != NULL) {
        offset = (index & 3) * 48 + 304;
        map = *(u8 **)(table + offset);
    }

    roundedX = x;
    if (x < 0)
        roundedX += 15;
    roundedY = y;
    if (y < 0)
        roundedY += 15;
    entryOffset = map[(((roundedY >> 4) << 7) + (roundedX >> 4)) * 4 + 3] * 4;
    Func_080072f0(
        Data_0202c001 + entryOffset,
        x & 15,
        y & 15,
        Data_080134fc[Data_0202c000[entryOffset] & 15]);
}
