#include "scene.h"
#include "abi/battle/effects/runtime/upd/update_slot.h"
#include "effect_0809b11c.h"

void EffectSlot_Update(struct EffectSlot *effect)
{
    if (effect->active != 0) {
        effect->age++;
        if (effect->callback_delay != 0)
            effect->callback_delay--;
        else if (effect->callback != 0)
            effect->callback(effect);
        if (effect->active != 0) {
            if (effect->update_motion != 0)
                Battle_Run(effect);
            if (effect->render != 0)
                Battle_Run2(effect);
        }
    }
}
