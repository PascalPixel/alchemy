/* Draft, not exact (2026-09-24): 380 of 380 bytes, 8 differing halfwords
   (was 189 at 372). Once a palette blend is running, add the step table
   into the working colours (or, on the last step, copy the targets in and
   stop), pack the working colours into the back palette buffer, flip
   buffers and queue both halves for the next frame. The green and blue
   masks are u16 values of link symbols (halfword pool constants, which
   place the pool before the pack loop), and the queue writes use the IO
   write queue idiom of SYSTEM/IO_WRITE_QUEUE.C with function-level queue
   and IME pointers. Residual: in the pack loop preheader the reference sets
   the count before the source pointer, and it computes the first queued
   address ahead of the IME pointer load and keeps the saved IME word in r1
   (here r7). */
/* 2026-09-29 (alchemy permute scorer): 120, two reordered instructions
   (was 250 with three unresolved names). The masks need no link symbols:
   written as plain 0x3e0 and 0x1f inside the packing expression, GCC
   narrows those ANDs to halfwords and loads both constants from the pool
   before the loop, exactly as the reference does. The IO write queue is now
   SYSTEM/IO_WRITE_QUEUE.C's own idiom (the IME pointer set inside the
   do-while, as there; it carries that file's FAKEMATCH reasons), the copy
   goes through a local Iwram_CopyWords pointer, and the work pointer is the
   linked Data_03001ed0. Remaining: in the pack loop preheader the reference
   sets the count (movs r0, #224 / lsls r0, #1) before the source pointer's
   lsls/adds; here the scheduler places the count after. Up-count, down-count
   (do-while, for, while), pointer walks, struct and flat indexing, statement
   and declaration orders were tried: the down-count spellings fix the count
   but then put the source pointer ahead of the hoisted 0x7c00 (180 to 210).
   Two searches (70,000 candidates) found nothing below 120. */
#include "TYPES.H"
#include "IO_WRITE_QUEUE.H"
#include "IO_REG.H"
#include "IWRAM_CALL.H"

s32 GameFlag_IsSet(s32 flag);

extern u8 *Data_03001ed0;

typedef s32 (*CopyWordsFn)(void *destination, const void *source, s32 size);

/* FAKEMATCH: SYSTEM/IO_WRITE_QUEUE.C's queue idiom, for that file's reasons:
   the loop that runs once around the IME read, and the count stored through
   a u16 pointer before the entry is written. */
#define QUEUE_WRITE(value, address, control) {                             \
        u32 saved;                                                          \
        s32 count;                                                          \
                                                                            \
        do {                                                                \
            ime = &REG_IME;                                                 \
            saved = *ime;                                                   \
        } while (0);                                                        \
        *ime = (u16)ime;                                                    \
        count = q->count;                                                   \
        if (count <= 31) {                                                  \
            u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);           \
            *(u16 *)&q->count = count + 1;                                  \
            *destination++ = (value);                                       \
            *destination++ = (address);                                     \
            *destination = (control);                                       \
        }                                                                   \
        *ime = saved;                                                       \
    }

void Func_080908e0(void)
{
    u8 *p = Data_03001ed0;
    u16 *add = (u16 *)(p + 0x1880);
    volatile u16 *ime;
    struct IoWriteQueue *q;
    u8 *base;
    s32 i;

    if (GameFlag_IsSet(0x152) != 0) {
        return;
    }
    if (*(s8 *)(p + 0x2a01) == 0) {
        return;
    }
    if (++*(s8 *)(p + 0x2a02) < *(s8 *)(p + 0x2a01)) {
        u16 *sum = (u16 *)(p + 0x380);
        for (i = 0; i < 0x540; i++) {
            *sum++ += *add++;
        }
    } else {
        CopyWordsFn copy = Iwram_CopyWords;

        copy(p + 0x380, p + 0xe00, 0xa80);
        *(s8 *)(p + 0x2a01) = 0;
    }

    {
        s16 (*in)[3] = (s16 (*)[3])(p + 0x380);
        u16 *out = (u16 *)(p + (1 ^ p[0x2a00]) * 0x380 + 0x2300);

        for (i = 0; i < 0x1c0; i++) {
            *out++ = (in[i][0] & 0x7c00) | ((in[i][1] >> 5) & 0x3e0) | ((in[i][2] >> 10) & 0x1f);
        }
    }

    p[0x2a00] ^= 1;
    base = p + p[0x2a00] * 0x380;
    q = &gIoWriteQueue;
    {
        u32 bg = (u32)(base + 0x2300);
        QUEUE_WRITE(bg, 0x05000000, 0x84000070);
    }
    QUEUE_WRITE((u32)(base + 0x24c0), 0x05000200, 0x84000070);
}
