#include "types.h"

void Func_080dbb98(void);

void BattleEffect_RunNoEffectFrames(s32 arg0) {
    s32 var_r6;

    var_r6 = 0;
    if (arg0 != 0) {
        do {
            var_r6 += 1;
            Func_080dbb98();
        } while (var_r6 != arg0);
    }
}
