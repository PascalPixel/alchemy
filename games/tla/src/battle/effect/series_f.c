#include "types.h"

struct BattleEffectArgument;
void BattleFx_RunSparkGroups(struct BattleEffectArgument *effect, s32 mode);

void Func_08149b94(struct BattleEffectArgument *effect)
{
    BattleFx_RunSparkGroups(effect, 0);
}

void Func_08149ba0(struct BattleEffectArgument *effect)
{
    BattleFx_RunSparkGroups(effect, 1);
}
