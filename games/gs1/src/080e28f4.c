#include "effect_step.h"

void Func_080e28f4(struct EffectStep *step)
{
    if (step->variant == 0) {
        Func_080e2974(step, 6);
        return;
    }
    if (step->variant == 1) {
        Func_080e2974(step, 7);
        return;
    }
    Func_080e2974(step, 8);
}
