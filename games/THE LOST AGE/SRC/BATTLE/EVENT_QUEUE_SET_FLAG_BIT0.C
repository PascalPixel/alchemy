#include "TYPES.H"
#include "OWNER_STATE.H"

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
s32 Owner_UpdateRatioPairFar(void *, s32);
struct BattleEventObjectSlot *GetBattleObjectSlot(s32 arg0);
s32 ActivateBattleObjectSlot(s32 arg0);
s32 BattleActor_RemoveFromLists(s32);

s32 BattleActor_DestroyTemporaryObject(s32 arg0)
{
    s32 result;
    struct BattleEventActor *creature;
    struct BattleEventObjectSlot *runtime;

    creature = Owner_GetState(arg0);
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
    u8 unknown_00[0x16c];
    u32 flags;
};

void Battle_SetRuntimeFlagBit0(struct BattleEventFlagWork *runtime, u32 operand)
{
    runtime->flags |= 1;
}
