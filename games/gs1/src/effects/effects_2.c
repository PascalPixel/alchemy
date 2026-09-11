#include "scene.h"
#include "effect_step.h"
#include "types.h"

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

/* effects/step/run_animation_mode_0.c */
void EffectStep_RunAnimationMode0(struct EffectStep *step)
{
    EffectStep_RunAnimation(step, 0);
}

/* effects/step/run_animation_mode_0_duplicate.c */
void EffectStep_RunAnimationMode0Duplicate(struct EffectStep *step)
{
    EffectStep_RunAnimation(step, 0);
}

/* effects/step/run_animation_mode_1.c */
void EffectStep_RunAnimationMode1(struct EffectStep *step)
{
    EffectStep_RunAnimation(step, 1);
}

/* effects/step/run_animation_mode_2.c */
void EffectStep_RunAnimationMode2(struct EffectStep *step)
{
    EffectStep_RunAnimation(step, 2);
}

/* effects/step/run_animation_mode_3_or_4_or_5.c */
void EffectStep_RunAnimationMode3Or4Or5(struct EffectStep *step)
{
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

/* effects/physics/advance_with_gravity_3d.c */
void EffectStep_AdvanceWithGravity3D(struct EffectStep *step, s32 damping, s32 gravity)
{
    step->x = (s32)((u32)step->x + (u32)step->velocity_x);
    step->y = (s32)((u32)step->y + (u32)step->velocity_y);
    step->z = (s32)((u32)step->z + (u32)step->velocity_z);
    step->velocity_y =
        (s32)((u32)step->velocity_y + (u32)gravity);
    step->velocity_x =
        (s32)((u32)step->velocity_x *(u32)damping) / 64;
    step->velocity_y =
        (s32)((u32)step->velocity_y *(u32)damping) / 64;
    step->velocity_z =
        (s32)((u32)step->velocity_z *(u32)damping) / 64;
}

/* effects/physics/advance_with_gravity_2d.c */
void EffectStep_AdvanceWithGravity2D(struct EffectStep *step, s32 damping, s32 gravity)
{
    step->x = (s32)((u32)step->x + (u32)step->velocity_x);
    step->y = (s32)((u32)step->y + (u32)step->velocity_y);
    step->velocity_y =
        (s32)((u32)step->velocity_y + (u32)gravity);
    step->velocity_x =
        (s32)((u32)step->velocity_x *(u32)damping) / 64;
    step->velocity_y =
        (s32)((u32)step->velocity_y *(u32)damping) / 64;
}

/* effects/position/apply_base_and_y_offset.c */
s32 EffectPosition_ApplyBaseAndYOffset(s32 arg0, void *arg1)
{
    Sys_Check();
    FIELD_AT_OFFSET(arg1, s32 *, 4) = (s32)(FIELD_AT_OFFSET(arg1, s32 *, 4) - 0x10);
}

/* effects/position/apply_animation_and_y_offset.c */
void EffectPosition_ApplyAnimation(s32, struct EffectPosition *);

void EffectPosition_ApplyAnimationAndYOffset(s32 arg0, struct EffectPosition *position)
{
    EffectPosition_ApplyAnimation(arg0, position);
    position->y = (s32)((u32)position->y - 0x10);
}

/* effects/position/apply_step_and_y_offset.c */
void EffectPosition_ApplyStepAndYOffset(s32 arg0, struct EffectPosition *position)
{
    Sys_Apply(arg0, position);
    position->y = (s32)((u32)position->y - 0x10);
}

/* effects/position/apply_alternate_step_and_y_offset.c */
void EffectPosition_ApplyAlternateStepAndYOffset(s32 arg0, struct EffectPosition *position)
{
    Sys_Apply(arg0, position);
    position->y = (s32)((u32)position->y - 0x10);
}
