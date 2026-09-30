#include "EFFECT_STEP.H"

void BattlePres_RunBurstScene(struct EffectStep *, s32);

void EffectStep_RunAnimationMode3Or4Or5(struct EffectStep *step)
{
    if (step->variant == 0) {
        BattlePres_RunBurstScene(step, 3);
        return;
    }
    if (step->variant == 1) {
        BattlePres_RunBurstScene(step, 4);
        return;
    }
    BattlePres_RunBurstScene(step, 5);
}
