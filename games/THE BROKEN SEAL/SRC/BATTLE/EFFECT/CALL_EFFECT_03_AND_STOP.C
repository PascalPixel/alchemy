#include "TYPES.H"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_080994d0();
#define RunBattleEffect03 Func_080994d0

void BattleFx_CallEffect03AndStop(void)
{
    RunBattleEffect03();
    EffectRuntime_StopCurrentObject();
}
