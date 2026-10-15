#include "types.h"
#include "effect_slot.h"

void EffectSlot_SetPosition(struct EffectSlot *effect, s32 x, s32 z)
{
    effect->targetX = EFFECT_NO_TARGET;
    effect->targetZ = EFFECT_NO_TARGET;
    effect->x = x;
    effect->z = z;
    effect->speed = 0;
}

void Animation_ApplyChildArgumentFar(void *, s32);

void EffectSlot_SetObjectMode(struct EffectSlot *effect, s32 mode)
{
    Animation_ApplyChildArgumentFar(effect->object, mode);
}

void EffectSlot_SetCallback(struct EffectSlot *effect, EffectCallback callback)
{
    effect->callback = callback;
    effect->callbackDelay = 0;
    /* FAKEMATCH: a one-pass block loads the state's zero before the halfword zero is built; stored straight the two rows trade places (differ 2). */
    do
    {
        effect->age = 0;
        effect->state = 0;
    } while (0);
}
