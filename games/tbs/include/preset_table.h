#ifndef GUARD_PRESET_TABLE_H
#define GUARD_PRESET_TABLE_H

/// @file
/// PresetValues, a row of preset values.

#include "types.h"

struct PresetValues {
  s32 first;
  u8 digits[4];
  s32 values[4];
};

#endif
