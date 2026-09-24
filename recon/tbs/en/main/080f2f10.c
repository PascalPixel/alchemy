/* Draft, not exact (2026-09-24): 34 differing halfwords, same length.
   Hand-written from the assembly beside TitlePalette_InitializeBuffers. The
   queued palette transfers are QueueIoWriteDelay-style inline writes of DMA
   triples. Residual: register choice between the queue, IME pointer and the
   saved IME values across the two queued writes, and the order of the
   bank-address constants. */
/* Title palette fade: step the 8.8 colour channels toward the target, pack
   them into the back palette buffer and queue both banks for the next frame. */
#include "TYPES.H"
#include "DMA.H"
#include "IO_WRITE_QUEUE.H"

extern u8 gWorkSlot[];
extern volatile u16 Data_04000208;
#define REG_IME Data_04000208

#define QUEUE_PALETTE(source, destination) {                                \
        volatile u16 *ime;                                                  \
        struct IoWriteQueue *q;                                             \
        u32 saved;                                                          \
        s32 count;                                                          \
                                                                            \
        q = &gIoWriteQueue;                                                 \
        ime = &REG_IME;                                                     \
        saved = *ime;                                                       \
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
    current = (u16 *)(buffer + 0x400);
    for (i = 0; i < 512; i++) {
        *packed++ = (current[0] & blue) | (((s16)current[1] >> 5) & 0x3e0) | (((s16)current[2] >> 10) & 0x1f);
        current += 3;
    }
    *(u8 *)(buffer + 0x3000) ^= 1;
    bank = buffer + (*(u8 *)(buffer + 0x3000) << 10);
    packed = (u16 *)(bank + 0x2800);
    QUEUE_PALETTE(packed, 0x05000000);
    QUEUE_PALETTE(bank + 0x2a00, 0x05000200);
}
