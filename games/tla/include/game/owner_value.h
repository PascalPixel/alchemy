#ifndef ALCHEMY_OWNER_VALUE_H
#define ALCHEMY_OWNER_VALUE_H

#include "types.h"
#include "edition.h"

/// Adds delta to a unit's HP, kept between 0 and its maximum, updates its
/// gauges and returns the new HP.
s32 Unit_AddHp(s32 owner, s32 delta);
/// Adds `delta` to the unit's HP, kept between 0 and its maximum, updates its
/// gauges and returns the new HP.
s32 Unit_AddHpFar(s32 owner, s32 delta);

#endif
