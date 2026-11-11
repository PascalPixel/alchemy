#include "types.h"
#include "task.h"
#include "heap_release.h"
#include "dma.h"
#include "io_reg.h"
#include "display_transition.h"

void Func_080cf78c(void);

void ScreenTransition_InitializeBattleEffectState(s32 mode) {
  struct ScreenTransitionState *state =
      HeapSlot_AllocEwram(124, sizeof(*state));
  volatile u32 zero;

  zero = 0;
  Dma_Set((const void *)&zero, state,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | (sizeof(*state) / 4),
          REG_DMA3);
  state->mode = mode;
  state->value = 0;
  state->mask = 0x3f3f;
  state->active = (REG_WINOUT & 0xff00) | 1;
  Task_Add(Func_080cf78c, TASK_PASS_VBLANK);
  Task_Add(BattleEffect_StartWindowHBlankDma, TASK_PASS_FRAME - 10);
}

/// Unused: suspends the window transition's two tasks. Nothing calls it,
/// by name, far-call entry or table.
UNUSED void ScreenTransition_SuspendCallbacks(void) {
  Task_SuspendAll(Func_080cf78c);
  Task_SuspendAll(BattleEffect_StartWindowHBlankDma);
}

/// Allocates the screen transition state (heap slot 124) and clears it.
struct ScreenTransitionState *ScreenTransition_AllocateAndClearState(void) {
  struct ScreenTransitionState *state =
      HeapSlot_AllocEwram(124, sizeof(*state));
  volatile u32 zero = 0;

  Dma_Set(&zero, state,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | (sizeof(*state) / 4),
          REG_DMA3);
  return state;
}
