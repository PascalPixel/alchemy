#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "IO_WRITE_QUEUE.H"
#include "IO_REG.H"

extern u16 Clear_BlendFrame;

void Clear_UpdateBlend(void);

static __inline__ void RestoreInterrupts(u32 saved)
{
    /* FAKEMATCH: keep the final hardware address local to restoration. */
    do { REG_IME = saved; } while (0);
}

/* FAKEMATCH: the one-pass IME read keeps the saved copy before masking;
 * the count cast preserves the queue's original publication order. */
#define QUEUE_WRITE(address, value)                                         \
    do {                                                                    \
        volatile u16 *ime;                                                  \
        u32 saved;                                                          \
        s32 count;                                                          \
                                                                            \
        do {                                                                \
            ime = &REG_IME;                                           \
            saved = *ime;                                                   \
        } while (0);                                                        \
        *ime = (u16)ime;                                                    \
        count = q->count;                                                   \
        if (count <= 31) {                                                  \
            u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);           \
            *(u16 *)&q->count = count + 1;                                  \
            *destination++ = (value);                                       \
            *destination++ = (address);                                     \
            *destination = 0x20000;                                         \
        }                                                                   \
        RestoreInterrupts(saved);                                          \
    } while (0)

/* Fade the blend in step by step each frame; remove itself once full. */
void Clear_UpdateBlend(void)
{
    struct IoWriteQueue *q;
    s32 frame;
    s32 level;

    frame = Clear_BlendFrame + 1;
    Clear_BlendFrame = frame;
    q = &gIoWriteQueue;
    level = (u16)frame >> 1;
    QUEUE_WRITE(0x4000050, 0x2e51);
    QUEUE_WRITE(0x4000052, ((16 - (u16)level) << 8) | (u16)level);
    if ((u16)level > 15)
        Engine_TaskRemoveCallback(Clear_UpdateBlend);
}
