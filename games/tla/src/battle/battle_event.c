#include "types.h"
#include "battle_work.h"

struct BattleDisplayFlags {
  u8 unk00[8];
  s32 marked;
};

void Battle_SetRuntimeFlagBit0(struct BattleEventState *runtime, u32 operand);
s32 BattleActor_DestroyTemporaryObject(s32 unitId);
void UiText_DrawQuantity(s32 value, s32 kind);
void UiText_ShowMessageAndWaitCoreFar(u32 message);
void BattlePresentation_WaitForAdvance(void);
void UiWork_ClearValueNameTablesFar(void);
void Audio_PlayCue(s32 cue);
void BattleMotion_RunValueSequence(u32 unitId, s32 a, s32 b);
s32 Func_0812824c(s32 unitId, s32 earned);
void BattleActor_ResetRuntimeFields(u32 unitId);
void BattleMotion_InitializeActorRecords(u32 unitId);
void PartyStatusDrawFar(s32 state);
struct BattleObj *BattleObjGet(s32 objectId);
void BattleUnit_BuildStatusFlags(u32 unitId, struct BattleObj *slot);
s32 BattlePres_SetActorModeAndAction(s32 id);
s32 BattleActor_RemoveFromLists(s32 unitId);
u32 BattleEventRuntime_Reset(void);

// Runs the queued battle events in order, then resets the queue.
u32 BattleEv_DispatchQueued(void) {
  struct BattleWork *work;
  struct BattleEventState *runtime;
  struct BattleEventQueue *queue;
  s32 i;

  /* FAKEMATCH: the one-pass block loads the battle work before the events
   * offset is built (differ 2 without it), as in BattleEventRuntime_Reset. */
  do {
    work = Ram_HeapSlots->battleWork;
  } while (0);
  runtime = &work->events;
  queue = &runtime->queue;

  for (i = 0; i < queue->count; i++) {
    u8 opcode = queue->opcodes[i];
    if (opcode <= 15)
      switch (opcode) {
      case BATTLE_EVENT_DJINN_BONUS:
        Battle_SetRuntimeFlagBit0(runtime, queue->operands[i]);
        break;
      case BATTLE_EVENT_REMOVE_UNIT:
        BattleActor_DestroyTemporaryObject(queue->operands[i]);
        break;
      case BATTLE_EVENT_UNIT_NAME:
        UiText_DrawQuantity(queue->operands[i], 1);
        break;
      case BATTLE_EVENT_NUMBER:
        UiText_DrawQuantity(queue->operands[i], 5);
        break;
      case BATTLE_EVENT_ITEM_NAME:
        UiText_DrawQuantity(queue->operands[i] & 0x1ff, 2);
        break;
      case BATTLE_EVENT_ABILITY_NAME:
        UiText_DrawQuantity(queue->operands[i] & 0x3fff, 4);
        break;
      case BATTLE_EVENT_MARK:
        ((struct BattleDisplayFlags *)Ram_HeapSlots->unk84[4])->marked = 1;
        break;
      case BATTLE_EVENT_CLEAR_ARGUMENTS:
        UiWork_ClearValueNameTablesFar();
        break;
      case BATTLE_EVENT_MESSAGE:
        if ((s32)queue->operands[i] >= 0)
          UiText_ShowMessageAndWaitCoreFar(queue->operands[i]);
        BattlePresentation_WaitForAdvance();
        UiWork_ClearValueNameTablesFar();
        break;
      case BATTLE_EVENT_MESSAGE_NO_WAIT:
        if ((s32)queue->operands[i] >= 0)
          UiText_ShowMessageAndWaitCoreFar(queue->operands[i]);
        UiWork_ClearValueNameTablesFar();
        break;
      case BATTLE_EVENT_HIT:
        if (runtime->pendingCue > 0)
          Audio_PlayCue(runtime->pendingCue);
        BattleMotion_RunValueSequence(queue->operands[i], 0, 0);
        break;
      case BATTLE_EVENT_DEFEAT:
        Func_0812824c(queue->operands[i], runtime->flags);
        BattleActor_ResetRuntimeFields(queue->operands[i]);
        BattleMotion_InitializeActorRecords(queue->operands[i]);
        break;
      case BATTLE_EVENT_DRAW_STATUS:
        PartyStatusDrawFar(Ram_HeapSlots->battleWork->partyStatus);
        break;
      case BATTLE_EVENT_REFRESH_UNIT:
        BattleUnit_BuildStatusFlags(queue->operands[i],
                                    BattleObjGet(queue->operands[i]));
        BattlePres_SetActorModeAndAction(queue->operands[i]);
        break;
      case 15:
        BattleActor_RemoveFromLists(queue->operands[i]);
        break;
      }
  }
  return BattleEventRuntime_Reset();
}

void BattleEvPush(u32 opcode, u32 operand) {
  struct BattleEventQueue *queue = &Ram_HeapSlots->battleWork->events.queue;
  u32 index = queue->count;

  queue->opcodes[index] = opcode;
  queue->operands[index] = operand;
  queue->count = index + 1;
}
