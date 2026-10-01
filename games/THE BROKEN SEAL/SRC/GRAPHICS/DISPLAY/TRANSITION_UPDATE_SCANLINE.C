#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "RAM_BUFFER.H"
#include "IWRAM_CALL.H"
#include "IO_WRITE_QUEUE.H"
#include "IO_REG.H"
extern u8 gCam[];

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void DisplayTransition_UpdateScanline(void)
{
    u32 line;
    u8 *state;
    u16 value;

    line = *(volatile u16 *)0x04000006;
    state = *(u8 **)((u32)&gCam);

again:
    switch (FIELD(state, u16, 0x108)) {
    case 3:
        if (line >= FIELD(state, u16, 0x104)) {
            value = *(volatile u16 *)0x04000000;
            *(volatile u16 *)0x04000000 = (value & 0xFFF8) | 2;
            value = 9;
            FIELD(state, u16, 0x108) = value;
        }
        break;
    case 2:
        if (line >= FIELD(state, u16, 0x106)) {
            value = *(volatile u16 *)0x04000000;
            *(volatile u16 *)0x04000000 = value & 0xFFF8;
            value = 9;
            FIELD(state, u16, 0x108) = value;
        }
        break;
    case 1:
        if (line >= FIELD(state, u16, 0x104)) {
            value = *(volatile u16 *)0x04000000;
            *(volatile u16 *)0x04000000 = (value & 0xFFF8) | 2;
            FIELD(state, u16, 0x108)++;
            goto again;
        }
        if (line >= FIELD(state, u16, 0x106)) {
            value = *(volatile u16 *)0x04000000;
            *(volatile u16 *)0x04000000 = value & 0xFFF8;
            value = 3;
            FIELD(state, u16, 0x108) = value;
            goto again;
        }
        break;
    case 0:
        if (line <= 158) {
            value = 1;
            FIELD(state, u16, 0x108) = value;
            goto again;
        }
        break;
    }
}

/* Display transition frame callback: step the transition value towards its
 * end over the configured duration (or stop the H-blank DMA and remove the
 * callback when done), toggle the dither phase, write two palette nibbles
 * from the 64-entry dither table and queue the palette transfer. The table
 * is read inside the loop so that loop.c hoists its address on its first
 * pass and the 63 mask on its second. */

s32 Scheduler_RemoveCallback(void *);
extern const u8 DisplayTransition_DitherTable[];

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

void DisplayTransition_UpdateFrame(void)
{
    struct DisplayTransitionState *state = *(struct DisplayTransitionState **)Ram_DisplayWork;
    s32 value;
    s32 blend;
    u32 i;

    if (state->transition_duration != 0) {
        if (state->transition_step >= state->transition_duration) {
            volatile u16 *dma0;
            state->transition_duration = 0;
            Scheduler_RemoveCallback((void *)DisplayTransition_UpdateFrame);
            dma0 = REG_DMA0;
            dma0[5] &= 0xc5ff;
            dma0[5] &= 0x7fff;
            (void)dma0[5];
            return;
        } else {
            s32 delta = state->transition_end - state->transition_start;
            state->transition_step++;
            state->transition_value = state->transition_start
                + Iwram_SignedDivide(delta * state->transition_step, state->transition_duration);
        }
    }

    value = state->transition_value - 1;
    state->dither_toggle ^= 1;
    blend = 0;
    if (value & 32)
        blend = 15;
    value = (value & 31) * 2;
    for (i = 0; i <= 1; i++) {
        u8 entry = DisplayTransition_DitherTable[value & 63];
        u8 *p = &state->palette_nibbles[entry >> 1];
        if (entry & 1)
            *p = (*p & 0x0f) | (blend << 4);
        else
            *p = (*p & 0xf0) | blend;
        value++;
    }

    {
        struct IoWriteQueue *queue = &gIoWriteQueue;
        volatile u16 *ime = &REG_IME;
        s32 savedIme;
        s32 count;
        /* FAKEMATCH: the one-pass loop keeps the saved IME copy ahead of the masking write, as in the other queue writers. */
        do {
            savedIme = *ime;
        } while (0);
        *ime = (u16)(u32)ime;
        if ((count = queue->count) <= 31) {
            u32 *destination = queue->entries[count];
            /* FAKEMATCH: storing the count through a plain u16 pointer keeps it out of the entry stores' alias set, so sched2 finishes the entry address first. */
            *(u16 *)&queue->count = count + 1;
            *destination++ = (u32)state->palette_nibbles;
            *destination++ = 0x06000000;
            *destination = 0x84000008;
        }
        *ime = savedIme;
    }
}
