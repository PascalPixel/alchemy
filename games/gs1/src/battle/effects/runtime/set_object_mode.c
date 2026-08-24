#include "effect_0809b11c.h"

void Object_InitializeMode(void *, s32);

void EffectSlot_SetObjectMode(struct EffectSlot *effect, s32 mode) {
    Object_InitializeMode(effect->object, mode);
}
