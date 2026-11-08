#include "types.h"
#include "task.h"
#include "display_transition.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "iwram_call.h"
#include "ram_buffer.h"

extern const u8 gData_080f0206[];

/// The Broken Seal's ScreenTransition_UpdateDissolve: steps the transition's
/// value toward its end each frame, turning DMA0 off when it arrives, then
/// sets the two pixel nibbles the value selects and queues the tile to
/// character block 0.
void ScreenTransition_UpdateDissolve(void) {
  struct ScreenTransitionDissolveState *state;
  s32 value;
  s32 blend;
  u32 i;

  state = (struct ScreenTransitionDissolveState *)Ram_HeapSlots->screenTransition;
  if (state->duration != 0) {
    if (state->step >= state->duration) {
      volatile u16 *dma0;
      u16 control;

      state->duration = 0;
      Task_Remove(ScreenTransition_UpdateDissolve);
      dma0 = REG_DMA0;
      control = dma0[5];
      dma0[5] = control & 0xc5ff;
      dma0[5] &= 0x7fff;
      (void)dma0[5];
      return;
    } else {
      s32 delta = state->end - state->start;

      state->step++;
      state->value = state->start +
                     Iwram_SignedDivide(delta * state->step, state->duration);
    }
  }

  value = state->value - 1;
  state->ditherToggle ^= 1;
  blend = 0;
  if (value & 32)
    blend = 15;
  value = (value & 31) * 2;
  for (i = 0; i <= 1; i++) {
    u8 entry = gData_080f0206[value & 63];
    u8 *p = &state->paletteNibbles[entry >> 1];

    if (entry & 1)
      *p = (*p & 0x0f) | (blend << 4);
    else
      *p = (*p & 0xf0) | blend;
    value++;
  }

  IO_WRITE_QUEUE_PUSH((u32)BG_CHAR_BLOCK(0), (u32)state->paletteNibbles,
                      DMA_COPY_WORDS(8));
}
