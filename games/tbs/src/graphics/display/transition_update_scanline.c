#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "callback_scheduler.h"
#include "global_cells.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "io_write_queue.h"
#include "io_reg.h"
#include "disptran.h"

void DisplayTransition_UpdateScanline(void)
{
    u32 line;
    struct DisplayTransitionWindow *state;
    u16 value;

    line = REG_VCOUNT;
    state = gMapWork[0];

again:
    switch (state->irq_phase) {
    case 3:
        if (line >= state->irq_first_line) {
            value = REG_DISPCNT;
            REG_DISPCNT = (value & 0xFFF8) | 2;
            value = 9;
            state->irq_phase = value;
        }
        break;
    case 2:
        if (line >= state->irq_second_line) {
            value = REG_DISPCNT;
            REG_DISPCNT = value & 0xFFF8;
            value = 9;
            state->irq_phase = value;
        }
        break;
    case 1:
        if (line >= state->irq_first_line) {
            value = REG_DISPCNT;
            REG_DISPCNT = (value & 0xFFF8) | 2;
            state->irq_phase++;
            goto again;
        }
        if (line >= state->irq_second_line) {
            value = REG_DISPCNT;
            REG_DISPCNT = value & 0xFFF8;
            value = 3;
            state->irq_phase = value;
            goto again;
        }
        break;
    case 0:
        if (line <= 158) {
            value = 1;
            state->irq_phase = value;
            goto again;
        }
        break;
    }
}


void DisplayTransition_UpdateFrame(void)
{
    struct DisplayTransitionFrame *state = *Ram_DisplayWork;
    s32 value;
    s32 blend;
    u32 i;

    if (state->duration != 0) {
        if (state->step >= state->duration) {
            volatile u16 *dma0;
            state->duration = 0;
            Scheduler_RemoveCallback(DisplayTransition_UpdateFrame);
            dma0 = REG_DMA0;
            dma0[5] &= 0xc5ff;
            dma0[5] &= 0x7fff;
            (void)dma0[5];
            return;
        } else {
            s32 delta = state->end - state->start;
            state->step++;
            state->value = state->start
                + Iwram_SignedDivide(delta * state->step, state->duration);
        }
    }

    value = state->value - 1;
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

    IO_WRITE_QUEUE_PUSH((u32)BG_CHAR_BLOCK(0), (u32)state->palette_nibbles, 0x84000008);
}
