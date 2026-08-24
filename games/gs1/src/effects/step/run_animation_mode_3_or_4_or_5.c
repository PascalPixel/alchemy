#include "effect_step.h"

void EffectStep_RunAnimationMode3Or4Or5(struct EffectStep *step) {
    if (step->variant == 0) {
        EffectStep_RunAnimation(step, 3);
        return;
    }
    if (step->variant == 1) {
        EffectStep_RunAnimation(step, 4);
        return;
    }
    EffectStep_RunAnimation(step, 5);
}
