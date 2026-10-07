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

#define UnitCalcFar ((void (*)(u16))UnitCalcFar)


void BattleUnit_ResetGuardLevels(void)
{
    u16 ids[14];
    s32 count;
    s32 index;

    count = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, (u16 *)ids);
    for (index = 0; index < count; index++) {
        struct BtlUnit *actor;

        actor = UnitGet(ids[index]);
        actor->guard_lv = 0;
        UnitCalcFar(ids[index]);
    }
}
