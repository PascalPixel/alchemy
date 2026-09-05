#include "types.h"

#define BattleEffectA2 Func_080d8960
#define BattleEffectA Func_080d89ac
s32 BattleEffectA(s32, s32);

void BattleEffectA2(s32 effect) {
    BattleEffectA(effect, 2);
}
