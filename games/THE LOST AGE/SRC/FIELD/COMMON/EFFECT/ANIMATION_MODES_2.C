#include "EFFECT_STEP.H"

void BattlePres_RunBurstScene(struct EffectStep *, s32);

/* ⚓️'s second copy of EffectStep_RunAnimationMode6Or7Or8, after its new modes. */
void EffectStep_RunAnimationMode6Or7Or8B(struct EffectStep *step)
{
    if (step->variant == 0) {
        BattlePres_RunBurstScene(step, 6);
        return;
    }
    if (step->variant == 1) {
        BattlePres_RunBurstScene(step, 7);
        return;
    }
    BattlePres_RunBurstScene(step, 8);
}
