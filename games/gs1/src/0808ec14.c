#include "effect_runtime.h"

s32 Func_0808ec14(s32 object) {
    s32 index;
    s32 found;
    struct EffectRuntimeSlot *slot;

    slot = EFFECT_RUNTIME->slots;
    found = -1;
    index = 0;
    if (slot->active == object) {
        found = 0;
    } else {
loop_2:
        index += 1;
        slot++;
        if (index <= 9) {
            if (slot->active == object) {
                found = index;
            } else {
                goto loop_2;
            }
        }
    }
    return found;
}
