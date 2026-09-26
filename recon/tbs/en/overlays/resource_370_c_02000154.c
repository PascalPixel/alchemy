/* NONMATCHING: 168/168 bytes, 3 differing halfwords, 2 aligned edits.
 * Single-overlay unit binding Engine_* at their import veneers. Remaining:
 * allocation only. The reference keeps level in r5 and the first saved IME in
 * r1, and rematerialises the second IME pointer for its restore (ldr r3);
 * here CSE turns the second pointer into a copy of the first (pseudo 65, r5),
 * which takes r5 before level and saved. Symbol versus constant spellings of
 * the IME address did not separate them.
 * 2026-09-27 restore-boundary transfer from exact 371:020039fc: H1 wraps
 * each final IME restore in one pass. Result 168/168, 34 differing halfwords,
 * 12 aligned edits, binary-identical to the baseline. Queue body and pools
 * already match; the late address reload is still replaced by a saved copy.
 * Hypothesis rejected: restore scheduling alone does not change CSE lifetime.
 * H2: inline RestoreInterrupts owns the final hardware address. This recovers
 * the reference's r5 fade level, r1 first saved IME and late r3 address reload.
 * Full normalized diff now differs only in the first queue address load:
 * reference loads q before the level's two shifts; candidate loads it after.
 * All later instructions, callback removal, and the complete pool match. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "IO_WRITE_QUEUE.H"

extern volatile u16 Data_04000208;
extern u16 Data_020096b0;

void Func_02000154(void);

static __inline__ void RestoreInterrupts(u32 saved)
{
    /* FAKEMATCH: keep the final hardware address local to restoration. */
    do { Data_04000208 = saved; } while (0);
}

/* Queue a register write with interrupts masked; the value is evaluated only
 * when the queue has room.
 * FAKEMATCH: one-pass restoration separates queue publication from the
 * following callback-removal decision, as in WORLD_MAP/DISPLAY_TRANSITION.C. */
#define QUEUE_WRITE(address, value)                                         \
    do {                                                                    \
        volatile u16 *ime;                                                  \
        struct IoWriteQueue *q;                                             \
        u32 saved;                                                          \
        s32 count;                                                          \
                                                                            \
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

/* Fade the blend in step by step each frame; remove itself once full. */
void Func_02000154(void)
{
    s32 level;

    level = (u16)++Data_020096b0 >> 1;
    QUEUE_WRITE(0x4000050, 0x2e51);
    QUEUE_WRITE(0x4000052, ((16 - (u16)level) << 8) | (u16)level);
    if ((u16)level > 15)
        Engine_TaskRemoveCallback(Func_02000154);
}
