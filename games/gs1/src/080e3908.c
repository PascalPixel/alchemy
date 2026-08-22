#include "effect_step.h"

void Func_080e3908(struct EffectStep *step, s32 damping, s32 gravity)
{
    step->x = (s32)((u32)step->x + (u32)step->velocity_x);
    step->y = (s32)((u32)step->y + (u32)step->velocity_y);
    step->velocity_y =
        (s32)((u32)step->velocity_y + (u32)gravity);
    step->velocity_x =
        (s32)((u32)step->velocity_x * (u32)damping) / 64;
    step->velocity_y =
        (s32)((u32)step->velocity_y * (u32)damping) / 64;
}
