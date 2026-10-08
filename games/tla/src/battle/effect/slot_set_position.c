#include "types.h"
#include "effect_slot.h"

void EffectSlot_SetPosition(struct EffectSlot *effect, s32 x, s32 z)
{
    effect->target_x = EFFECT_NO_TARGET;
    effect->target_z = EFFECT_NO_TARGET;
    effect->x = x;
    effect->z = z;
    effect->speed = 0;
}

void Animation_ApplyChildArgumentFar(void *, s32);

void EffectSlot_SetObjectMode(struct EffectSlot *effect, s32 mode)
{
    Animation_ApplyChildArgumentFar(effect->object, mode);
}
