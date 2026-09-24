#include "TYPES.H"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 RunBattleEffect03();

void BattleFx_CallEffect03AndStop(void)
{
    RunBattleEffect03();
    EffectRuntime_StopCurrentObject();
}
