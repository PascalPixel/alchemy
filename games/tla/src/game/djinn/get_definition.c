#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "btlsmn.h"


const struct SummonDefinition *SummonDefinition_GetNear(u32 summon_id)
{
    if (summon_id > 28)
        return NULL;
    return &Summon_DefinitionTable[summon_id];
}
