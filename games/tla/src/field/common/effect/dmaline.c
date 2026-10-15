#include "types.h"
#include "callback_scheduler.h"
#include "io_reg.h"
#include "mem.h"
#include "dma.h"
#include "ram_buffer.h"

struct DmaFxPage {
    u32 lines[0x1e0];
};

struct DmaFxPages {
    struct DmaFxPage pages[2];
    u8 page;
};

// 毎フレーム: 今のページの先頭を書き、残りを HBlank DMA に任せる
void Func_080d5ff8(void)
{
    struct DmaFxPages *work = Ram_HeapSlots->unk84[1];
    u32 *line = work->pages[work->page].lines;
    volatile u32 *dest = (volatile u32 *)0x04000014;
    u8 *event = Ram_HeapSlots->eventWork;
    volatile u32 *target;

    if (event != NULL) {
        if (*(s8 *)(event + 0xca4) != 0)
            return;
        if (*(s16 *)(event + 0x164) != 0)
            return;
    }
    {
        volatile u16 *dma = REG_DMA0;
        u16 control;

        /* FAKEMATCH: the one-pass block reads the control word before its mask is built (one reordered pair, differ 2 without it), as in Func_080d64b8. */
        do {
            control = dma[5];
        } while (0);
        dma[5] = control & 0xc5ff; // 開始タイミングとリピートを消す
        dma[5] &= 0x7fff; // 止める
        target = dest;
        /* FAKEMATCH: the barrier copies the transfer destination before the last control read, as the native does; plain, the copy follows the first line word (one reordered pair, differ 2). */
        asm volatile("" : "+l"(target));
        (void)dma[5];
    }
    *dest = *line++;
    *dest = *line++;
    *dest = *line++;
    Dma_Set(line, (void *)target, 0xa6600003, REG_DMA0);
}
