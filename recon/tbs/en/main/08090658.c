/* 2026-09-29 alchemy permute: score 930 to 610 (with the build's names) on the permuter's scorer (0
   is exact); remaining 8 register-only, 4 operand, 3 reordered, 3 deleted.
   Kept rewrites: 2x introduce a temporary, 1x swap commutative operands,
   1x reorder independent statements, 1x reorder local declarations, 1x
   remove a temporary, 1x split or join a compound assignment, 1x move an
   assignment into or out of a condition, 1x toggle register, 1x test truth
   or compare with zero. FAKEMATCH: the permuter's temporaries, register
   hints and swapped operand orders below only steer allocation and
   scheduling; no programmer would write them, so they stay tagged until a
   natural spelling replaces them. */
/* 2026-09-24: 37 differing halfwords (from 48) after a do-while wrap and
   statement-swap sweep; the do-while wraps are search artefacts. */
#include "TYPES.H"

extern u8 Data_00000539[];

extern s32 Scheduler_RemoveCallback(void *);
extern s32 _call_via_r3(s32, s32, s32, s32);

struct DisplayTransitionState {
    u8 pad_000[0x508];
    u8 palette_nibbles[0x22];
    u16 transition_value;
    u8 pad_52c[13];
    u8 dither_toggle;
    s8 transition_start;
    s8 transition_end;
    s8 transition_duration;
    s8 transition_step;
};

struct DisplayTransfer {
    u32 source;
    u32 destination;
    u32 control;
};

struct DisplayTransferQueue {
    u16 count;
    u16 pad;
    struct DisplayTransfer entries[32];
};

extern struct DisplayTransferQueue gIoWriteQueue;

void DisplayTransition_UpdateFrame(void)
{
    struct DisplayTransitionState *state = *(struct DisplayTransitionState **)0x03001ECC;
    s8 *duration = &state->transition_duration;
    u16 value;

    if (*duration != 0) {
        s8 *step = &state->transition_step;
        if (*step >= *duration) {
            volatile u16 *dma0;
            *duration = 0;
            Scheduler_RemoveCallback((void *)DisplayTransition_UpdateFrame);
            dma0 = (volatile u16 *)0x040000b0;
            dma0[5] = dma0[5] & 0xc5ff;
            dma0[5] = dma0[5] & 0x7fff;
            (void)dma0[5];
            return;
        } else {
            register s32 delta = state->transition_end - state->transition_start;
            s32 v;
            (*step)++;
            v = _call_via_r3(delta * *step, *duration, delta, 0x03000380);
            state->transition_value = state->transition_start + v;
        }
    }
    {
        u8 *toggle = (u8 *)state + (u32)Data_00000539;
        s32 v2;
        s32 blend;
        s32 masked;
        const u8 *table = (const u8 *)0x0809e8ee;
        s32 idx;
        u32 i;
        s32 tmp2;
        value = *&state->transition_value;
        v2 = value - 1;
        *toggle = *toggle ^ 1;
        tmp2 = 32 & v2;
        do {
            blend = 0;
        } while (0);
        if (tmp2 != 0) {
            blend = 15;
        }
        masked = v2 & 31;
        do {
            idx = masked << 1;
        } while (0 != 0);
        i = 0;
        do {
            u8 entry = table[idx & 63];
            u8 *p = &state->palette_nibbles[entry >> 1];
            if (entry & 1) {
                s32 tmp;
                tmp = *p & 0x0f;
                *p = tmp | (blend << 4);
            } else {
                *p = (*p & 0xf0) | blend;
            }
            i++;
            idx++;
        } while (i <= 1);
    }
    {
        struct DisplayTransferQueue *queue = &gIoWriteQueue;
        volatile u16 *ime = (volatile u16 *)0x04000208;
        s32 savedIme;
        s32 counter;
        do {
            savedIme = *ime;
        } while (0);
        *ime = (u16)(u32)ime;
        if ((counter = queue->count) <= 31) {
            struct DisplayTransfer *entry = &queue->entries[counter];
            u32 *destination = &entry->source;
            queue->count = counter + 1;
            *destination++ = (u32)state->palette_nibbles;
            *destination++ = 0x06000000;
            *destination = 0x84000008;
        }
        *ime = savedIme;
    }
}
