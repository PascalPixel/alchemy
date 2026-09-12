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
    FunctionHead_080b50b0(arg0, position);
    position->y = (s32)((u32)position->y - 0x10);
}

/* effects/position/apply_alternate_step_and_y_offset.c */
void EffectPosition_ApplyAlternateStepAndYOffset(s32 arg0, struct EffectPosition *position)
{
    FunctionHead_080b50a8(arg0, position);
    position->y = (s32)((u32)position->y - 0x10);
}

/* object/group/probe_keys_when_field24_high.c */
/* object/group/probe_keys_when_field24_high.c */
void ObjectGroup_ProbeKeysWhenField24High(void)
{
    u8 *state = *(u8 **)ADDR_03001EEC;
    u8 *object = *(u8 **)(state + 0x7828);

    if (*(s16 *)(object + 0x24) > 0x7f)
        (void)*(volatile s32 *)ADDR_03001AE8;
}

/* battle/effects/set/dispatch_by_id_range.c */
void *Battle_Run(s32);
s32 Runtime_AllocateHeapBlock(s32 arg0, s32 arg1);
void *Runtime_AllocateBlock(s32 arg0, s32 arg1);
void *BattlePres_RunBeamScene(s32 *);
void *BattlePres_RunRingAndSparkScene(s32 *);
void *FunctionHead_080e47b8(s32 *);
void BattleFx_DispatchByIdRange(s32 *arg0)
{
  s32 no;
  s32 tmp;
  tmp = (tmp = 0x60E);
  Runtime_AllocateBlock(0x29, tmp);
  Runtime_AllocateHeapBlock(0x27, 0x782C);
  Runtime_AllocateHeapBlock(0x28, 0x4000);
  tmp = *arg0;
  no = tmp;
  tmp = no - 0x64;
  if (((u32)tmp) <= 0x23U)
  {
    FunctionHead_080e47b8(arg0);
  } else
    if (no > 0xC7)
  {
    BattlePres_RunRingAndSparkScene(arg0);
  } else
  {
    BattlePres_RunBeamScene(arg0);
  }
  Battle_Run(0x28);
  Battle_Run(0x27);
  Battle_Run(0x29);
}
