/* Near miss: differ 10 of 166, The Broken Seal's PaletteFade_Update on heap slot 0x80 (block32), ported fresh. Every remaining difference is a scheduling swap where the native issues a load before a split constant; a block temporary for the first queued write's address, found by a greedy search over one-pass blocks, barriers and block temporaries, fixes two of the five (differ 14 without it). */
#include "types.h"
#include "io_reg.h"
#include "dma.h"
#include "io_write_queue.h"
#include "palbuf.h"
#include "ram_buffer.h"

void PaletteFade_Update(void)
{
    struct PaletteFade *work = Ram_HeapSlots->block32;
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

        IO_WRITE_QUEUE_PUSH(({ s32 t; do { t = (s32)(0x05000000); } while (0); t; }), (u32)front, 0x84000080);
    }
    IO_WRITE_QUEUE_PUSH(0x05000200, (u32)(bank + 256), 0x84000080);
}
