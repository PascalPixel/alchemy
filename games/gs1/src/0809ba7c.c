#include "effect_0809b11c.h"

void Func_0809ba7c(struct EffectSlot *effect, EffectCallback callback) {
    effect->callback = callback;
    effect->callback_delay = 0;
    effect->age = 0;
    effect->state = 0;
}
