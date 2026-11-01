// Draft, differ 23 of 384, skeleton gap 7 (tla-en, 2026-11-01)
// Measured as the three functions it holds, in listing order: alchemy-devtools
// diff with 0811d24c,0811d2f8,0811d414 (BattleLink_SendActions and
// BattleLink_ReceiveActions, nested in the parent, adopt with it). Against
// 0811d414 alone the two nested functions count as extra: differ 230, gap 214.
// Both nested functions match. All that is left is the parent's first block:
// the native has add r2, sp, #8 ahead of mov r7, sp and str r0 ahead of str r1;
// the draft takes mov r7, sp and str r1 first, which splits movs r2, #8 from
// add r2, sp, so they stay two instructions and every later branch and pool
// offset shifts by one (19 of the 23).
// Cause, from the sched2 dump: the native order needs insn 4 (str r0) above
// insn 6 (str r1), so insn 6 must rank 75. It ranks 76 because the store to
// count costs its reload ldr r0, [r7] 2 cycles under -mthumb-slow-loads' rule
// for a load from the address just stored (arm_adjust_cost), where any other
// store before a load through r7 costs 1. This is a question about that
// approximation (K2), not this source; plain C cannot give the store and the
// load different addresses, as the native uses r7 for both.
// Tried: a long result and state count, or a long count parameter, give the
// native order (differ 8, gap 8) but CSE then forwards r1 and drops the load, so
// those types are wrong; an integer actions parameter (alias set of count) only
// moves str r1.
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
#include "heap_release.h"
#include "system.h"
#include "battle_work.h"
#include "battle_random.h"
#include "battle_system.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "serial.h"

s32 Serial_StartSend(void *source, s32 size);
s32 Serial_GetActiveTransfers(void);
void Heap_Release(void *block);

extern volatile u16 gLinkStatus;

struct BattleLinkedActionState {
  s32 count;
  s32 check;
  s32 seed;
  u8 unk0c[28];
};

s32 Battle_AppendLinkedActions(struct BattleActionRecord *actions,
                                           s32 count) {
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  s32 result = 0;
  s32 allocationSize = (u32)(count * 16 + 19) / 20 * 20;
  struct BattleLinkedActionState *state =
      (struct BattleLinkedActionState *)Heap_AllocEwram(40);
  s32 index;

  s32 BattleLink_SendActions(void) {
    s32 timeout;
    s32 disconnected;
    s32 status = Serial_StartSend(state, 20);

    disconnected = 0;
    timeout = 300;
    if (status == -1)
      return status;
    while (Serial_GetActiveTransfers()) {
      wait(1);
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
      status = Serial_StartSend(actions, allocationSize);
      if (status == -1)
        return status;
      while (Serial_GetActiveTransfers()) {
        wait(1);
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
    s32 status = Serial_StartReceive(state);

    disconnected = 0;
    timeout = 300;
    if (status == -1)
      return status;
    while (Serial_GetActiveTransfers()) {
      if (gSerialReceivedSize > 20)
        return -1;
      wait(1);
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
      status = Serial_StartReceive(actions + count);
      if (status == -1)
        return status;
      while (Serial_GetActiveTransfers()) {
        if (gSerialReceivedSize > (u32)(result * 16 + 19) / 20 * 20)
          return -1;
        wait(1);
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

    Heap_Release(state);
    return result;
  }

fail:
  BattleLink_ResetTransferState();
  Serial_RemoveIrqHandlers();
  Heap_Release(state);
  return -1;
}
