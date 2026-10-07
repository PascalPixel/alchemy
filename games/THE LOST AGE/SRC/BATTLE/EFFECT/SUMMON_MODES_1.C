#include "TYPES.H"
#include "BATTLE_EFFECT_WORK.H"

void Func_08181ed4(struct BattleEffectArgument *effect, s32 mode);

void Func_08181eb0(struct BattleEffectArgument *effect)
{
    Func_08181ed4(effect, 0);
}

void Func_08181ebc(struct BattleEffectArgument *effect)
{
    Func_08181ed4(effect, 2);
}

void Func_08181ec8(struct BattleEffectArgument *effect)
{
    Func_08181ed4(effect, 1);
}
