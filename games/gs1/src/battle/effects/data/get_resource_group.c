#include "types.h"

extern const u8 Data_0809f1a8[];

s8 BattleEffect_GetResourceGroup(s32 effect_index)
{
  u8 *entry;
  entry = (u8 *) ((effect_index * 8) + (s32)Data_0809f1a8);
  return (s8) (*((u8 *) (2 + entry)));
}
