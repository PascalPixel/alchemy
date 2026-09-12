#include "scene.h"
#include "effect_step.h"
#include "types.h"
#include "global_cells.h"

/* effects/run_animation_mode_6_or_7_or.c */
/* effects/step/run_animation_mode_6_or_7_or_8.c */
void EffectStep_RunAnimationMode6Or7Or8(struct EffectStep *step)
{
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
