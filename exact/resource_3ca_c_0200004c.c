/*
 * BYTE-EXACT and adopted 2026-08-07 with -fthumb-store-value-before-base: the
 * last residual was at 0x02000060, where the reference materialises the stored
 * zero before the split constant 0x05000000 that addresses it.
 */

#include "types.h"

/* Complete shared next-scene dispatch and backdrop-palette clear owner. */
struct SceneWork_0200004c {
    u8 reserved000[364];
    s16 next_scene;
};

extern struct SceneWork_0200004c *Data_03001ebc;


extern void Func_020014c2(s32 scene);
void Func_0200004c(void)
{
    Func_020014c2(Data_03001ebc->next_scene);
    {
        s32 zero = 0;
        volatile u16 *reg;
        reg = (volatile u16 *)0x05000000;
        *reg = (u16)zero;
        (void)reg;
    }
}
