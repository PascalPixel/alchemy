#include "TYPES.H"

extern u32 gKeysHeld;
extern u16 BattleFx_CyclePatternWords[];

u16 BattleFx_GetCycledTableWord(void)
{
    return BattleFx_CyclePatternWords[(gKeysHeld >> 4) & 15];
}
