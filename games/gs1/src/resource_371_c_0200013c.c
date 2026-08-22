#include "types.h"

extern u32 Data_03001e40;

typedef struct {
    u8 pad[9];
    u8 lo:2;
    u8 field:2;
    u8 hi:4;
} Record_0200013c;

u8 *Func_02004378();
void Func_02004618();
void Func_020043dc();
void Func_0200439c();

extern u8 Data_0200811d[];

void Func_0200013c(u8 *object)
{
    s32 scale;
    u8 *spawned;
    u8 *record;

    if ((Data_03001e40 & 4) != 0) {
        scale = 0x14ccc;
        *(s32 *)(object + 0x18) = scale;
        *(s32 *)(object + 0x1c) = scale;
    } else {
        scale = 0x10000;
        *(s32 *)(object + 0x18) = scale;
        *(s32 *)(object + 0x1c) = scale;
    }

    if ((Data_03001e40 & 2) == 0) {
        return;
    }

    {
        s32 x = *(s32 *)(object + 0x08);
        s32 y = *(s32 *)(object + 0x0c);
        s32 z = *(s32 *)(object + 0x10);
        spawned = Func_02004378(0x11d, x, y, z);
    }
    Func_02004618(0xf6);
    if (spawned == 0) {
        return;
    }

    {
        u8 *p = spawned + 0x55;
        s32 zero = 0;

        *p = zero;
        record = *(u8 **)(spawned + 0x50);
        ((Record_0200013c *)record)->field = 1;
        Func_020043dc(spawned, 0);
        Func_0200439c(spawned, 1);
        *(u16 *)(spawned + 0x64) = zero;
        *(s32 *)(spawned + 0x6c) = (s32)Data_0200811d;
    }
}
