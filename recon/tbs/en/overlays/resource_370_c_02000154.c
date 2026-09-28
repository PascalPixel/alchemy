/* Exact 168-byte owner, resource_370:02000154..020001fc, including pool.
 * Own-ROM reconstruction; 2026-09-27 initial model plus two follow-ups:
 * H1: one-pass final restore transferred from exact 371:020039fc. The
 * candidate was binary-identical to baseline: 34 differing halfwords,
 * 12 aligned edits. Scheduling alone did not change CSE lifetime.
 * H2: inline RestoreInterrupts owns its hardware address. This recovered
 * r5 fade level, r1 first saved IME and late r3 reload: 3 halfwords/2 edits.
 * H3: publish the incremented frame, initialize one persistent queue pointer,
 * then narrow the level. The queue load precedes both shifts: 0 differences.
 * All three hypotheses preserved in history; no register spelling sweep. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "IO_WRITE_QUEUE.H"

extern volatile u16 RegIme;
extern u16 Data_020096b0;

void Clear_UpdateBlend(void);

static __inline__ void RestoreInterrupts(u32 saved)
{
    /* FAKEMATCH: keep the final hardware address local to restoration. */
    do { RegIme = saved; } while (0);
}

/* Queue a register write with interrupts masked; the value is evaluated only
 * when the queue has room.
 * FAKEMATCH: one-pass restoration separates queue publication from the
 * following callback-removal decision, as in WORLD_MAP/DISPLAY_TRANSITION.C. */
#define QUEUE_WRITE(address, value)                                         \
    do {                                                                    \
        volatile u16 *ime;                                                  \
        u32 saved;                                                          \
        s32 count;                                                          \
                                                                            \
        do {                                                                \
            ime = &RegIme;                                           \
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

    frame = Data_020096b0 + 1;
    Data_020096b0 = frame;
    q = &gIoWriteQueue;
    level = (u16)frame >> 1;
    QUEUE_WRITE(0x4000050, 0x2e51);
    QUEUE_WRITE(0x4000052, ((16 - (u16)level) << 8) | (u16)level);
    if ((u16)level > 15)
        Engine_TaskRemoveCallback(Clear_UpdateBlend);
}
