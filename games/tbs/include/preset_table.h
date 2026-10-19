#ifndef GUARD_PRESET_TABLE_H
#define GUARD_PRESET_TABLE_H

/// @file
/// PresetValues, a row of preset values.

#include "types.h"

/// An enemy's element preset in gEnemy_ElementPresetTable: the element it
/// attacks with, its level in each element, and its base power and resistance
/// in each element as packed halfword pairs.
struct PresetValues {
  s32 first;
  u8 digits[4];
  s32 values[4];
};

#endif
