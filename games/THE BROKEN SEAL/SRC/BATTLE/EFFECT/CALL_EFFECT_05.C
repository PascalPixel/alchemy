#include "TYPES.H"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_080999f0();
#define RunBattleEffect05 Func_080999f0

void BattleFx_CallEffect05(void)
{
    RunBattleEffect05();
}
