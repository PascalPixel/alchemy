#ifndef ALCHEMY_EFFECT_STEP_H
#define ALCHEMY_EFFECT_STEP_H

#include "edition.h"
#include "types.h"
#include "layout_guard.h"

struct EffectStep {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 variant;
};

struct EffectPosition {
  s32 x;
  s32 y;
  s32 depth;
};

#if GAME_TLA
void EffectStep_AdvanceWithGravity3D(struct EffectStep *step, s32 damping,
                                     s32 gravity);
s32 EffectPosition_ApplyBaseAndYOffset(s32 *point,
                                       struct EffectPosition *position);
void EffectPosition_ApplyStepAndYOffset(s32 id,
                                        struct EffectPosition *position);
#endif

LAYOUT_OFFSET_GUARD(EffectStep_x_offset, struct EffectStep, x, 0x00);
LAYOUT_OFFSET_GUARD(EffectStep_y_offset, struct EffectStep, y, 0x04);
LAYOUT_OFFSET_GUARD(EffectStep_z_offset, struct EffectStep, z, 0x08);
LAYOUT_OFFSET_GUARD(EffectStep_velocity_x_offset, struct EffectStep, velocityX,
                    0x0c);
LAYOUT_OFFSET_GUARD(EffectStep_velocity_y_offset, struct EffectStep, velocityY,
                    0x10);
LAYOUT_OFFSET_GUARD(EffectStep_velocity_z_offset, struct EffectStep, velocityZ,
                    0x14);
LAYOUT_OFFSET_GUARD(EffectStep_variant_offset, struct EffectStep, variant,
                    0x18);
LAYOUT_SIZE_GUARD(EffectStep_size, struct EffectStep, 0x1c);
LAYOUT_SIZE_GUARD(EffectPosition_size, struct EffectPosition, 0x0c);

#endif
