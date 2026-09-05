#include "types.h"

s32 Func_02006aca();
s32 Func_02006ad8();

void Func_02003460(u8 *work)
{
    s32 parent;
    s32 phase;
    s32 next;
    s32 x;
    s32 y;

    parent = *(s32 *)(work + 104);
    phase = *(u16 *)(work + 100);
    x = Func_02006aca(phase);
    *(volatile s32 *)(work + 8) = (*(s32 *)(parent + 8) + (x * (*(s32 *)(work + 48) + 28)));
    y = Func_02006ad8(phase);
    *(volatile s32 *)(work + 16) = ((y << 4) + 0x900000);
    *(volatile s32 *)(work + 56) = *(s32 *)(work + 8);
    *(volatile s32 *)(work + 64) = ((y << 4) + 0x900000);
    next = *(volatile u16 *)(work + 100);
    next = next + -0x200;
    *(volatile u16 *)(work + 100) = (u16)next;
}
