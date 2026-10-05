#include "TYPES.H"
#include "EFFECT_SLOT.H"

void EffectSlot_UpdateMotion(struct EffectSlot *effect);
void BattleFx_DrawScaledObject(struct EffectSlot *effect);

/* One frame of an effect slot: its age, its delayed callback, then its
   motion and drawing while it stays active. */
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
                EffectSlot_UpdateMotion(effect);
            if (effect->render != 0)
                BattleFx_DrawScaledObject(effect);
        }
    }
}
