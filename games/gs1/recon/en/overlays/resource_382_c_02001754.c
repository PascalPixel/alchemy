#include "types.h"
/*
 * resource_382 owner at 0x02001754, 88 bytes.
 * Per-frame step for a projectile: advances x by its rate and mirrors it into
 * the shadow copy, then either follows the y rate or falls at a fixed rate
 * depending on the mode word at +100, and finally decays both rates.
 */
struct Effect_382 {
    u8 pad00[8];
    s32 f8;                     /* +8  */
    s32 f12;                    /* +12 */
    s32 f16;                    /* +16 */
    u8 pad14[28];
    s32 f48;                    /* +48 */
    s32 f52;                    /* +52 */
    s32 f56;                    /* +56 */
    s32 f60;                    /* +60 */
    s32 f64;                    /* +64 */
    u8 pad44[32];
    s16 f100;                   /* +100 */
};
s32 Func_02003092();
s32 Func_020030a0();
void Func_02001754(struct Effect_382 *effect)
{
    s32 step;
    s32 rate;

    effect->f8 += effect->f48;
    effect->f56 = effect->f8;

    if (effect->f100 != 0) {
        step = effect->f52;
    } else {
        effect->f16 += effect->f52;
        effect->f64 = effect->f16;
        step = 1024;
    }
    effect->f12 += step;
    effect->f60 = effect->f12;

    rate = effect->f48;
    effect->f48 = rate - Func_02003092(rate, 28);
    rate = effect->f52;
    effect->f52 = rate - Func_020030a0(rate, 28);
}
