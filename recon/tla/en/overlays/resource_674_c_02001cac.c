// Near miss, differ 2 of 34: the listing reads the DMA control (ldrh r4, [r3, #10]) before it starts the mask
// 0xc5ff; here the mask starts first. The read as its own barrier-held value turns the mask into a pool
// load (differ 11). The same shape as Func_080dcdc8 in battle/effect/hue.c.
#include "../../../../games/tla/src/field/overlay_674/ov674.h"
#include "dma.h"
#include "io_reg.h"

extern u8 Data_02004f04;
extern s16 Data_02004f10[2][160];

// 走査線ごとの BG1 横スクロールを HBlank DMA に掛ける
void Func_02001cac(void)
{
    s16 *lines = Data_02004f10[Data_02004f04];
    s32 first = lines[0];
    volatile u16 *scroll;
    volatile u16 *dma;

    /* FAKEMATCH: keeps the first line's load before the register address is built */
    __asm__("" : : "r"(first));
    scroll = (volatile u16 *)0x04000014;
    *scroll = first;
    /* FAKEMATCH: keeps the scroll store before the DMA address is built */
    __asm__("");
    dma = REG_DMA0;

    dma[5] &= 0xc5ff; // 開始タイミングとリピートを消す
    dma[5] &= 0x7fff; // 止める
    lines++;
    /* FAKEMATCH: keeps the cursor step before the last control read */
    __asm__("" : : "r"(lines));
    (void)dma[5];
    Dma_Set(lines, (void *)scroll, 0xa2600001, (volatile u32 *)dma);
}
