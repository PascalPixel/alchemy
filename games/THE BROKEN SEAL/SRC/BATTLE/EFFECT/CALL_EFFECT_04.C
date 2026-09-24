#include "TYPES.H"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 RunBattleEffect04();

void BattleFx_CallEffect04(void)
{
    RunBattleEffect04();
}
