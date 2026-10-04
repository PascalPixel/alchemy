#include "TYPES.H"

extern const u8 Field_SceneTable[];

s8 BattleFx_GetResourceGroup(s32 effect_index)
{
  u8 *entry;
  entry = (u8 *)((effect_index * 8) + (s32)Field_SceneTable);
  return (s8)(*((u8 *)(2 + entry)));
}

/* The last halfword of a scene's row: packed attributes the field reads
   four bits at a time. */
s16 Field_GetSceneAttributes(s32 scene)
{
  u8 *entry;
  entry = (u8 *)((scene * 8) + (s32)Field_SceneTable);
  return *(s16 *)(6 + entry);
}
