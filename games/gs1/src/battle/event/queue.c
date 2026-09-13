#include "types.h"
#include "global_cells.h"
#include "battle_event.h"

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
s32 Func_08077130(void *, s32);
struct BattleEventObjectSlot *GetBattleObjectSlot(s32 arg0);
s32 ActivateBattleObjectSlot(s32 arg0);
s32 Func_080bac6c(s32);

s32 BattleActor_DestroyTemporaryObject(s32 arg0)
{
    s32 result;
    struct BattleEventActor *creature;
    struct BattleEventObjectSlot *runtime;

    creature = Runtime_GetObject();
    if (creature->field_12a == 1) {
        Func_08077130(creature, 0);
        Func_080bac6c(arg0);
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

extern u8 *Data_03001e74;
extern u8 *Data_03001ee4;



void Func_08015120(u32, u32);
void Func_080151c8(u32);
void Func_080bb65c(void);
void Func_08015118(void);
void Func_080f9010(u32);
void Func_080babdc(u32, u32, u32);
void Func_080c24f0(u32, u32);
void Func_080bb588(u32);
void Func_080bace8(u32);
void Func_08015130(u32);
void Func_080b78e4(u32, u32);
void Func_080b7aac(u32);
u32 Func_080bdfec(void);

u32 BattleEv_DispatchQueued(void)
{
    struct BattleEventRuntime *runtime = (void *)(Data_03001e74 + 0x6b8);
    struct BattleEventQueue *queue = &runtime->queue;
    s32 i;

    for (i = 0; i < FIELD(queue, s32, 0x144); i++) {
        u8 opcode = queue->opcodes[i];
        if (opcode <= 13) switch (opcode) {
        case 13: Battle_SetRuntimeFlagBit0((struct BattleEventFlagWork *)queue, queue->operands[i]); break;
        case 12: BattleActor_DestroyTemporaryObject(queue->operands[i]); break;
        case 0: Func_08015120(queue->operands[i], 1); break;
        case 1: Func_08015120(queue->operands[i], 5); break;
        case 2: Func_08015120(queue->operands[i] & 0x1ff, 2); break;
        case 3: Func_08015120(queue->operands[i] & 0x3fff, 4); break;
        case 6: FIELD(Data_03001ee4, s32, 8) = 1; break;
        case 7: Func_08015118(); break;
        case 4:
            if ((s32)queue->operands[i] >= 0) Func_080151c8(queue->operands[i]);
            Func_080bb65c();
            Func_08015118();
            break;
        case 5:
            if ((s32)queue->operands[i] >= 0) Func_080151c8(queue->operands[i]);
            Func_08015118();
            break;
        case 8:
            if (runtime->pending_sound_id > 0) Func_080f9010(runtime->pending_sound_id);
            Func_080babdc(queue->operands[i], 0, 0);
            break;
        case 9:
        {
            u32 operand_offset = i * 4 + 64;
            u32 auxiliary = runtime->actor_auxiliary;
            Func_080c24f0(FIELD(queue, u32, operand_offset), auxiliary);
            Func_080bb588(FIELD(queue, u32, operand_offset));
            Func_080bace8(FIELD(queue, u32, operand_offset));
            break;
        }
        case 10: Func_08015130(Data_03001e74[65]); break;
        case 11:
            Func_080b78e4(queue->operands[i], (u32)GetBattleObjectSlot(queue->operands[i]));
            Func_080b7aac(queue->operands[i]);
            break;
        }
    }
    return Func_080bdfec();
}

#define BattleEv_Push Func_080bbabc

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
