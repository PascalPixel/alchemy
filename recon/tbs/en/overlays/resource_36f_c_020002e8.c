/* NONMATCHING canonical H2: 364/364 bytes, three differing halfwords.
 * Restored after rejected H3 at 27b227b0c; H2 source witness is 866d51e33.
 * All queue instructions, IME/counter roles and pools are admitted.
 *
 * Rejected H3 direct reset: 360/364 bytes, 161 differing halfwords /
 * 37 aligned edits (2026-09-27). Folding the single-use zero into its
 * destination fixes address r2 / zero r3 and the strh operand order, while
 * preserving H2's IME/counter/queue roles. It removes the HI temporary,
 * however: both loads are now ordinary SI producers and the early pool
 * moves to the tail. Reject full-owner admission. H2 at 866d51e33 is the
 * stronger canonical. Further work must retain destination-first expansion
 * and a real HI producer together; do not trade pool reach for local order.
 * Three structural models are complete; no function/alignment credit.
 *
 * Admitted H2 restoration ownership: 364/364 bytes, three differing
 * halfwords / aligned edits (2026-09-27). Exact MENU/CLEAR/BLEND_FADE.C's
 * local IME/save pair plus RestoreInterrupts produces the reference's r5
 * IME, r6 fade counter, r7 queue and all eleven pool words. First IME
 * pseudo 43 now spans 170 instructions with one set / 18 uses; the queue
 * remains function-owned. Complete queue, loop, calls and tail are exact.
 * Remaining: initial halfword reset swaps destination r2 and zero r3.
 * CSE shows the standalone zero user pseudo is created before the address;
 * folding that sole consumer into the halfword assignment is the next
 * isolated producer-boundary hypothesis, with all H2 bytes frozen.
 *
 * Rejected H1 single IME binding: 364/364 bytes, 28 differing halfwords
 * and 28 aligned edits (2026-09-27). WORLD_MAP/DISPLAY_TRANSITION.C binds
 * IME once before its queue sequence. Transferring that ownership changes
 * pseudo 35 from five sets / 33 uses / 3712-insn lifetime to one set /
 * 25 uses / 218-insn lifetime. The prediction fails: counter still wins r5,
 * IME remains r6, and initial IME/queue literal order reverses. Frame and
 * topology stay equal; all pool values survive but their first order does
 * not. Reject the model; the 23-halfword canonical is in parent history.
 * The allocator still ranks the 9-use / 54-insn counter before IME. This
 * is not fixed by removing repeated IME definitions alone.
 *
 * Previous canonical: 364 bytes, candidate 364, 23 halfwords (2026-09-24).
 * Single-overlay unit binding Engine_* at their import veneers. Remaining:
 * global allocation order only: the reference gives the IME pointer r5 and
 * the fade counter r6, here the counter outranks it (9 refs over 56 insns
 * against 33 over 3712) and takes r5; the first counter reset also swaps its
 * address and pool-zero registers. The count is block-local in the queue
 * macro and the IME pointer and queue are function-level, which fixed the
 * count and saved registers.
 * 2026-09-27 family transfer: exact WORLD_MAP/DISPLAY_TRANSITION.C now
 * scopes final IME restoration separately from subsequent publication/calls.
 * Apply that boundary to the same queued triple stores here, leaving the
 * existing read scope and IO_WRITE_QUEUE.H unchanged. Admission: IME r5,
 * fade counter r6, then complete 364-byte owner/pool equality. One trial.
 * Result: byte-identical to the prior candidate, 364/364 bytes and 23
 * differing halfwords/aligned edits. All eleven pool words are exact.
 * IME remains pseudo 35/r6 (33 uses, five sets, two crossed calls) and the
 * fade counter r5. Final-publication scoping does not change this allocation.
 * Stop this transfer axis; the exact world-map source was not edited. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "IO_WRITE_QUEUE.H"

extern volatile u16 Data_04000208;
extern u8 Value_00000000;
extern s16 Data_0200868c;

void Local_02000454(void);
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

void Func_020002e8(void)
{
    s32 i;
    s32 zero;
    u8 *event;
    struct IoWriteQueue *q;

    Local_02000454();
    Engine_EventWait(30);
    zero = (u16)(u32)&Value_00000000;
    Data_0200868c = zero;
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
