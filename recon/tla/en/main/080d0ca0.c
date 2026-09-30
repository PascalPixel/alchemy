/* Battle palette blend, run each frame by the scheduler once
   BattleEffect_InitializeBuffers has set it up: add the step table into the
   working colours (on the last step, copy the targets in and stop), pack them
   into the back palette buffer, flip buffers and queue both halves.
   The packing loop follows GRAPHICS/PALETTE/TITLE_UPDATE_FADE.C. */
#include "TYPES.H"
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

void BattlePalette_UpdateBlend(void)
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
        u16 *packed = (u16 *)(p + (1 ^ p[0x2a00]) * 0x380 + 0x2300);
        s32 blue = 0x7c00;
        u16 *current;

        i = 0x1c0;
        current = (u16 *)(p + 0x380);
        for (; i != 0; i--) {
            *packed++ = (current[0] & blue) | (((s16)current[1] >> 5) & 0x3e0) | (((s16)current[2] >> 10) & 0x1f);
            current += 3;
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
