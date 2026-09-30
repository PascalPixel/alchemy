#include "TYPES.H"

extern const u8 Field_SceneTable[];

s8 BattleFx_GetResourceGroup(s32 effect_index)
{
  u8 *entry;
  entry = (u8 *)((effect_index * 8) + (s32)Field_SceneTable);
  return (s8)(*((u8 *)(2 + entry)));
}
