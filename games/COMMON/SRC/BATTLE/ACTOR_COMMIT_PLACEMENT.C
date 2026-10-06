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


void BattleActor_CommitPlacement(void)
{
    u8 actor_slots[28];

    BattleParty_ListActorIds(3, (u16 *)actor_slots);
    BattleActor_SpawnObjectsForList(actor_slots, 1);
}
