#ifndef ALCHEMY_FIELD_FIELD_EFFECT_H
#define ALCHEMY_FIELD_FIELD_EFFECT_H

/// @file
/// The tints a field Psynergy washes the screen in, and the motion updates of
/// its effect actors.

#include "types.h"

union FieldObject;

/// The eight tints a field Psynergy washes the screen in (BGR555, fxdata.c).
extern const u32 gFieldEffect_TargetColors[8];

void FieldEffect_AdvanceMotion(union FieldObject *object);

void Effect_AdvanceMotionAtLeaderPriority(union FieldObject *object);

#endif
