#include "types.h"

extern u8 Data_00000539[];

extern s32 Func_08004278(void *);
extern s32 Func_080072f0(s32, s32, s32, s32);

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

extern struct DisplayTransferQueue Data_02002090;

void Func_08090658(void)
{
    struct DisplayTransitionState *state =
        *(struct DisplayTransitionState **)0x03001ECC;
    s8 *duration = &state->transition_duration;
    u16 value;

    if (*duration != 0) {
        s8 *step = &state->transition_step;

        if (*step >= *duration) {
            volatile u16 *dma0;

            *duration = 0;
            Func_08004278((void *)Func_08090658);
            dma0 = (volatile u16 *)0x040000b0;
            dma0[5] = dma0[5] & 0xc5ff;
            dma0[5] = dma0[5] & 0x7fff;
            (void)dma0[5];
            return;
        } else {
            s32 delta = state->transition_end - state->transition_start;
            s32 v;

            (*step)++;
            v = Func_080072f0(delta * *step, *duration, delta, 0x03000380);
            state->transition_value = state->transition_start + v;
        }
    }

    {
        u16 *valuePtr = &state->transition_value;
        u8 *toggle = (u8 *)state + (u32)Data_00000539;
        s32 v2;
        s32 blend;
        s32 masked;
        s32 idx;
        const u8 *table = (const u8 *)0x0809e8ee;
        u32 i;

        value = *valuePtr;
        v2 = value - 1;
        *toggle ^= 1;

        blend = 0;
        if ((v2 & 32) != 0) {
            blend = 15;
        }
        masked = v2 & 31;
        idx = masked << 1;

        i = 0;
        do {
            u8 entry = table[idx & 63];
            u8 *p = &state->palette_nibbles[entry >> 1];

            if (entry & 1) {
                *p = (*p & 0x0f) | (blend << 4);
            } else {
                *p = (*p & 0xf0) | blend;
            }
            i++;
            idx++;
        } while (i <= 1);
    }

    {
        struct DisplayTransferQueue *queue = &Data_02002090;
        volatile u16 *ime = (volatile u16 *)0x04000208;
        s32 savedIme;
        s32 counter;

        savedIme = *ime;
        *ime = (u16)(u32)ime;
        counter = queue->count;
        if (counter <= 31) {
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
