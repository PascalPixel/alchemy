#include "local.h"

void Func_02003c00(s32 index, s32 value)
{
    u8 *base = (u8 *)VENT_WORK;

    asm volatile(""); /* FAKEMATCH: ⚓️ loads the work address before it scales the index */
    ((struct VentRow *)(base + index * 20))->value = value;
}
