#include "types.h"

struct BattleEffectArgument;
void Func_08144b14(struct BattleEffectArgument *effect, s32 mode);

void Func_08144af0(struct BattleEffectArgument *effect)
{
    Func_08144b14(effect, 0);
}

void Func_08144afc(struct BattleEffectArgument *effect)
{
    Func_08144b14(effect, 1);
}

void Func_08144b08(struct BattleEffectArgument *effect)
{
    Func_08144b14(effect, 2);
}
