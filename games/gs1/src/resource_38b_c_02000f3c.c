#include "types.h"

/* Per-site raw names preserve the stock relocated branch operands; they
 * resolve to Func_080090c8, Func_08009098 and Func_08009020 respectively. */
extern u8 *Func_02001f90();
extern void Func_02001f96();
extern void Func_02001fa8();
extern u8 Data_02009d7c[];

void Func_02000f3c(u8 *actor)
{
    u8 *spawned;
    u8 *child;
    s32 mask;

    spawned = Func_02001f90(24,
                            *(s32 *)(actor + 0x08),
                            *(s32 *)(actor + 0x0c),
                            *(s32 *)(actor + 0x10));
    if (spawned == 0)
        return;

    child = *(u8 **)(spawned + 0x50);
    Func_02001f96(spawned, Data_02009d7c);
    *(u8 *)(spawned + 0x55) = 0;
    *(u8 *)(spawned + 0x22) = 1;
    *(u8 *)(spawned + 0x23) = 2;
    if (child == 0)
        return;

    Func_02001fa8(child, 2);
    *(u8 *)(child + 0x26) = 0;
    mask = 13;
    mask = -mask;
    *(u8 *)(child + 5) = (u8)((*(u8 *)(child + 5) & mask) | 4);
    *(u8 *)(child + 9) |= 12;
}
