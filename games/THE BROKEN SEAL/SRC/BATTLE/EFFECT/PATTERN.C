#include "GLOBAL.H"
#include "TYPES.H"

extern u32 gKeysHeld;

u16 BattleFx_GetCycledTableWord(void)
{
    return BattleFx_CyclePatternWords[(gKeysHeld >> 4) & 15];
}
