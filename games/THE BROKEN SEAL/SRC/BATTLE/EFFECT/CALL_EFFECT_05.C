#include "TYPES.H"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 RunBattleEffect05();

void BattleFx_CallEffect05(void)
{
    RunBattleEffect05();
}
