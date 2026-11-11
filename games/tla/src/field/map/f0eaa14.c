#include "types.h"
#include "actor.h"
#include "ram_buffer.h"

struct SwitchCell {
  u8 unk00[2];
  u8 block;
  u8 unk03_0 : 7;
  u8 raised : 1;
};

struct ActorFlag {
  u16 actor;
  s16 flag;
};

struct RestoreLayer {
  u8 unk00[0x138 - 0x108];
  struct SwitchCell *cells;
  u8 unk34[4];
};

struct RestoreMap {
  u8 unk000[0x108];
  struct RestoreLayer layers[1];
};

extern u8 gMapShapeGrid[];

struct Actor *Actor_GetById(u32 actor);
s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
void GameFlag_SetBit(s32 flag);
s32 Func_080eab70(struct Actor *object);
u32 Func_080202f0(s32 terrain, s32 x, s32 z);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);
s32 Func_080eab98(s32 unused, s32 key, s32 value);

void Func_080eaa14(struct ActorFlag *list) {
  u8 *event = Ram_HeapSlots->eventWork;
  u8 *grid = gMapShapeGrid;

  *(struct Actor **)(event + 0xcbc) = NULL;
  for (; list->actor != 0xffff; list++) {
    s32 change = 0;
    struct Actor *object = Actor_GetById(list->actor);
    u8 frame = ((u8 *)object->animation)[24];
    u16 flag = list->flag;

    if (GameFlag_Test(flag)) {
      if (frame != 0)
        change = 1;
    } else {
      if (frame != 1)
        change = 1;
    }
    if (change) {
      struct SwitchCell *cells = ((struct RestoreMap *)Ram_HeapSlots->mapWork)
                                     ->layers[object->terrainId]
                                     .cells;
      u8 *shapes;
      s32 slot;
      u32 key;
      s32 height;

      shapes = &grid[((s32)cells - (s32)Ram_MapCellBuffer) >> 2];
      slot = Func_080eab70(object);
      key = Func_080202f0(object->terrainId, object->x, object->z);
      height =
          Map_GetTerrainHeightFar(object->terrainId, object->x, object->z) >>
          19;

      *(struct Actor **)(event + 0xcbc) = object;
      if (GameFlag_Test(flag)) {
        height -= 4;
        GameFlag_ClearBit(flag);
      } else {
        height += 4;
        GameFlag_SetBit(flag);
      }
      if ((object->x | object->z) != 0) {

        cells += slot;
        shapes += slot;
        *shapes = Func_080eab98(object->terrainId, key, height);
        if (GameFlag_Test(flag)) {
          s32 bits;

          cells->block = 0;
          // The bit set in int arithmetic, where the clear below is a bitfield.
          bits = ((u8 *)cells)[3] | 0x80;
          ((u8 *)cells)[3] = bits;
        } else {
          cells->block = 0xff;
          cells->raised = 0;
        }
      }
    }
  }
}
