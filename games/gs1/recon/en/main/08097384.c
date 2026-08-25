#include "types.h"

#define BattleEffect_InitializeSharedScene Func_08097384

void BattleEffect_InitializeSharedScene(void)
{
    volatile u32 *dma = (volatile u32 *)0x040000d4;
    u8 **state = (u8 **)0x03001ebc;
    u8 *scene = state[0];
    u8 *workspace = state[5];
    s32 selection = *(s32 *)0x03001e40 & 7;

    dma[0] = (u32)(workspace + 0x1340);
    dma[1] = (u32)(scene + 0x776);
    dma[2] = 0x84000150;
    if (*(s16 *)(scene + 0xcb8) == 0) {
        dma[0] = (u32)(workspace + 0xe00);
        dma[1] = (u32)(scene + 0x236);
        dma[2] = 0x84000150;
    }
    dma[0] = (u32)(workspace + 0xe00);
    dma[1] = (u32)(workspace + 0x380);
    dma[2] = 0x840002a0;

    if (Func_080770c0(0x148)) selection = 0;
    if (Func_080770c0(0x149)) selection = 1;
    if (Func_080770c0(0x14a)) selection = 2;
    if (Func_080770c0(0x14b)) selection = 3;
    if (Func_080770c0(0x14c)) selection = 4;
    if (Func_080770c0(0x14d)) selection = 5;
    if (Func_080770c0(0x14e)) selection = 6;
    if (Func_080770c0(0x14f)) selection = 7;

    Func_08091200(((u32 *)0x080a0108)[selection], 1);
    Func_08091254(8);
}
