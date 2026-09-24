/* Title palette fade: step the 8.8 colour channels toward the target, pack
   them into the back palette buffer and queue both banks for the next frame.

   FAKEMATCH: each queued write is QueueIoWriteDelay-style inline code with
   function-level queue and IME pointers and the one-pass loop around the
   IME read (as in SYSTEM/IO_WRITE_QUEUE.C); the front bank address passes
   through a block local, and the packing loop counts down from an explicit
   512 set before the source pointer. */
#include "TYPES.H"
#include "DMA.H"
#include "IO_WRITE_QUEUE.H"

extern u8 gWorkSlot[];
extern volatile u16 Data_04000208;
#define REG_IME Data_04000208

#define QUEUE_PALETTE(source, destination) {                                \
        u32 saved;                                                          \
                                                                            \
        q = &gIoWriteQueue;                                                 \
        do {                                                                \
            ime = &REG_IME;                                                 \
            saved = *ime;                                                   \
        } while (0);                                                        \
        *ime = (u16)ime;                                                    \
        count = q->count;                                                   \
        if (count <= 31) {                                                  \
            u32 *entry = (u32 *)((u8 *)q + count * 12 + 4);                 \
            *(u16 *)&q->count = count + 1;                                  \
            *entry++ = (u32)(source);                                       \
            *entry++ = (destination);                                       \
            *entry = 0x84000080;                                            \
        }                                                                   \
        *ime = saved;                                                       \
    }

void TitlePalette_UpdateFade(void)
{
    u8 *buffer = *(u8 **)(gWorkSlot + 32 * 4);
    u16 *delta = (u16 *)(buffer + 0x1c00);
    u16 *current;
    u16 *packed;
    s32 i;
    u8 *bank;
    s32 blue;
    volatile u16 *ime;
    struct IoWriteQueue *q;
    s32 count;

    if (*(s8 *)(buffer + 0x3001) == 0)
        return;
    if ((s8)++*(u8 *)(buffer + 0x3002) < *(s8 *)(buffer + 0x3001)) {
        current = (u16 *)(buffer + 0x400);
        for (i = 0; i <= 0x5ff; i++)
            *current++ += *delta++;
    } else {
        Dma_Set(buffer + 0x1000, buffer + 0x400, 0x84000300, (volatile u32 *)0x040000d4);
        *(u8 *)(buffer + 0x3001) = 0;
    }
    packed = (u16 *)(buffer + ((*(u8 *)(buffer + 0x3000) ^ 1) << 10) + 0x2800);
    blue = 0x7c00;
    i = 512;
    current = (u16 *)(buffer + 0x400);
    for (; i != 0; i--) {
        *packed++ = (current[0] & blue) | (((s16)current[1] >> 5) & 0x3e0) | (((s16)current[2] >> 10) & 0x1f);
        current += 3;
    }
    *(u8 *)(buffer + 0x3000) ^= 1;
    bank = buffer + (*(u8 *)(buffer + 0x3000) << 10);
    {
        u8 *front = bank + 0x2800;

        QUEUE_PALETTE(front, 0x05000000);
    }
    QUEUE_PALETTE(bank + 0x2a00, 0x05000200);
}
