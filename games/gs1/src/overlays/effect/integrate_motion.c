#include "types.h"

/* Integrate position and sprite angle for one scene effect. The sibling
   overlays keep the same effect and sprite layout. */

struct Sprite {
    u8 pad00[0x1e];
    u16 angle;
};

struct Effect {
    u8 pad00[8];
    s32 position[3];
    u8 pad14[4];
    s32 accum18;
    /* The angle store below must not be hoisted above this write: the pair of
       volatile references is what keeps the sprite load after it. */
    volatile s32 accum1c;
    u8 pad20[0x10];
    s32 rate30;
    s32 rate34;
    u8 pad38[12];
    s32 velocity[3];
    struct Sprite *volatile sprite;
    u8 pad54[0x10];
    u16 step64;
};

#define IntegrateEffectMotion Func_02000104

void IntegrateEffectMotion(struct Effect *effect)
{
    effect->position[0] += effect->velocity[0];
    effect->position[1] += effect->velocity[1];
    effect->position[2] += effect->velocity[2];
    effect->accum18 += effect->rate30;
    effect->accum1c += effect->rate34;
    effect->sprite->angle += effect->step64;
}
