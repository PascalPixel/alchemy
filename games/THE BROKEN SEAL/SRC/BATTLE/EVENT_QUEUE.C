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
#include "BATTLE_RUNTIME.H"
#include "BATTLE_STATUS_ICON.H"
#include "GLOBAL_CELLS.H"
#include "BATTLE_EVENT.H"
#include "BATTLE_WORK.H"
void BattleEv_MarkDisplay(void)
{
    gBattleDisplayWork->marked = 1;
}

void ObjectDispatch_ReleaseFar(struct DispatchObject *object);
struct BattleObjectSlot *GetBattleObjectSlot(s32 arg0);
s32 ActivateBattleObjectSlot(s32 arg0);

s32 BattleActor_DestroyTemporaryObject(s32 arg0)
{
    /* FAKEMATCH: the shared discard-only result type preserves TBS epilogue register choice. No result is taken from the void destructor or returned. */
    struct BattleUnit *creature;
    struct BattleObjectSlot *runtime;

    creature = Owner_GetStateFar(arg0);
    if (creature->status_12a == 1) {
        Owner_UpdateRatioPairFar(creature, 0);
        BattleActor_RemoveFromLists(arg0);
        ActivateBattleObjectSlot(arg0);
        runtime = GetBattleObjectSlot(arg0);
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
    struct BattleEventState *runtime = &gBattleWork->events;
    struct BattleEventQueue *queue = &runtime->queue;
    s32 i;

    for (i = 0; i < queue->count; i++) {
        u8 opcode = queue->opcodes[i];
        if (opcode <= BATTLE_EVENT_SCRIPT_UPDATE) switch (opcode) {
        case BATTLE_EVENT_SCRIPT_UPDATE: Battle_SetRuntimeFlagBit0(runtime, queue->operands[i]); break;
        case BATTLE_EVENT_ACTOR_EFFECT: BattleActor_DestroyTemporaryObject(queue->operands[i]); break;
        case BATTLE_EVENT_UNIT: UiWork_PushValueSlotFar(queue->operands[i], 1); break;
        case BATTLE_EVENT_VALUE: UiWork_PushValueSlotFar(queue->operands[i], 5); break;
        case BATTLE_EVENT_ITEM: UiWork_PushValueSlotFar(queue->operands[i] & ITEM_ID_MASK, 2); break;
        case BATTLE_EVENT_ACTION: UiWork_PushValueSlotFar(queue->operands[i] & OWNER_ACTION_ID_MASK, 4); break;
        case BATTLE_EVENT_MARK: gBattleDisplayWork->marked = 1; break;
        case BATTLE_EVENT_RESET: UiWork_ClearValueNameTablesFar(); break;
        case BATTLE_EVENT_TEXT:
            if ((s32)queue->operands[i] >= 0) UiText_ShowMessageAndWaitCoreFar(queue->operands[i]);
            BattlePresentation_WaitForAdvance();
            UiWork_ClearValueNameTablesFar();
            break;
        case BATTLE_EVENT_TEXT_CONTINUE:
            if ((s32)queue->operands[i] >= 0) UiText_ShowMessageAndWaitCoreFar(queue->operands[i]);
            UiWork_ClearValueNameTablesFar();
            break;
        case BATTLE_EVENT_ACTOR_BEGIN:
            if (runtime->pending_cue > 0) Audio_PlayCue(runtime->pending_cue);
            BattleMotion_RunValueSequence(queue->operands[i], 0, 0);
            break;
        case BATTLE_EVENT_ACTOR_RESOLVE:
            BattleEnemy_RecordDefeat(queue->operands[i], runtime->flags);
            BattleActor_ResetRuntimeFields(queue->operands[i]);
            BattleMotion_InitializeActorRecords(queue->operands[i]);
            break;
        case BATTLE_EVENT_REFRESH: UiWindow_DrawPartyStatusContentsFar(gBattleWork->party_status_mode); break;
        case BATTLE_EVENT_ACTOR_FINISH:
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
    u32 index = queue->count;

    queue->opcodes[index] = opcode;
    queue->operands[index] = operand;
    queue->count = index + 1;
    return opcode;
}
