// Near miss (12 of 384 instructions differ, all load scheduling): the
// Broken Seal link exchange with its two nested transfers, ported with
// TLA names; four loads sit one slot from the native. 2026-10-12: the first
// pair is in BattleLink_SendActions' prologue (the native stores the
// static chain as sub sp; mov r3, sp; mov r2, r9; str; adds r7, r2). An
// empty asm barrier as the function's first statement leaves one row there
// (mov r2, r9 ahead of mov r3, sp: sched2 ranks the two equal and takes the
// one with more dependents); a one-pass block changes nothing. The three
// functions adopt together, so the prologue blocks all 852 bytes.
// 2026-10-12, queue 7: sched2's dump of that block (alchemy diff --rtl,
// 0811d414.c.23.sched2) gives the chain's copy (r7 = r2) priority 36 and its
// store 35, because the copy leads to subs r3, r7, #4 and the load of state
// (cost 2) while the store reaches that load directly; the native order needs
// the two equal, which is the order flow2 already has them in. The other three
// pairs (state's count load at the second transfer, two in the parent) were not
// tried with barriers. BattleQueue_SortByPriority, their neighbour, is adopted
// in games/tla/src/battle/turn.c, where these three belong.
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
#include "mem.h"
#include "system.h"
#include "battle_work.h"
#include "battle_random.h"
#include "battle_system.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "serial_runtime.h"

s32 SerialRuntime_StartSend(void *source, s32 size);
s32 SerialRuntime_GetActiveTransfers(void);
void MemFree(void *block);

extern volatile u16 gLinkStatus;

struct BattleLinkedActionState {
  s32 count;
  s32 check;
  s32 seed;
  u8 unk0c[28];
};

s32 BattlePresentation_AppendLinkedActions(struct BattleActionRecord *actions,
                                           s32 count) {
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  s32 result = 0;
  s32 allocationSize = (u32)(count * 16 + 19) / 20 * 20;
  struct BattleLinkedActionState *state =
      (struct BattleLinkedActionState *)Runtime_BumpAllocateAlternatePool(40);
  s32 index;

  s32 BattleLink_SendActions(void) {
    s32 timeout;
    s32 disconnected;
    s32 status = SerialRuntime_StartSend(state, 20);

    disconnected = 0;
    timeout = 300;
    if (status == -1)
      return status;
    while (SerialRuntime_GetActiveTransfers()) {
      WaitTicks(1);
      if (--timeout < 0)
        return -1;
      if ((gLinkStatus & 3) != 3) {
        if (++disconnected > 24)
          return -1;
      } else {
        disconnected = 0;
      }
    }
    if (allocationSize) {
      status = SerialRuntime_StartSend(actions, allocationSize);
      if (status == -1)
        return status;
      while (SerialRuntime_GetActiveTransfers()) {
        WaitTicks(1);
        if (--timeout < 0)
          return -1;
        if ((gLinkStatus & 3) != 3) {
          if (++disconnected > 24)
            return -1;
        } else {
          disconnected = 0;
        }
      }
    }
    return 0;
  }

  s32 BattleLink_ReceiveActions(void) {
    s32 timeout;
    s32 disconnected;
    s32 status = SerialRuntime_StartReceive(state);

    disconnected = 0;
    timeout = 300;
    if (status == -1)
      return status;
    while (SerialRuntime_GetActiveTransfers()) {
      if (gSerialReceivedSize > 20)
        return -1;
      WaitTicks(1);
      if (--timeout < 0)
        return -1;
      if ((gLinkStatus & 3) != 3) {
        if (++disconnected > 24)
          return -1;
      } else {
        disconnected = 0;
      }
    }
    if (gSerialReceivedSize != 20)
      return -1;
    result = state->count;
    if (state->count) {
      status = SerialRuntime_StartReceive(actions + count);
      if (status == -1)
        return status;
      while (SerialRuntime_GetActiveTransfers()) {
        if (gSerialReceivedSize > (u32)(result * 16 + 19) / 20 * 20)
          return -1;
        WaitTicks(1);
        if (--timeout < 0)
          return -1;
        if ((gLinkStatus & 3) != 3) {
          if (++disconnected > 24)
            return -1;
        } else {
          disconnected = 0;
        }
      }
      if (gSerialReceivedSize != (u32)(result * 16 + 19) / 20 * 20)
        return -1;
    }
    return 0;
  }

  for (index = 0; index < count; index++) {
    struct BattleActionRecord *action = &actions[index];

    action->slot = battle->ownerSlots[action->unitId];

    if (battle->linkSide == 0) {
      if (action->priority & 1)
        action->priority++;
    } else {
      action->priority |= 1;
    }
  }

  if (battle->linkPaused == 0) {
    if (battle->linkSide == 0) {
      volatile u16 *ime;
      u32 saved;

      state->count = count;
      state->check = Battle_RandomFar();
      /* FAKEMATCH: the one-pass IME scope keeps the seed copy between writes
       * and reloads state after the first write. */
      do {
        ime = &REG_IME;
        saved = *ime;
        *ime = (u32)ime;
      } while (0);
      gBattleRandomSeed = state->seed = gRandomState;
      do { /* FAKEMATCH: the one-pass restore, see above */
        *ime = saved;
      } while (0);
      if (BattleLink_SendActions() < 0 || BattleLink_ReceiveActions() < 0)
        goto fail;
      result = state->count;
    } else {
      if (BattleLink_ReceiveActions() < 0)
        goto fail;
      result = state->count;
      state->count = count;
      if (BattleLink_SendActions() < 0)
        goto fail;
      if (Battle_RandomFar() != state->check)
        goto fail;
      gBattleRandomSeed = state->seed;
    }

    for (index = 0; index < result; index++) {
      struct BattleActionRecord *action = &actions[count + index];

      action->unitId = action->slot;
      action->target = action->target ^ 0x80;
    }

    MemFree(state);
    return result;
  }

fail:
  BattleLink_ResetTransferState();
  SerialRuntime_RemoveIrqHandlers();
  MemFree(state);
  return -1;
}
