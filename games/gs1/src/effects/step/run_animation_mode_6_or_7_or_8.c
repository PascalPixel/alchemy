#include "effect_step.h"

void EffectStep_RunAnimationMode6Or7Or8(struct EffectStep *step) {
    if (step->variant == 0) {
        EffectStep_RunAnimation(step, 6);
        return;
    }
    if (step->variant == 1) {
        EffectStep_RunAnimation(step, 7);
        return;
    }
    EffectStep_RunAnimation(step, 8);
}
