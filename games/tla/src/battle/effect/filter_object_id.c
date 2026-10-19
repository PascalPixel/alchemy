/// @file
/// Battle effects: filter the effect's target object.

#include "types.h"

s32 BattleFx_GetFlags();

/// Return no target (-1) when the current effect definition's flags are 0xff.
/// id: target object ID.
s32 BattleFx_FilterObjectIdByFlags(s32 id) {
  if (BattleFx_GetFlags() == 0xff)
    return -1;
  return id;
}
