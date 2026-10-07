#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "BATTLE_PARTY.H"
#include "BATTLE_UNIT.H"
#include "OWNER_STATE.H"

#define Owner_RecalculateStatsFar ((void (*)(u16))Owner_RecalculateStatsFar)


void BattleUnit_ResetGuardLevels(void)
{
    u16 ids[14];
    s32 count;
    s32 index;

    count = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, ids);
    for (index = 0; index < count; index++) {
        struct BattleUnit *actor;

        actor = Owner_GetState(ids[index]);
        actor->guard_level = 0;
        Owner_RecalculateStatsFar(ids[index]);
    }
}
