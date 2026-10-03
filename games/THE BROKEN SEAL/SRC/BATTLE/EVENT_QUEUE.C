#include "TYPES.H"
#include "BATTLE_STATUS_ICON.H"
#include "GLOBAL_CELLS.H"
#include "BATTLE_EVENT.H"
#include "BATTLE_WORK.H"
void BattleEv_SetRuntimeField8(void)
{
    gBattleDisplayWork->marked = 1;
}

s32 Object_Destroy(s32);
struct BattleUnit *Owner_GetStateFar();
s32 Owner_UpdateRatioPairFar(void *, s32);
struct BattleObjectSlot *GetBattleObjectSlot(s32 arg0);
s32 ActivateBattleObjectSlot(s32 arg0);
s32 BattleActor_RemoveFromLists(s32);

s32 BattleActor_DestroyTemporaryObject(s32 arg0)
{
    s32 result;
    struct BattleUnit *creature;
    struct BattleObjectSlot *runtime;

    creature = Owner_GetStateFar();
    if (creature->status_12a == 1) {
        Owner_UpdateRatioPairFar(creature, 0);
        BattleActor_RemoveFromLists(arg0);
        ActivateBattleObjectSlot(arg0);
        runtime = GetBattleObjectSlot(arg0);
        result = Object_Destroy((s32)runtime->object);
        runtime->object = 0;
        runtime->active = 0;
        return result;
    }
    return (s32)creature;
}

/* Opcode 13: defeats from here on earn spoils. */
void Battle_SetRuntimeFlagBit0(struct BattleEventState *runtime, u32 operand)
{
    runtime->flags |= 1;
}

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))





void UiWork_PushValueSlotFar(u32, u32);
void UiText_ShowMessageAndWaitCoreFar(u32);
void BattlePresentation_WaitForAdvance(void);
void UiWork_ClearValueNameTablesFar(void);
void Audio_PlayCue(u32);
void BattleMotion_RunValueSequence(u32, u32, u32);
void BattleEnemy_RecordDefeat(u32, u32);
void BattleActor_ResetRuntimeFields(u32);
void BattleMotion_InitializeActorRecords(u32);
void UiWindow_DrawPartyStatusContentsFar(u32);
void BattlePres_SetActorModeAndAction(u32);

u32 BattleEv_DispatchQueued(void)
{
    struct BattleEventState *runtime = &gBattleWork->events;
    struct BattleEventQueue *queue = &runtime->queue;
    s32 i;

    for (i = 0; i < queue->count; i++) {
        u8 opcode = queue->opcodes[i];
        if (opcode <= 13) switch (opcode) {
        case 13: Battle_SetRuntimeFlagBit0(runtime, queue->operands[i]); break;
        case 12: BattleActor_DestroyTemporaryObject(queue->operands[i]); break;
        case 0: UiWork_PushValueSlotFar(queue->operands[i], 1); break;
        case 1: UiWork_PushValueSlotFar(queue->operands[i], 5); break;
        case 2: UiWork_PushValueSlotFar(queue->operands[i] & 0x1ff, 2); break;
        case 3: UiWork_PushValueSlotFar(queue->operands[i] & 0x3fff, 4); break;
        case 6: gBattleDisplayWork->marked = 1; break;
        case 7: UiWork_ClearValueNameTablesFar(); break;
        case 4:
            if ((s32)queue->operands[i] >= 0) UiText_ShowMessageAndWaitCoreFar(queue->operands[i]);
            BattlePresentation_WaitForAdvance();
            UiWork_ClearValueNameTablesFar();
            break;
        case 5:
            if ((s32)queue->operands[i] >= 0) UiText_ShowMessageAndWaitCoreFar(queue->operands[i]);
            UiWork_ClearValueNameTablesFar();
            break;
        case 8:
            if (runtime->pending_cue > 0) Audio_PlayCue(runtime->pending_cue);
            BattleMotion_RunValueSequence(queue->operands[i], 0, 0);
            break;
        case 9:
        {
            /* FAKEMATCH: the operand's byte offset is formed before the
               flags are read, as the ROM schedules it. */
            u32 operand_offset = (u8 *)&queue->operands[i] - (u8 *)queue;
            u32 flags = runtime->flags;

            BattleEnemy_RecordDefeat(FIELD(queue, u32, operand_offset), flags);
            BattleActor_ResetRuntimeFields(FIELD(queue, u32, operand_offset));
            BattleMotion_InitializeActorRecords(FIELD(queue, u32, operand_offset));
            break;
        }
        case 10: UiWindow_DrawPartyStatusContentsFar(gBattleWork->party_status_mode); break;
        case 11:
            BattleUnit_BuildStatusFlags(queue->operands[i], GetBattleObjectSlot(queue->operands[i]));
            BattlePres_SetActorModeAndAction(queue->operands[i]);
            break;
        }
    }
    return BattleEventRuntime_Reset();
}


u32 BattleEv_Push(u32 opcode, u32 operand)
{
    struct BattleEventQueue *queue = &gBattleWork->events.queue;
    u32 *count = (u32 *)&queue->count;
    u32 index = *count;

    queue->opcodes[index] = opcode;
    queue->operands[index] = operand;
    *count = index + 1;
    return opcode;
}
