#include "types.h"
/*
 * resource_382 owner at 0x02001754, 88 bytes.
 * Per-frame step for a projectile: advances x by its rate and mirrors it into
 * the shadow copy, then either follows the y rate or falls at a fixed rate
 * depending on the mode word at +100, and finally decays both rates.
 */
#define Overlay382_UpdateEffectMotionWithDamping Func_02001754
#define EffectMotion_GetHorizontalDecay Func_02003092
#define EffectMotion_GetVerticalDecay Func_020030a0

struct OverlayEffectMotion {
    u8 pad00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 pad14[28];
    s32 horizontal_rate;
    s32 vertical_rate;
    s32 shadow_x;
    s32 shadow_y;
    s32 shadow_z;
    u8 pad44[32];
    s16 mode;
};
s32 EffectMotion_GetHorizontalDecay();
s32 EffectMotion_GetVerticalDecay();

void Overlay382_UpdateEffectMotionWithDamping(struct OverlayEffectMotion *effect)
{
    s32 horizontal_rate;
    s32 vertical_rate;

    effect->x += effect->horizontal_rate;
    effect->shadow_x = effect->x;

    if (effect->mode != 0) {
        effect->y += effect->vertical_rate;
        effect->shadow_y = effect->y;
    } else {
        effect->z += effect->vertical_rate;
        effect->shadow_z = effect->z;
        effect->y += 1024;
        effect->shadow_y = effect->y;
    }

    horizontal_rate = effect->horizontal_rate;
    effect->horizontal_rate = horizontal_rate - EffectMotion_GetHorizontalDecay(horizontal_rate, 28);
    vertical_rate = effect->vertical_rate;
    effect->vertical_rate = vertical_rate - EffectMotion_GetVerticalDecay(vertical_rate, 28);
}
