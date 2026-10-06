#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "BATTLE_PARTY.H"

#define BattleActor_SpawnObjectsForList ((s32 (*)(void*,s32))BattleActor_SpawnObjectsForList)


void BattleParty_ListAllUnitsAndSubmit(void)
{
    u8 local[28];
    BattleParty_ListActorIds(3, (u16 *)local);
    BattleActor_SpawnObjectsForList(local, 0);
}
