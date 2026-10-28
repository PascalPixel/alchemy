#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "battle_work.h"
#include "system.h"
#include "task.h"

u32 BattlePlayback_Reset(void);

u32 BattlePlayback_Reset(void) {
  struct BattleEventState *events;
  struct BattleWork *work;

  /* FAKEMATCH: the one-pass block loads the battle work before the events
   * offset is built (differ 2 without it). */
  do {
    work = Ram_HeapSlots->battleWork;
  } while (0);
  events = &work->events;
  events->phase = BATTLE_PLAYBACK_IDLE;
  events->queue.count = 0;
  events->eventIndex = 0;
  events->timer = 0;
  events->queue.targetIndex = 0;
  events->pendingCue = 0x86;
  events->unk16a = 0;
  events->flags = 0;
}

s32 BattlePlayback_WaitForReady(void) {
  s32 state;
  struct BattleEventState *events;

  events = &Ram_HeapSlots->battleWork->events;
  state = events->phase;
  if (state == BATTLE_PLAYBACK_IDLE) {
    events->phase = BATTLE_PLAYBACK_RESOLVE_TARGET;
    state = BATTLE_PLAYBACK_RESOLVE_TARGET;
  }
  if (state != BATTLE_PLAYBACK_READY) {
    do {
      WaitFrames(1U);
    } while (events->phase != BATTLE_PLAYBACK_READY);
  }
  Task_Remove((u32)BattleEvent_Playback);
  return BattlePlayback_Reset();
}
