#include "TYPES.H"
#include "BATTLE_PARTY.H"
#include "BATTLE_UNIT.H"
#include "OWNER_STATE.H"

void Owner_RecalculateStatsFar(u16 id);

void BattleUnit_ResetGuardLevels(void)
{
    u16 ids[14];
    s32 count;
    s32 index;

    count = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, (u16 *)ids);
    for (index = 0; index < count; index++) {
        struct BattleUnit *actor;

        actor = Owner_GetState(ids[index]);
        actor->guard_level = 0;
        Owner_RecalculateStatsFar(ids[index]);
    }
}
