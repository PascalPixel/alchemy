// Draft, differ 52 of 84: register choice and order. The native keeps the
// layer 2 table in r10 and the slot in r4, spilled to sp 0 across
// Func_080eab98, and adds the slot to the shape row before that call; this
// spills layer 2's table and keeps the slot in r10. Byte 3 is read once
// into a local, as in Func_080eabd0's draft.
// Raises the map again under the switch actor recorded in the
// event work's word at 0xcbc (by Func_080eace8 or Func_080eaa14), as
// Func_080eabd0 does for each listed actor: the cell's shape takes
// Func_080eab98's tag four half-units higher, the cell entry's byte 3 is
// saved in the actor's word at 76 before its raised bit is set, and layer
// 2's slot is blocked.
#include "types.h"
#include "actor.h"
#include "ram_buffer.h"

struct SwitchCell {
  u8 unk00[2];
  u8 block;
  u8 flags;
};

struct RestoreLayer {
  u8 unk00[0x138 - 0x108];
  struct SwitchCell *cells;
  u8 unk34[4];
};

struct RestoreMap {
  u8 unk000[0x108];
  struct RestoreLayer layers[3];
};

extern u8 gMapShapeGrid[];

s32 Func_080eab70(struct Actor *object);
u32 Func_080202f0(s32 terrain, s32 x, s32 z);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);
s32 Func_080eab98(s32 unused, s32 key, s32 value);

void Func_080eadfc(void) {
  struct Actor *target =
      *(struct Actor **)((u8 *)Ram_HeapSlots->eventWork + 0xcbc);

  if (target != NULL) {
    struct RestoreMap *map = (struct RestoreMap *)Ram_HeapSlots->mapWork;
    struct SwitchCell *cells = map->layers[target->terrainId].cells;
    struct SwitchCell *blocks = map->layers[2].cells;
    u8 *shapes = &gMapShapeGrid[((s32)cells - 0x02010000) >> 2];
    u32 key = Func_080202f0(target->terrainId, target->x, target->z);
    s32 level =
        Map_GetTerrainHeightFar(target->terrainId, target->x, target->z);
    s32 slot = Func_080eab70(target);
    s32 tag = Func_080eab98(target->terrainId, key, (level >> 19) + 4);
    u8 flags;

    flags = cells[slot].flags;
    target->unk4c = flags;
    cells[slot].flags = flags | 0x80;
    shapes[slot] = tag;
    blocks[slot].block = 0xff;
  }
}
