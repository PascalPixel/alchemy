#include "TYPES.H"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_08098cd8();
#define RunBattleEffect04 Func_08098cd8

void BattleFx_CallEffect04(void)
{
    RunBattleEffect04();
}
