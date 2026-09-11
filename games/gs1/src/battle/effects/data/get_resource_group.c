#include "types.h"
#include "scene.h"
#include "abi/battle/effects/data/get_resource_group.h"

extern const u8 gRom[];

s8 BattleFx_GetResourceGroup(s32 effect_index)
{
  u8 *entry;
  entry = (u8 *)((effect_index * 8) + (s32)gRom);
  return (s8)(*((u8 *)(2 + entry)));
}
