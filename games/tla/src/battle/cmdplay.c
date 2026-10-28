#include "battle.h"
#include "system.h"
#include "dma.h"
#include "io_reg.h"
#include "battle_work.h"
#include "task.h"

u32 Math_UnsignedMulHigh(u32 a, u32 b);

void Func_08122c18(void) {
  Math_UnsignedMulHigh(5, 0);
  Dma_Set(0, 0, (DMA_ENABLE | DMA_32BIT) << 16, REG_DMA3);
  Dma_Set(0, 0, (DMA_ENABLE | DMA_32BIT) << 16, REG_DMA3);
  Dma_Set(0, 0, (DMA_ENABLE | DMA_32BIT) << 16, REG_DMA3);
}

void BattlePlayback_Start(s32 cue) {
  struct BattleEventState *ev;

  ev = &Ram_HeapSlots->battleWork->events;
  if (ev->phase == BATTLE_PLAYBACK_IDLE) {
    ev->phase = BATTLE_PLAYBACK_RESOLVE_TARGET;
    if (cue != 0) {
      if (ev->unk16a == 0)
        ev->pendingCue = cue;
    }
  }
}

s32 BattlePlayback_SchedulePhase(s32 frames) {
  struct BattleWork *work;
  struct BattleEventState *ev;

  /* FAKEMATCH: the one-pass block loads the battle work before the events
   * offset is built (differ 2 without it), as in BattlePlayback_Reset. */
  do {
    work = Ram_HeapSlots->battleWork;
  } while (0);
  ev = &work->events;
  ev->queue.count = 0;
  ev->eventIndex = 0;
  ev->timer = frames;
  ev->phase = BATTLE_PLAYBACK_DISPATCH;
  work->unk670[0x0d] = 0;
  return Task_Add(BattleEvent_Playback, TASK_PASS_VBLANK);
}
