#include "types.h"
#include "scene.h"
#include "abi/battle/effects/data/load_resource_group.h"

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
