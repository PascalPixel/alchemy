#ifndef ALCHEMY_UNIT_HP_H
#define ALCHEMY_UNIT_HP_H

/// @file
/// Adding to or taking from a unit's HP.

#include "types.h"
#include "edition.h"

/// Adds delta to a unit's HP, kept between 0 and its maximum, updates its
/// gauges and returns the new HP.
s32 Unit_AddHp(s32 owner, s32 delta);
/// Unit_AddHp, for an overlay.
s32 Unit_AddHpFar(s32 owner, s32 delta);

#endif
