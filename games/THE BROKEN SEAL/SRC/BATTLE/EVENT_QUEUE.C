#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "OBJDISP.H"
#include "ITEM.H"
#include "BTLSYS.H"
#include "BATTLE_STATUS_ICON.H"
#include "GLOBAL_CELLS.H"
#include "BTLEV.H"
#include "BTLWORK.H"
void BattleEv_MarkDisplay(void)
{
    gBattleDisplayWork->marked = 1;
}

void ObjectDispatch_ReleaseFar(struct DispatchObject *object);
struct BtlObj *BtlObjGet(s32 arg0);
s32 ActivateBattleObjectSlot(s32 arg0);

s32 BattleActor_DestroyTemporaryObject(s32 arg0)
{
    /* FAKEMATCH: needed to match: the shared discard-only result type preserves TBS epilogue register choice */
    struct BtlUnit *creature;
    struct BtlObj *runtime;

    creature = UnitGetFar(arg0);
    if (creature->side == 1) {
        Owner_UpdateRatioPairFar(creature, 0);
        BattleActor_RemoveFromLists(arg0);
        ActivateBattleObjectSlot(arg0);
        runtime = BtlObjGet(arg0);
        ObjectDispatch_ReleaseFar((struct DispatchObject *)runtime->object);
        runtime->object = 0;
        runtime->active = 0;
    }
}

void Battle_SetRuntimeFlagBit0(struct BattleEventState *runtime, u32 operand)
{
    runtime->flags |= 1;
}

void UiWork_PushValueSlotFar(u32, u32);
void Audio_PlayCue(u32);
void BattleMotion_RunValueSequence();

u32 BattleEv_DispatchQueued(void)
{
    struct BattleEventState *runtime = &BtlWk->events;
    struct BattleEventQueue *queue = &runtime->queue;
    s32 i;

    for (i = 0; i < queue->count; i++) {
        u8 opcode = queue->opcodes[i];
        if (opcode <= BEV_SCR) switch (opcode) {
        case BEV_SCR: Battle_SetRuntimeFlagBit0(runtime, queue->operands[i]); break;
        case BEV_OBJ_EFX: BattleActor_DestroyTemporaryObject(queue->operands[i]); break;
        case BEV_UNIT: UiWork_PushValueSlotFar(queue->operands[i], 1); break;
        case BEV_NUM: UiWork_PushValueSlotFar(queue->operands[i], 5); break;
        case BEV_ITEM: UiWork_PushValueSlotFar(queue->operands[i] & ITEM_ID_MASK, 2); break;
        case BEV_ACT: UiWork_PushValueSlotFar(queue->operands[i] & OWNER_ACTION_ID_MASK, 4); break;
        case BEV_MARK: gBattleDisplayWork->marked = 1; break;
        case BEV_CLR: UiWork_ClearValueNameTablesFar(); break;
        case BEV_MES:
            if ((s32)queue->operands[i] >= 0) UiText_ShowMessageAndWaitCoreFar(queue->operands[i]);
            BattlePresentation_WaitForAdvance();
            UiWork_ClearValueNameTablesFar();
            break;
        case BEV_MES_ADD:
            if ((s32)queue->operands[i] >= 0) UiText_ShowMessageAndWaitCoreFar(queue->operands[i]);
            UiWork_ClearValueNameTablesFar();
            break;
        case BEV_OBJ_BGN:
            if (runtime->pending_cue > 0) Audio_PlayCue(runtime->pending_cue);
            BattleMotion_RunValueSequence(queue->operands[i], 0, 0);
            break;
        case BEV_OBJ_RES:
            BattleEnemy_RecordDefeat(queue->operands[i], runtime->flags);
            BattleActor_ResetRuntimeFields(queue->operands[i]);
            BattleMotion_InitializeActorRecords(queue->operands[i]);
            break;
        case BEV_REDRAW: PtyStDrawFar(BtlWk->pty_st); break;
        case BEV_OBJ_END:
            BattleUnit_BuildStatusFlags(queue->operands[i], BtlObjGet(queue->operands[i]));
            BattlePres_SetActorModeAndAction(queue->operands[i]);
            break;
        }
    }
    return BattleEventRuntime_Reset();
}

u32 BtlEvPush(u32 opcode, u32 operand)
{
    struct BattleEventQueue *queue = &BtlWk->events.queue;
    u32 index = queue->count;

    queue->opcodes[index] = opcode;
    queue->operands[index] = operand;
    queue->count = index + 1;
    return opcode;
}
