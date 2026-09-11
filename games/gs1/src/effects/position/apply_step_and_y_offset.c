#include "scene.h"
#include "abi/effects/position/apply_step_and_y_offset.h"
#include "effect_step.h"

void EffectPosition_ApplyStepAndYOffset(s32 arg0, struct EffectPosition *position)
{
    Sys_Apply(arg0, position);
    position->y = (s32)((u32)position->y - 0x10);
}
