#include "effect_step.h"

void Func_080e2974(struct EffectStep *, s32);

void EffectStep_RunAnimationMode6Or7Or8(struct EffectStep *step)
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

void EffectStep_RunAnimationMode0(struct EffectStep *step)
{
    Func_080e2974(step, 0);
}

void EffectStep_RunAnimationMode0Alternate(struct EffectStep *step)
{
    Func_080e2974(step, 0);
}

void EffectStep_RunAnimationMode1(struct EffectStep *step)
{
    Func_080e2974(step, 1);
}

void EffectStep_RunAnimationMode2(struct EffectStep *step)
{
    Func_080e2974(step, 2);
}

void EffectStep_RunAnimationMode3Or4Or5(struct EffectStep *step)
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
