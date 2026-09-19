#include "TYPES.H"

extern const u8 Data_080f17a8[];

s8 BattleFx_GetResourceGroup(s32 effect_index)
{
  u8 *entry;
  entry = (u8 *)((effect_index * 8) + (s32)Data_080f17a8);
  return (s8)(*((u8 *)(2 + entry)));
}
