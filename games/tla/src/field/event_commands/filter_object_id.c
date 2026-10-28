/// @file
/// Battle effects: filter the effect's target object.

#include "types.h"

s32 BattleEffect_GetFlags();

/// Returns no target (-1) when the current effect definition's flags are
/// 0xff, else id.
/// id: target object ID.
s32 BattleEffect_FilterObjectIdByFlags(s32 id) {
  if (BattleEffect_GetFlags() == 0xff)
    return -1;
  return id;
}
