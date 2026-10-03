#include "PALBUF.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "DMA.H"
#include "IO_WRITE_QUEUE.H"
#include "IO_REG.H"
#include "HEAP_STATE.H"


#define QUEUE_PALETTE(source, destination) {                                \
        u32 saved;                                                          \
                                                                            \
        q = &gIoWriteQueue;                                                 \
        do { \
            /* FAKEMATCH: removing this one-pass boundary changes measured instruction scheduling. */ \
            ime = &REG_IME;                                                 \
            saved = *ime;                                                   \
        } while (0);                                                        \
        *ime = (u16)ime;                                                    \
        count = q->count;                                                   \
        if (count <= 31) {                                                  \
            u32 *entry = q->entries[count];                                  \
            *(u16 *)&q->count = count + 1;                                  \
            *entry++ = (u32)(source);                                       \
            *entry++ = (destination);                                       \
            *entry = 0x84000080;                                            \
        }                                                                   \
        *ime = saved;                                                       \
    }

void Graphics_InterpolatePaletteBuffers(s16 *source, s16 *target, s16 *step, s32 frames)
{
    s32 index;
    s32 first;
    s32 second;
    s32 (*divide)(s32, s32);

    if (frames > 0) {
        divide = Iwram_SignedDivide;
        index = 0x5FF;
        do {
            first = *source;
            second = *target;
            *step = divide(second - first, frames);
            index--;
            source++;
            target++;
            step++;
        } while (index >= 0);
    }
}

/* Step the title palette channels toward the target and queue both banks. */
void TitlePalette_UpdateFade(void)
{
    /* FAKEMATCH: retain the existing one-pass IME scopes, halfword queue
       count stores, front-bank block and packing-loop pointer lifetime.
       The existing byte-slot read also avoids the extra add#128 emitted
       for the typed heap array in all six editions. */
    struct TitlePaletteWork *work = *(struct TitlePaletteWork **)((u8 *)gWorkSlot + 32 * sizeof(void *));
    u16 *delta = work->delta;
    u16 *current;
    u16 *packed;
    s32 i;
    u16 *bank;
    s32 blue;
    volatile u16 *ime;
    struct IoWriteQueue *q;
    s32 count;

    if (work->duration == 0)
        return;
    if ((s8)++work->step < work->duration) {
        current = work->current;
        for (i = 0; i <= 0x5ff; i++)
            *current++ += *delta++;
    } else {
        Dma_Set(work->target, work->current, 0x84000300, (volatile u32 *)0x040000d4);
        work->duration = 0;
    }
    packed = work->packed[work->page ^ 1];
    blue = 0x7c00;
    i = 512;
    current = work->current;
    for (; i != 0; i--) {
        *packed++ = (current[0] & blue) | (((s16)current[1] >> 5) & 0x3e0) | (((s16)current[2] >> 10) & 0x1f);
        current += 3;
    }
    work->page ^= 1;
    bank = work->packed[work->page];
    {
        u16 *front = bank;

        QUEUE_PALETTE(front, 0x05000000);
    }
    QUEUE_PALETTE(bank + 256, 0x05000200);
}
