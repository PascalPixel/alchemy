#include "effect_0809b11c.h"

void Func_0809b804(struct EffectSlot *effect)
{
    if (effect->active != 0) {
        effect->age++;
        if (effect->callback_delay != 0)
            effect->callback_delay--;
        else if (effect->callback != 0)
            effect->callback(effect);
        if (effect->active != 0) {
            if (effect->update_motion != 0)
                Func_0809b8f4(effect);
            if (effect->render != 0)
                Func_0809b86c(effect);
        }
    }
}
