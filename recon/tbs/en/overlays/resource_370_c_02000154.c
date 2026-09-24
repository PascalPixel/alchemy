/* NONMATCHING: 168 bytes, candidate 168, 34 differing halfwords (2026-09-24).
 * Single-overlay unit binding Engine_* at their import veneers. Remaining:
 * allocation only. The reference keeps level in r5 and the first saved IME in
 * r1, and rematerialises the second IME pointer for its restore (ldr r3);
 * here CSE turns the second pointer into a copy of the first (pseudo 65, r5),
 * which takes r5 before level and saved. Symbol versus constant spellings of
 * the IME address did not separate them. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "IO_WRITE_QUEUE.H"

extern volatile u16 Data_04000208;
extern u16 Data_020096b0;

void Local_02000154(void);

/* Queue a register write with interrupts masked; the value is evaluated only
 * when the queue has room. */
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
        *ime = saved;                                                       \
    } while (0)

/* Fade the blend in step by step each frame; remove itself once full. */
void Local_02000154(void)
{
    s32 level;

    level = (u16)++Data_020096b0 >> 1;
    QUEUE_WRITE(0x4000050, 0x2e51);
    QUEUE_WRITE(0x4000052, ((16 - (u16)level) << 8) | (u16)level);
    if ((u16)level > 15)
        Engine_TaskRemoveCallback(Local_02000154);
}
