/* Near miss: differ 14 of 166, The Broken Seal's TitlePalette_UpdateFade on heap slot 0x80 (block_32), ported fresh. Every remaining difference is a scheduling swap where the native issues a load before a split constant (five spots); the greedy device search found nothing. */
#include "types.h"
#include "io_reg.h"
#include "dma.h"
#include "io_write_queue.h"
#include "palbuf.h"
#include "ram_buffer.h"

void TitlePalette_UpdateFade(void)
{
    struct TitlePaletteWork *work = Ram_HeapSlots->block_32;
    u16 *delta = work->delta;
    u16 *current;
    u16 *packed;
    s32 i;
    u16 *bank;
    s32 blue;

    if (work->duration == 0)
        return;
    if ((s8)++work->step < work->duration) {
        current = work->current;
        for (i = 0; i <= 0x5ff; i++)
            *current++ += *delta++;
    } else {
        Dma_Set(work->target, work->current, DMA_ENABLE32 | DMA_32BIT | 0x300, REG_DMA3);
        work->duration = 0;
    }
    packed = work->packed[work->page ^ 1];
    blue = 0x7c00;
    i = 512;
    current = work->current;
    for (; i != 0; i--) {
        *packed++ = (current[0] & blue) | (((s16)current[1] >> 5) & 0x3e0)
                    | (((s16)current[2] >> 10) & 0x1f);
        current += 3;
    }
    work->page ^= 1;
    bank = work->packed[work->page];
    {
        u16 *front = bank;

        IO_WRITE_QUEUE_PUSH(0x05000000, (u32)front, 0x84000080);
    }
    IO_WRITE_QUEUE_PUSH(0x05000200, (u32)(bank + 256), 0x84000080);
}
