#include "types.h"

extern s32 *Data_03001e70;
extern s32 Data_02008f08[];
extern s32 Data_02008f20[];

extern s32 *Func_02001706(s32 slot);
extern void Func_0200177c(s32 tileX, s32 tileZ, s32 width, s32 height,
                          s32 worldX, s32 worldZ);
extern s32 Func_02000bf0(s32 layer, s32 tileX, s32 tileZ, s32 width,
                         s32 height, s32 alpha);
extern s32 Func_02000c02(s32 layer, s32 tileX, s32 tileZ, s32 width,
                         s32 height, s32 alpha);

struct Work {
    s32 slot;
    s32 unused;
    s32 x;
    s32 y;
    s32 z;
    s32 w;
};

s32 Func_020008c0(s32 slot)
{
    s32 *world = Data_03001e70;
    struct Work work;
    s32 *actor;
    s32 i;
    s32 index;
    s32 height;
    s32 width;
    s32 temporary;
    s32 tableIndex;
    s32 finalHeight;
    s32 worldX;
    s32 worldZ;
    s32 rawWidth;

    actor = Func_02001706(slot);
    i = 0;
    if (*(s16 *)((s32 *)actor[20])[10] == Data_02008f08[i]) {
        work.slot = i;
    } else {
        for (;;) {
            work.slot = 7;
            i = i + 1;
            if ((u32)i > 5)
                break;
            if (*(s16 *)((s32 *)actor[20])[10] == Data_02008f08[i]) {
                work.slot = i;
                break;
            }
        }
    }
    index = work.slot;
    if ((u32)index > 6)
        return 0;

    work.x = actor[2];
    work.y = actor[3];
    work.z = actor[4];

    tableIndex = index * 4;
    temporary = Data_02008f20[tableIndex + 1];
    if (temporary < 0)
        temporary = -temporary;
    height = Data_02008f20[tableIndex + 3];
    if (height < 0)
        height = -height;
    finalHeight = (temporary + height) >> 4;

    rawWidth = Data_02008f20[tableIndex];
    width = rawWidth;
    if (width < 0)
        width = -width;
    temporary = Data_02008f20[tableIndex + 2];
    if (temporary < 0)
        temporary = -temporary;

    work.x = work.x + (rawWidth << 16);
    work.z = work.z + (Data_02008f20[tableIndex + 1] << 16);
    /* The ROM normalizes x before z, then keeps both values through the call. */
    work.x = work.x >> 20;
    work.z = work.z >> 20;
    width = (width + temporary) >> 4;

    /* Both world-origin scalars are live before either stacked sum is stored. */
    worldX = world[79] >> 20;
    worldZ = world[80] >> 20;
    Func_0200177c(work.x, work.z, width, finalHeight,
                  worldX + work.x, worldZ + work.z);
    Func_02000bf0(0, work.x, work.z, width, finalHeight, 255);
    Func_02000c02(2, work.x, work.z, width, finalHeight, 255);
    return 1;
}
