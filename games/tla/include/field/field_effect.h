#ifndef ALCHEMY_FIELD_FIELD_EFFECT_H
#define ALCHEMY_FIELD_FIELD_EFFECT_H

/// @file
/// The tints a field Psynergy washes the screen in, and the motion updates of
/// its effect actors.

#include "types.h"

union FieldObject;

/// The eight tints a field Psynergy washes the screen in, by their index in
/// gFieldEffect_TargetColors and FLAG_PSYNERGY_TINT.
enum PsynergyTint {
  PSYNERGY_TINT_BLUE = 0,
  PSYNERGY_TINT_GREEN = 1,
  PSYNERGY_TINT_RED = 2,
  PSYNERGY_TINT_CYAN = 3,
  PSYNERGY_TINT_MAGENTA = 4,
  PSYNERGY_TINT_YELLOW = 5,
  PSYNERGY_TINT_DARK_GREY = 6,
  PSYNERGY_TINT_GREY = 7,
  PSYNERGY_TINT_COUNT = 8
};

/// Each tint's colour (field_effect_data.c).
extern const u32 gFieldEffect_TargetColors[PSYNERGY_TINT_COUNT];

void FieldEffect_AdvanceMotion(union FieldObject *object);

void FieldEffect_AdvanceMotionAtLeaderPriority(union FieldObject *object);

#endif
