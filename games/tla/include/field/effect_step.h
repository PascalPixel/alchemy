#ifndef ALCHEMY_EFFECT_STEP_H
#define ALCHEMY_EFFECT_STEP_H

#include "edition.h"
#include "types.h"
#include "static_assert.h"

struct EffectStep {
#ifdef UBFIX
  union {
    struct {
      s32 x;
      s32 y;
      s32 z;
    };
    s32 coordinates[3];
  };
#else
  s32 x;
  s32 y;
  s32 z;
#endif
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 variant;
};

/// UB: indexing from &x crosses the step's separate scalar members.
/// UBFIX supplies a real coordinate array without including motion state.
static inline const s32 *EffectStep_Coordinates(const struct EffectStep *step) {
#ifdef UBFIX
  return step->coordinates;
#else
  return &step->x;
#endif
}

struct EffectPosition {
#ifdef UBFIX
  /// Projection writes three coordinates; effects read their named components.
  union {
    struct {
      s32 x;
      s32 y;
      s32 depth;
    };
    s32 coordinates[3];
  };
#else
  s32 x;
  s32 y;
  s32 depth;
#endif
};

/// UB: indexing from &x crosses separate scalar members in the legacy record.
/// UBFIX provides a real array while keeping the effects' named coordinates.
/// An always-on union changes native alias analysis in the effect/window code.
static inline s32 *EffectPosition_Coordinates(struct EffectPosition *position) {
#ifdef UBFIX
  return position->coordinates;
#else
  return &position->x;
#endif
}

void EffectStep_AdvanceWithGravity3D(struct EffectStep *step, s32 damping,
                                     s32 gravity);
s32 EffectPosition_ProjectPoint(const void *point,
                                struct EffectPosition *position);
void EffectPosition_ProjectScaledBattler(s32 id,
                                         struct EffectPosition *position);

STATIC_ASSERT(offsetof(struct EffectStep, x) == 0x00);
STATIC_ASSERT(offsetof(struct EffectStep, y) == 0x04);
STATIC_ASSERT(offsetof(struct EffectStep, z) == 0x08);
STATIC_ASSERT(offsetof(struct EffectStep, velocityX) == 0x0c);
STATIC_ASSERT(offsetof(struct EffectStep, velocityY) == 0x10);
STATIC_ASSERT(offsetof(struct EffectStep, velocityZ) == 0x14);
STATIC_ASSERT(offsetof(struct EffectStep, variant) == 0x18);
STATIC_ASSERT(sizeof(struct EffectStep) == 0x1c);
STATIC_ASSERT(sizeof(struct EffectPosition) == 0x0c);

#endif
