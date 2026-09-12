#include "types.h"
#include "effect_0809b11c.h"

/* battle/effects/runtime/update_slot.c */
void Battle_Run(struct EffectSlot *effect);
void Battle_unk2_4(struct EffectSlot *effect);

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
                Battle_unk2_4(effect);
        }
    }
}
