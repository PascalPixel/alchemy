#include "GLOBAL.H"
#include "TYPES.H"


void BattlePlacement_GetStepPair(s32 index, s32 *x, s32 *y)
{
    index *= 2;
    *x = BattlePlacement_StepPairs[index];
    *y = BattlePlacement_StepPairs[index + 1];
}
