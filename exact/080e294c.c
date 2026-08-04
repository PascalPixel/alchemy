#include "effect_step.h"

void Func_080e294c(struct EffectStep *step)
{
    if (step->variant == 0) {
        Func_080e2974(step, 3);
        return;
    }
    if (step->variant == 1) {
        Func_080e2974(step, 4);
        return;
    }
    Func_080e2974(step, 5);
}
