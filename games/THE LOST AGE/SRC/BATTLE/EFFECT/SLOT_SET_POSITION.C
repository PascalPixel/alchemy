#include "TYPES.H"

#define EFFECT_NO_TARGET ((s32)0x80000000)

struct EffectSlot {
    /* 0x00 */ void *object;
    /* 0x04 */ s32 x;
    /* 0x08 */ s32 z;
    /* 0x0c */ s32 target_x;
    /* 0x10 */ s32 target_z;
    /* 0x14 */ s32 origin_x;
    /* 0x18 */ s32 origin_z;
    /* 0x1c */ s32 speed;
};

void EffectSlot_SetPosition(struct EffectSlot *effect, s32 x, s32 z)
{
    effect->target_x = EFFECT_NO_TARGET;
    effect->target_z = EFFECT_NO_TARGET;
    effect->x = x;
    effect->z = z;
    effect->speed = 0;
}
