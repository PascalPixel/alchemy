#include "TYPES.H"
#include "BATTLE_EVENT.H"
#include "BATTLE_WORK.H"
extern u8 Data_03001ee4[];


#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 BattleActor_DestroyTemporaryObject(s32 arg0)
{
    s32 result;
    struct BattleEventActor *creature;
    struct BattleEventObjectSlot *runtime;

    creature = Owner_GetStateFar();
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
