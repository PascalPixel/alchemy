#include "BATTLE.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "BATTLE_SUMMON.H"


const struct SummonDefinition *SummonDefinition_GetNear(u32 summon_id)
{
    if (summon_id > 28)
        return NULL;
    return &Summon_DefinitionTable[summon_id];
}
