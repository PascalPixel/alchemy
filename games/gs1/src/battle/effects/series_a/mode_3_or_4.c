#include "types.h"

#define BattleEffectA Func_080d89ac
#define BattleEffectA3Or4 Func_080d896c

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 BattleEffectA(void *, s32);

void BattleEffectA3Or4(void *effect) {
    if (M2C_FIELD(effect, s32 *, 0x18) == 0) {
        BattleEffectA(effect, 3);
        return;
    }
    BattleEffectA(effect, 4);
}
