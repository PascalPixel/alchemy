#ifndef ALCHEMY_EFFECT_STEP_H
#define ALCHEMY_EFFECT_STEP_H

#include "types.h"

/* エフェクト1段分の座標・移動量。演算は32bitで循環する。 */
struct EffectStep {
    s32 x;
    s32 y;
    s32 z;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 variant;
};

/* 描画補助へ渡す二次元座標。 */
struct EffectPosition {
    s32 x;
    s32 y;
};

s32 Func_080e2974(struct EffectStep *step, s32 animation);

#define EFFECT_STEP_OFFSET(field) \
    ((u32)&(((struct EffectStep *)0)->field))

typedef char EffectStep_x_offset[
    EFFECT_STEP_OFFSET(x) == 0x00 ? 1 : -1
];
typedef char EffectStep_y_offset[
    EFFECT_STEP_OFFSET(y) == 0x04 ? 1 : -1
];
typedef char EffectStep_z_offset[
    EFFECT_STEP_OFFSET(z) == 0x08 ? 1 : -1
];
typedef char EffectStep_velocity_x_offset[
    EFFECT_STEP_OFFSET(velocity_x) == 0x0c ? 1 : -1
];
typedef char EffectStep_velocity_y_offset[
    EFFECT_STEP_OFFSET(velocity_y) == 0x10 ? 1 : -1
];
typedef char EffectStep_velocity_z_offset[
    EFFECT_STEP_OFFSET(velocity_z) == 0x14 ? 1 : -1
];
typedef char EffectStep_variant_offset[
    EFFECT_STEP_OFFSET(variant) == 0x18 ? 1 : -1
];
typedef char EffectStep_size[
    sizeof(struct EffectStep) == 0x1c ? 1 : -1
];
typedef char EffectPosition_size[
    sizeof(struct EffectPosition) == 0x08 ? 1 : -1
];

#undef EFFECT_STEP_OFFSET

#endif
