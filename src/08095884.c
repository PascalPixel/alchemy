#include "effect_0809b11c.h"

void Func_08095884(void) {
    s32 var_r5;
    s32 var_r6;

    var_r5 = *(s32 *)0x03001F30 + 0x58;
    var_r6 = 0x17;
    do {
        var_r6 -= 1;
        Func_0809b804((struct EffectSlot *)var_r5);
        var_r5 += 0x48;
    } while (var_r6 >= 0);
}
