#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "BATTLE_EVENT.H"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void BattleEv_SetRuntimeField8(void)
{
    FIELD_AT_OFFSET(*(void **)ADDR_03001EE4, s32 *, 8) = 1;
}

struct BattleEventActor {
    u8 padding_000[0x12a];
    u8 field_12a;
};

struct BattleEventObjectSlot {
    s32 field_00;
    u8 padding_004[0x24];
    s16 field_28;
};

s32 Object_Destroy(s32);
struct BattleEventActor *Runtime_GetObject();
s32 Owner_UpdateRatioPairFar(void *, s32);
struct BattleEventObjectSlot *GetBattleObjectSlot(s32 arg0);
s32 ActivateBattleObjectSlot(s32 arg0);
s32 BattleActor_RemoveFromLists(s32);

s32 BattleActor_DestroyTemporaryObject(s32 arg0)
{
    s32 result;
    struct BattleEventActor *creature;
    struct BattleEventObjectSlot *runtime;

    creature = Runtime_GetObject();
    if (creature->field_12a == 1) {
        Owner_UpdateRatioPairFar(creature, 0);
        BattleActor_RemoveFromLists(arg0);
        ActivateBattleObjectSlot(arg0);
        runtime = GetBattleObjectSlot(arg0);
        result = Object_Destroy(runtime->field_00);
        runtime->field_00 = 0;
        runtime->field_28 = 0;
        return result;
    }
    return (s32)creature;
}

struct BattleEventFlagWork {
    u8 padding_000[0x16c];
    s32 flags;
};

void Battle_SetRuntimeFlagBit0(struct BattleEventFlagWork *runtime, u32 operand)
{
    runtime->flags |= 1;
}

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

struct BattleEventRuntime {
    struct BattleEventQueue queue;
    u8 reserved_148[0x20];
    s32 pending_sound_id;
    u32 actor_auxiliary;
};

extern u8 *gBattleWork;
extern u8 *gBattleDisplayWork;



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
void BattleUnit_BuildStatusFlags(u32, u32);
void BattlePres_SetActorModeAndAction(u32);
u32 BattleEventRuntime_Reset(void);

u32 BattleEv_DispatchQueued(void)
{
    struct BattleEventRuntime *runtime = (void *)(gBattleWork + 0x6b8);
    struct BattleEventQueue *queue = &runtime->queue;
    s32 i;

    for (i = 0; i < FIELD(queue, s32, 0x144); i++) {
        u8 opcode = queue->opcodes[i];
        if (opcode <= 13) switch (opcode) {
        case 13: Battle_SetRuntimeFlagBit0((struct BattleEventFlagWork *)queue, queue->operands[i]); break;
        case 12: BattleActor_DestroyTemporaryObject(queue->operands[i]); break;
        case 0: UiWork_PushValueSlotFar(queue->operands[i], 1); break;
        case 1: UiWork_PushValueSlotFar(queue->operands[i], 5); break;
        case 2: UiWork_PushValueSlotFar(queue->operands[i] & 0x1ff, 2); break;
        case 3: UiWork_PushValueSlotFar(queue->operands[i] & 0x3fff, 4); break;
        case 6: FIELD(gBattleDisplayWork, s32, 8) = 1; break;
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
            if (runtime->pending_sound_id > 0) Audio_PlayCue(runtime->pending_sound_id);
            BattleMotion_RunValueSequence(queue->operands[i], 0, 0);
            break;
        case 9:
        {
            u32 operand_offset = i * 4 + 64;
            u32 auxiliary = runtime->actor_auxiliary;
            BattleEnemy_RecordDefeat(FIELD(queue, u32, operand_offset), auxiliary);
            BattleActor_ResetRuntimeFields(FIELD(queue, u32, operand_offset));
            BattleMotion_InitializeActorRecords(FIELD(queue, u32, operand_offset));
            break;
        }
        case 10: UiWindow_DrawPartyStatusContentsFar(gBattleWork[65]); break;
        case 11:
            BattleUnit_BuildStatusFlags(queue->operands[i], (u32)GetBattleObjectSlot(queue->operands[i]));
            BattlePres_SetActorModeAndAction(queue->operands[i]);
            break;
        }
    }
    return BattleEventRuntime_Reset();
}


u32 BattleEv_Push(u32 opcode, u32 operand)
{
    u8 *base = *(u8 **)ADDR_03001E74;
    struct BattleEventQueue *queue =
        (struct BattleEventQueue *)(base + 1720);
    u32 *count = (u32 *)&queue->count;
    u32 index = *count;

    queue->opcodes[index] = opcode;
    queue->operands[index] = operand;
    *count = index + 1;
    return opcode;
}
