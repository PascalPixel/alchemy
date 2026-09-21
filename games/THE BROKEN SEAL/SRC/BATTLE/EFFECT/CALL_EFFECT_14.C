#include "TYPES.H"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_0809a8c4();
#define RunBattleEffect14 Func_0809a8c4

void BattleFx_CallEffect14(void)
{
    RunBattleEffect14();
}
