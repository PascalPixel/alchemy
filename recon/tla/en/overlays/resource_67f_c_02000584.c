// Draft, differ 92 of 150: the variant of Func_020001d0 that first gathers
// people 8 to 10 on the leader and copies person 9's look onto each pillar
// whose second flag is set. The same function is overlay 68F's Func_020025e0
// and 691's Func_020030bc.
// Measured against its copies: overlay 68F's Func_020025e0 (differ 92 of 150),
// overlay 691's Func_020030bc (differ 92 of 150).
#include "types.h"
#include "actor.h"
#include "adventure.h"
#include "ram_buffer.h"
#include "map.h"

extern u8 gMapShapeGrid[];

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void WaitFrames(s32 frames);
void Func_020014a8(s32 id, s32 target);
void Func_0200056c(s32 id);
void Actor_SetModeById(u32 id, s32 mode);
void Func_020014a0(s32 id, s32 x, s32 z);
s32 Func_02001508(struct Actor *object);
s32 Func_02001478(s32 layer, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_020014c0(s32 id);
s32 Func_02001528(s32 layer, s32 shape, s32 height);
void ObjectVisual_CopyAttributes(u32 targetId, u32 sourceId);

void Func_02000584(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;

  Func_020014a8(8, gAdventure.currentOwner);
  Func_020014a8(9, gAdventure.currentOwner);
  Func_020014a8(10, gAdventure.currentOwner);
  WaitFrames(1);
  Func_0200056c(8);
  Func_0200056c(9);
  Func_0200056c(10);
  Actor_SetModeById(9, 0);
  WaitFrames(1);
  Func_020014a0(8, 0, 0);
  Func_020014a0(9, 0, 0);
  Func_020014a0(10, 0, 0);
  WaitFrames(1);
  goto test;
body:
  {
    u32 id = list[0];
    struct Actor *object = Actor_GetById(id);
    s32 flag = list[1];
    struct MapCell *cells = map->layers[object->terrainId].cells;
    u8 *shapes = gMapShapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);

    object->unk5c[0] = 3;
    if (GameFlag_Test(flag + 1)) {
      Func_020014a0(id, 0, 0);
    } else {
      s32 index = Func_02001508(object);
      struct MapCell *cell = cells + index;
      u8 *shape = shapes + index;
      s32 kind = Func_02001478(object->terrainId, object->x, object->z);
      s32 ground =
          Map_GetTerrainHeight(object->terrainId, object->x, object->z);

      Func_020014c0(id);
      cell->attributeB = 128;
      *shape = Func_02001528(object->terrainId, kind, (ground >> 19) + 4);
      if (GameFlag_Test(flag))
        ObjectVisual_CopyAttributes(id, 9);
    }
    list += 2;
  }
test:
  if (list[0] != 0xffff)
    goto body;
  WaitFrames(10);
}
