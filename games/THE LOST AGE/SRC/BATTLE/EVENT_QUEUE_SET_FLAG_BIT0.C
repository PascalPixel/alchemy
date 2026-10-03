#include "TYPES.H"
#include "BATTLE_RUNTIME.H"
#include "BATTLE_SESSION.H"
#include "OWNER_STATE.H"

s32 Object_Destroy(s32);
void Owner_UpdateRatioPairFar(struct BattleUnit *, s32);
struct BattleObjectSlot *GetBattleObjectSlot(s32 arg0);
s32 ActivateBattleObjectSlot(s32 arg0);
void BattleActor_RemoveFromLists(s32);

s32 BattleActor_DestroyTemporaryObject(s32 arg0)
{
    /* FAKEMATCH: the existing used word result crosses the void object-release
       veneer; the native cleanup epilogue returns its live r0 unchanged. */
    s32 result;
    struct BattleUnit *creature;
    struct BattleObjectSlot *runtime;

    creature = Owner_GetState(arg0);
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

struct BattleEventFlagWork {
    u8 unknown_00[0x16c];
    u32 flags;
};

void Battle_SetRuntimeFlagBit0(struct BattleEventFlagWork *runtime, u32 operand)
{
    runtime->flags |= 1;
}
