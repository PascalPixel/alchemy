#include "effect_step.h"

s32 Func_08005268(s32, struct EffectPosition *);
void Func_080e3994(s32, struct EffectPosition *);
void Func_080b50b0(s32, struct EffectPosition *);
void Func_080b50a8(s32, struct EffectPosition *);

void EffectStep_AdvanceWithGravity3D(struct EffectStep *step, s32 damping, s32 gravity)
{
    step->x = (s32)((u32)step->x + (u32)step->velocity_x);
    step->y = (s32)((u32)step->y + (u32)step->velocity_y);
    step->z = (s32)((u32)step->z + (u32)step->velocity_z);
    step->velocity_y = (s32)((u32)step->velocity_y + (u32)gravity);
    step->velocity_x = (s32)((u32)step->velocity_x * (u32)damping) / 64;
    step->velocity_y = (s32)((u32)step->velocity_y * (u32)damping) / 64;
    step->velocity_z = (s32)((u32)step->velocity_z * (u32)damping) / 64;
}

void EffectStep_AdvanceWithGravity2D(struct EffectStep *step, s32 damping, s32 gravity)
{
    step->x = (s32)((u32)step->x + (u32)step->velocity_x);
    step->y = (s32)((u32)step->y + (u32)step->velocity_y);
    step->velocity_y = (s32)((u32)step->velocity_y + (u32)gravity);
    step->velocity_x = (s32)((u32)step->velocity_x * (u32)damping) / 64;
    step->velocity_y = (s32)((u32)step->velocity_y * (u32)damping) / 64;
}

s32 EffectPosition_ApplyBaseAndYOffset(s32 id, struct EffectPosition *position)
{
    s32 result = Func_08005268(id, position);
    position->y = (s32)((u32)position->y - 0x10);
    return result;
}

void EffectPosition_ApplyAnimationAndYOffset(s32 id, struct EffectPosition *position)
{
    Func_080e3994(id, position);
    position->y = (s32)((u32)position->y - 0x10);
}

void EffectPosition_ApplyStepAndYOffset(s32 id, struct EffectPosition *position)
{
    Func_080b50b0(id, position);
    position->y = (s32)((u32)position->y - 0x10);
}

void EffectPosition_ApplyAlternateStepAndYOffset(s32 id, struct EffectPosition *position)
{
    Func_080b50a8(id, position);
    position->y = (s32)((u32)position->y - 0x10);
}
