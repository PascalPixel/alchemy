#include "types.h"
#include "task.h"
#include "mem.h"
#include "dma.h"
#include "io_reg.h"
#include "display_transition.h"

void Func_080cf78c(void);

void ScreenTransition_InitializeBattleEffectState(s32 mode) {
  struct ScreenTransitionState *state =
      HeapSlot_AllocEwram(124, sizeof(*state));
  volatile u32 zero;

  zero = 0;
  Dma_Set((const void *)&zero, state, 0x85000000 | (sizeof(*state) / 4),
          REG_DMA3);
  state->mode = mode;
  state->value = 0;
  state->mask = 0x3f3f;
  state->active = (REG_WINOUT & 0xff00) | 1;
  Task_Add(Func_080cf78c, TASK_PASS_VBLANK);
  Task_Add(BattleEffect_StartWindowHBlankDma, TASK_PASS_FRAME - 10);
}

void Func_080d0184(void) {
  Task_SuspendAll(Func_080cf78c);
  Task_SuspendAll(BattleEffect_StartWindowHBlankDma);
}

void *Func_080d019c(void) {
  void *block = HeapSlot_AllocEwram(124, 0x540);
  volatile u32 zero = 0;

  Dma_Set(&zero, block, 0x85000150, (volatile u32 *)0x040000d4);
  return block;
}
