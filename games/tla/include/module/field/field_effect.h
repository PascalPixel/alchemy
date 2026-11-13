#ifndef ALCHEMY_MODULE_FIELD_FIELD_EFFECT_H
#define ALCHEMY_MODULE_FIELD_FIELD_EFFECT_H

/// @file
/// What games/tla/src/field/field_effect/ declares for the files that use it,
/// written once.

#include "types.h"

union FieldObject;

extern const u32 gData_080f3954[8];

void Effect_AdvanceMotion(union FieldObject *object);

void Effect_AdvanceMotionAtLeaderPriority(union FieldObject *object);

#endif
