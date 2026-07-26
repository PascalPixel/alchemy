#include "types.h"

extern s32 Func_08004080(void);
extern void Func_0802875c(s32 arg0, s32 index);

void Func_080287a8(s32 arg0)
{
    u8 *base;
    u8 *entry;
    s16 idx;
    s32 slot;
    s32 off;
    s32 flags;

    base = *(u8 **)0x03001F38;
    idx = *(s16 *)(base + 142);
    if (idx <= 5)
    {
        *(u16 *)(base + 142) = *(u16 *)(base + 142) + 1;
        entry = base + idx * 20;
        slot = Func_08004080();
        Func_0802875c(slot, arg0);
        *(u16 *)(entry + 12) = idx * 24 + 32;
        flags = 136;
        *(u16 *)(entry + 14) = flags;
        off = idx + 132;
        *(u16 *)(entry + 18) = slot;
        base[off] = (u8) arg0;
    }
}
