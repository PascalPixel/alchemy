#include "types.h"
#include "ram_buffer.h"
#include "dma.h"

struct AffineStepWork {
    u8 unknown_0000[0x7790];
    s32 counter;
    s32 period;
    s32 step_x;
    s32 step_y;
    u8 unknown_77a0[0x30];
    s32 x;
    s32 y;
};

void Func_0813bb38(void)
{
    struct AffineStepWork *work;

    /* FAKEMATCH: the one-pass block loads the effect work before the counter offset is started (one reordered pair, differ 2 without it). */
    do {
        work = (struct AffineStepWork *)Ram_HeapSlots->battle_fx_work[0];
    } while (0);

    if (++work->counter == work->period) {
        /* FAKEMATCH: the native loads x before it starts the affine reference register's address; the plain store starts the address first (one reordered pair, differ 2). The value is pinned to the r3 it already takes: in a plain local the two registers trade places (differ 16). */
        register s32 x __asm__("r3") = work->x;

        /* FAKEMATCH: the one-pass block's loop note holds the address until the pinned value is loaded. */
        do { } while (0);
        *(volatile s32 *)0x04000028 = x;
        *(volatile s32 *)0x0400002c = work->y;
        work->x += work->step_x;
        work->y += work->step_y;
        work->counter = 0;
    }
}

void Func_0813bba0(void)
{
    volatile u16 *dma = (volatile u16 *)0x040000b0;
    u16 control = dma[5];

    asm volatile(""); /* FAKEMATCH: ⚓️ reads the control word before it starts the mask; written as dma[5] &= 0xc5ff the mask is started first (differ 2) */
    dma[5] = control & 0xc5ff; // 開始タイミングとリピートを消す
    dma[5] &= 0x7fff; // 止める
    asm volatile(""); /* FAKEMATCH: ⚓️ stores the stopped control word before it starts the destination address (differ 2 without) */
    (void)dma[5];
    Dma_Set(Ram_MapCellBuffer, (void *)0x04000040, 0xa2600001, (volatile u32 *)dma);
}
