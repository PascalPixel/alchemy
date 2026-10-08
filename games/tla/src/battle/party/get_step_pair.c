#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"


void BattlePlacement_GetStepPair(s32 index, s32 *x, s32 *y)
{
    index *= 2;
    *x = BattlePlacement_StepPairs[index];
    *y = BattlePlacement_StepPairs[index + 1];
}
