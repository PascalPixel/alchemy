#include "EFFECT_STEP.H"

s32 Render_ProjectPoint(s32 *, s32 *);
void BattleUnit_ProjectToScreen(s32, struct EffectPosition *);
void BattleMotion_ProjectScaledPositionFar(s32, struct EffectPosition *);
void BattleMotion_ProjectPositionFar(s32, struct EffectPosition *);

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

s32 EffectPosition_ApplyBaseAndYOffset(s32 *point, struct EffectPosition *position)
{
    s32 result = Render_ProjectPoint(point, &position->x);
    position->y = (s32)((u32)position->y - 0x10);
    return result;
}

void EffectPosition_ApplyAnimationAndYOffset(s32 id, struct EffectPosition *position)
{
    BattleUnit_ProjectToScreen(id, position);
    position->y = (s32)((u32)position->y - 0x10);
}

void EffectPosition_ApplyStepAndYOffset(s32 id, struct EffectPosition *position)
{
    BattleMotion_ProjectScaledPositionFar(id, position);
    position->y = (s32)((u32)position->y - 0x10);
}

void EffectPosition_ApplyAlternateStepAndYOffset(s32 id, struct EffectPosition *position)
{
    BattleMotion_ProjectPositionFar(id, position);
    position->y = (s32)((u32)position->y - 0x10);
}
