#include "types.h"
#include "battle_work.h"

struct BattleDisplayFlags
{
    u8 unknown_00[8];
    s32 marked;
};

void Battle_SetRuntimeFlagBit0(struct BattleEventState *runtime, u32 operand);
s32 BattleActor_DestroyTemporaryObject(s32 unit_id);
void UiText_DrawQuantity(s32 value, s32 kind);
void UiText_ShowMessageAndWaitCoreFar(u32 message);
void BattlePresentation_WaitForAdvance(void);
void UiWork_ClearValueNameTablesFar(void);
void Audio_PlayCue(s32 cue);
void BattleMotion_RunValueSequence(u32 unit_id, s32 a, s32 b);
s32 Func_0812824c(s32 unitId, s32 earned);
void BattleActor_ResetRuntimeFields(u32 unit_id);
void BattleMotion_InitializeActorRecords(u32 unit_id);
void PtyStDrawFar(s32 state);
struct BtlObj *BtlObjGet(s32 object_id);
void BattleUnit_BuildStatusFlags(u32 unit_id, struct BtlObj *slot);
s32 BattlePres_SetActorModeAndAction(s32 id);
s32 BattleActor_RemoveFromLists(s32 unit_id);
u32 BattleEventRuntime_Reset(void);

// Runs the queued battle events in order, then resets the queue.
u32 BattleEv_DispatchQueued(void)
{
    struct BtlWork *work;
    struct BattleEventState *runtime;
    struct BattleEventQueue *queue;
    s32 i;

    /* FAKEMATCH: the one-pass block loads the battle work before the events offset is built (differ 2 without it), as in BattleEventRuntime_Reset. */
    do
    {
        work = Ram_HeapSlots->battle_work;
    } while (0);
    runtime = &work->events;
    queue = &runtime->queue;

    for (i = 0; i < queue->count; i++)
    {
        u8 opcode = queue->opcodes[i];
        if (opcode <= 15)
            switch (opcode)
            {
            case BEV_SCR:
                Battle_SetRuntimeFlagBit0(runtime, queue->operands[i]);
                break;
            case BEV_OBJ_EFX:
                BattleActor_DestroyTemporaryObject(queue->operands[i]);
                break;
            case BEV_UNIT:
                UiText_DrawQuantity(queue->operands[i], 1);
                break;
            case BEV_NUM:
                UiText_DrawQuantity(queue->operands[i], 5);
                break;
            case BEV_ITEM:
                UiText_DrawQuantity(queue->operands[i] & 0x1ff, 2);
                break;
            case BEV_ACT:
                UiText_DrawQuantity(queue->operands[i] & 0x3fff, 4);
                break;
            case BEV_MARK:
                ((struct BattleDisplayFlags *)Ram_HeapSlots->unknown_84[4])->marked = 1;
                break;
            case BEV_CLR:
                UiWork_ClearValueNameTablesFar();
                break;
            case BEV_MES:
                if ((s32)queue->operands[i] >= 0)
                    UiText_ShowMessageAndWaitCoreFar(queue->operands[i]);
                BattlePresentation_WaitForAdvance();
                UiWork_ClearValueNameTablesFar();
                break;
            case BEV_MES_ADD:
                if ((s32)queue->operands[i] >= 0)
                    UiText_ShowMessageAndWaitCoreFar(queue->operands[i]);
                UiWork_ClearValueNameTablesFar();
                break;
            case BEV_OBJ_BGN:
                if (runtime->pending_cue > 0)
                    Audio_PlayCue(runtime->pending_cue);
                BattleMotion_RunValueSequence(queue->operands[i], 0, 0);
                break;
            case BEV_OBJ_RES:
                Func_0812824c(queue->operands[i], runtime->flags);
                BattleActor_ResetRuntimeFields(queue->operands[i]);
                BattleMotion_InitializeActorRecords(queue->operands[i]);
                break;
            case BEV_REDRAW:
                PtyStDrawFar(Ram_HeapSlots->battle_work->pty_st);
                break;
            case BEV_OBJ_END:
                BattleUnit_BuildStatusFlags(queue->operands[i], BtlObjGet(queue->operands[i]));
                BattlePres_SetActorModeAndAction(queue->operands[i]);
                break;
            case 15:
                BattleActor_RemoveFromLists(queue->operands[i]);
                break;
            }
    }
    return BattleEventRuntime_Reset();
}

void BtlEvPush(u32 opcode, u32 operand)
{
    struct BattleEventQueue *queue = &Ram_HeapSlots->battle_work->events.queue;
    u32 index = queue->count;

    queue->opcodes[index] = opcode;
    queue->operands[index] = operand;
    queue->count = index + 1;
}
