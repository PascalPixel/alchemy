#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "battle_event.h"

/* battle/event_runtime/set_runtime_field8.c */
void BattleEv_SetRuntimeField8(void)
{
    FIELD_AT_OFFSET(*(void **)ADDR_03001EE4, s32 *, 8) = 1;
}

/* battle/actor/destroy_temporary_object.c */
struct Creature_080bb8e8 {
    u8 padding_000[0x12a];
    u8 field_12a;
};

struct Runtime_080bb8e8 {
    s32 field_00;
    u8 padding_004[0x24];
    s16 field_28;
};

s32 Object_Destroy(s32);
struct Creature_080bb8e8 *Runtime_GetObject();

struct Runtime_080bb8e8 *GetBattleObjectSlot(s32 arg0);
s32 ActivateBattleObjectSlot(s32 arg0);

s32 BattleActor_DestroyTemporaryObject(s32 arg0)
{
    s32 result;
    struct Creature_080bb8e8 *creature;
    struct Runtime_080bb8e8 *runtime;

    creature = Runtime_GetObject();
    if (creature->field_12a == 1) {
        Actor_Apply(creature, 0);
        Actor_Check(arg0);
        ActivateBattleObjectSlot(arg0);
        runtime = GetBattleObjectSlot(arg0);
        result = Object_Destroy(runtime->field_00);
        runtime->field_00 = 0;
        runtime->field_28 = 0;
        return result;
    }
    return (s32)creature;
}

/* battle/event_runtime/set_runtime_flag_bit0.c */
struct Runtime_080bb928 {
    u8 padding_000[0x16c];
    s32 flags;
};

void Battle_SetRuntimeFlagBit0(struct Runtime_080bb928 *runtime)
{
    runtime->flags |= 1;
}

/* battle/event_runtime/dispatch_queued_events.c */
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

struct BattleEventRuntime {
    struct BattleEventQueue queue;
    u8 reserved_148[0x20];
    s32 pending_sound_id;
    u32 actor_auxiliary;
};

extern u8 *gBattleWork;
extern u8 *gIw;

u32 BattleEv_DispatchQueued(void)
{
    struct BattleEventRuntime *runtime = (void *)(gBattleWork + 0x6b8);
    struct BattleEventQueue *queue = &runtime->queue;
    s32 i;

    for (i = 0; i < FIELD(queue, s32, 0x144); i++) {
        u8 opcode = queue->opcodes[i];
        if (opcode <= 13) switch (opcode) {
        case 13: Battle_Apply(queue, queue->operands[i]); break;
        case 12: Battle_Do(queue->operands[i]); break;
        case 0: Battle_Apply2(queue->operands[i], 1); break;
        case 1: Battle_Apply2(queue->operands[i], 5); break;
        case 2: Battle_Apply2(queue->operands[i] & 0x1ff, 2); break;
        case 3: Battle_Apply2(queue->operands[i] & 0x3fff, 4); break;
        case 6: FIELD(gIw, s32, 8) = 1; break;
        case 7: Battle_Run(); break;
        case 4:
            if ((s32)queue->operands[i] >= 0) Battle_unk2_2(queue->operands[i]);
            Battle_unk2_4();
            Battle_Run();
            break;
        case 5:
            if ((s32)queue->operands[i] >= 0) Battle_unk2_2(queue->operands[i]);
            Battle_Run();
            break;
        case 8:
            if (runtime->pending_sound_id > 0) Battle_unk3_2(runtime->pending_sound_id);
            Battle_Place(queue->operands[i], 0, 0);
            break;
        case 9:
        {
            u32 operand_offset = i * 4 + 64;
            u32 auxiliary = runtime->actor_auxiliary;
            Battle_Apply3(FIELD(queue, u32, operand_offset), auxiliary);
            Battle_unk4_2(FIELD(queue, u32, operand_offset));
            Battle_unk5(FIELD(queue, u32, operand_offset));
            break;
        }
        case 10: Battle_unk6(gBattleWork[65]); break;
        case 11:
            Battle_Apply4(queue->operands[i], Battle_unk7(queue->operands[i]));
            Battle_unk8(queue->operands[i]);
            break;
        }
    }
    return Battle_unk3_4();
}

/* battle/event/push.c */
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

/* battle/effects/calc/can_affect_defeated_unit.c */
s32 BattleFx_CanAffectDefeatedUnit(s32 effect)
{
    switch (effect) {
    case 31:
    case 32:
    case 60:
    case 69:
        return 1;
    default:
        return 0;
    }
}
