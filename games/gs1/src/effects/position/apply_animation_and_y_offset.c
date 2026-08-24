#include "effect_step.h"

void EffectPosition_ApplyAnimation(s32, struct EffectPosition *);

void EffectPosition_ApplyAnimationAndYOffset(s32 arg0, struct EffectPosition *position)
{
    EffectPosition_ApplyAnimation(arg0, position);
    position->y = (s32)((u32)position->y - 0x10);
}
