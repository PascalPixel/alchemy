#include "TYPES.H"
#include "OBJDISP.H"
#include "BATTLE_RUNTIME.H"
#include "BATTLE_SESSION.H"
#include "OWNER_STATE.H"

void Object_Destroy(struct DispatchObject *object);
void Owner_UpdateRatioPairFar(struct BattleUnit *, s32);
struct BattleObjectSlot *GetBattleObjectSlot(s32 arg0);
s32 ActivateBattleObjectSlot(s32 arg0);
s32 BattleActor_RemoveFromLists(s32);

/* Attempt: a void owner matches TLA; TBS differs only at the 64-byte
 * helper epilogue, changing pop r1 / bx r1 to pop r0 / bx r0. */
s32 BattleActor_DestroyTemporaryObject(s32 arg0)
{
    /* FAKEMATCH: the shared discard-only result type preserves TBS epilogue
       register choice. No result is taken from the void destructor or returned. */
    struct BattleUnit *creature;
    struct BattleObjectSlot *runtime;

    creature = Owner_GetState(arg0);
    if (creature->status_12a == 1) {
        Owner_UpdateRatioPairFar(creature, 0);
        BattleActor_RemoveFromLists(arg0);
        ActivateBattleObjectSlot(arg0);
        runtime = GetBattleObjectSlot(arg0);
        Object_Destroy((struct DispatchObject *)runtime->object);
        runtime->object = 0;
        runtime->active = 0;
    }
}

struct BattleEventFlagWork {
    u8 unknown_00[0x16c];
    u32 flags;
};

void Battle_SetRuntimeFlagBit0(struct BattleEventFlagWork *runtime, u32 operand)
{
    runtime->flags |= 1;
}
