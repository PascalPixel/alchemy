#include "types.h"
#include "palbuf.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "io_write_queue.h"
#include "io_reg.h"

s32 GameFlag_Test(s32 flag);
typedef s32 (*CopyWordsFn)(void *destination, const void *source, s32 size);

/* FAKEMATCH: system/io_write_queue.c's queue idiom, for that file's reasons:
   the loop that runs once around the IME read, and the count stored through
   a u16 pointer before the entry is written. */
#define QUEUE_WRITE(value, address, control) {                             \
        u32 saved;                                                          \
        s32 count;                                                          \
                                                                            \
        do { \
            /* FAKEMATCH: removing this one-pass boundary changes measured instruction scheduling. */ \
            ime = &REG_IME;                                                 \
            saved = *ime;                                                   \
        } while (0);                                                        \
        *ime = (u16)ime;                                                    \
        count = q->count;                                                   \
        if (count <= 31) {                                                  \
            u32 *destination = q->entries[count];                            \
            *(u16 *)&q->count = count + 1;                                  \
            *destination++ = (value);                                       \
            *destination++ = (address);                                     \
            *destination = (control);                                       \
        }                                                                   \
        *ime = saved;                                                       \
    }

void BattlePalette_UpdateBlend(void)
{
    struct BattleEffectBuffers *work = Ram_HeapSlots->block_32;
    u16 *add = work->delta;
    volatile u16 *ime;
    struct IoWriteQueue *q;
    u8 *base;
    s32 i;

    if (GameFlag_Test(0x152) != 0) {
        return;
    }
    if (work->duration == 0) {
        return;
    }
    if (++work->step < work->duration) {
        u16 *sum = work->current;
        for (i = 0; i < 0x540; i++) {
            *sum++ += *add++;
        }
    } else {
        CopyWordsFn copy = Iwram_CopyWords;

        copy(work->current, work->target, sizeof(work->current));
        work->duration = 0;
    }

    {
        u16 *packed = work->packed[1 ^ work->page];
        s32 blue = 0x7c00;
        u16 *current;

        i = 0x1c0;
        current = work->current;
        for (; i != 0; i--) {
            *packed++ = (current[0] & blue) | (((s16)current[1] >> 5) & 0x3e0) | (((s16)current[2] >> 10) & 0x1f);
            current += 3;
        }
    }

    work->page ^= 1;
    base = (u8 *)work + work->page * sizeof(work->packed[0]);
    q = &gIoWriteQueue;
    {
        u32 bg = (u32)(base + 0x2300);
        QUEUE_WRITE(bg, 0x05000000, 0x84000070);
    }
    QUEUE_WRITE((u32)(base + 0x24c0), 0x05000200, 0x84000070);
}
