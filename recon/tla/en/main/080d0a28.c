/* Near miss, differ 4 of 158 (ported from The Broken Seal's
   DisplayTransition_UpdateFrame, with two tagged one-pass blocks): after
   the join the native starts the toggle's offset (movs r2, #160), loads the
   value, then finishes the offset; this draft finishes the offset before the
   load. sched2 ranks the value's load 8 and the offset's shift 9; the native
   ranks the load at least as high. A block after the value read, a temporary
   for the raw value, the toggle written first or through a pointer all differ
   more; permute (3 min) found nothing. */
#include "types.h"
#include "callback_scheduler.h"
#include "disptran.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "iwram_call.h"
#include "ram_buffer.h"

extern const u8 Data_080f0206[];

void DisplayTransition_UpdateFrame(void)
{
    struct DisplayTransitionFrame *state;
    s32 value;
    s32 blend;
    u32 i;

    /* FAKEMATCH: the one-pass block loads the transition state before the duration's offset is built (one reordered row, differ 2 without it). */
    do {
        state = (struct DisplayTransitionFrame *)Ram_HeapSlots->display_transition;
    } while (0);
    if (state->duration != 0) {
        if (state->step >= state->duration) {
            volatile u16 *dma0;
            u16 control;

            state->duration = 0;
            Scheduler_RemoveCallback(DisplayTransition_UpdateFrame);
            dma0 = REG_DMA0;
            /* FAKEMATCH: the one-pass block reads the DMA control before the first mask is built (one reordered row, differ 2 without it). */
            do {
                control = dma0[5];
            } while (0);
            dma0[5] = control & 0xc5ff;
            dma0[5] &= 0x7fff;
            (void)dma0[5];
            return;
        } else {
            s32 delta = state->end - state->start;

            state->step++;
            state->value = state->start + Iwram_SignedDivide(delta * state->step, state->duration);
        }
    }

    value = state->value - 1;
    state->dither_toggle ^= 1;
    blend = 0;
    if (value & 32)
        blend = 15;
    value = (value & 31) * 2;
    for (i = 0; i <= 1; i++) {
        u8 entry = Data_080f0206[value & 63];
        u8 *p = &state->palette_nibbles[entry >> 1];

        if (entry & 1)
            *p = (*p & 0x0f) | (blend << 4);
        else
            *p = (*p & 0xf0) | blend;
        value++;
    }

    IO_WRITE_QUEUE_PUSH((u32)BG_CHAR_BLOCK(0), (u32)state->palette_nibbles, 0x84000008);
}
