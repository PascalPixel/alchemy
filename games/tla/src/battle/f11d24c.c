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
#include "irq_lock.h"

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

s32 Battle_AppendLinkedActions(struct BattleActionRecord *actions, s32 count) {
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
      u32 saved;

      state->count = count;
      state->check = Battle_RandomFar();
      IRQ_LOCK(saved);
      gBattleRandomSeed = state->seed = gRandomState;
      IRQ_UNLOCK(saved);
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
