#include "types.h"
#include "scene.h"

/* battle/effects/data/get_resource_group.c */
extern const u8 gRom[];

s8 BattleFx_GetResourceGroup(s32 effect_index)
{
  u8 *entry;
  entry = (u8 *)((effect_index * 8) + (s32)gRom);
  return (s8)(*((u8 *)(2 + entry)));
}

/* battle/effects/data/load_resource_group.c */
struct Work {
    u8 unknown_000[448];
    s16 index;
};

extern struct Work gCell;
extern u8 gRom[];
extern u8 gOv[];

void BattleFx_LoadResourceGroup(void)
{
    u8 *table = gRom;
    s16 index = gCell.index;
    s16 value = *(s16 *)(table + index * 8);
    SceneData_Apply(value, gOv);
}
