#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "IO_WRITE_QUEUE.H"

extern volatile u16 Data_04000208;
extern s16 Data_0200868c;

void Title_LoadBackground(void);
void Title_Func020001c0(s32 mode);
void Title_RevealSpriteRow(void);

static __inline__ void RestoreInterrupts(u32 saved)
{
    /* FAKEMATCH: BLEND_FADE.C keeps this address local to restoration. */
    do { Data_04000208 = saved; } while (0);
}

/* Queue a register write with interrupts masked.
 * FAKEMATCH: the final one-pass restore retains the queue-publication
 * boundary used by the exact world-map transfer family. */
#define QUEUE_WRITE(address, value)                                         \
    do {                                                                    \
        volatile u16 *ime;                                                  \
        u32 saved;                                                          \
        s32 count;                                                          \
        q = &gIoWriteQueue;                                                 \
        do {                                                                \
            ime = &Data_04000208;                                           \
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

/* FAKEMATCH: expand the destination first while the one-halfword record
 * retains the short-range pool-zero producer used by PALETTE_START.C. */
static __inline__ void ResetCounter(s16 *destination)
{
    struct { u16 value; } zero;

    zero.value = 0;
    *destination = zero.value;
}

void Title_RevealScreen(void)
{
    s32 i;
    u8 *event;
    struct IoWriteQueue *q;

    Title_LoadBackground();
    Engine_EventWait(30);
    ResetCounter(&Data_0200868c);
    Title_Func020001c0(0);
    Engine_TaskAddCallback(Title_RevealSpriteRow, 0xc80);
    QUEUE_WRITE(0x4000000, 0x1540);
    QUEUE_WRITE(0x4000050, 0x2fce);
    QUEUE_WRITE(0x4000054, 16);
    QUEUE_WRITE(0x4000052, 0x1010);
    Engine_EventWait(120);
    for (i = 0; i <= 16; i++) {
        QUEUE_WRITE(0x4000054, 16 - i);
        Engine_TaskWait(3);
    }
    event = *(u8 **)0x03001ebc;
    *(s32 *)(event + 0x1c0) = 0;
    *(s32 *)(event + 0x1c8) = 1;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    *(s32 *)(*(u8 **)0x03001ebc + 0x1c8) = 60;
}
