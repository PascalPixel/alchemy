// Draft, differ 142 of 159: register choice throughout, and some order.
// The native keeps the object in r6 and the change flag in r7, the cell
// table in r8 and the shape row in r9, and spills the slot and key (sp 4,
// sp 0); this swaps r6 and r7 and keeps the slot in r10, so every later
// use differs. Func_080eab98's value goes in unextended, so its parameter
// is an int here (an s8 parameter adds lsl/asr). The cell's raised bit as
// a bitfield measured 142, as |= 0x80 and &= 0x7f 153.
// Raises or lowers the map under a list of switch actors. For each
// (actor, flag) entry until 0xffff whose sprite frame (0 when the flag is
// set, 1 when clear) disagrees with its flag, it records the actor in the
// event work's word at 0xcbc, toggles the flag and moves the actor's cell
// four half-units down (flag was set) or up: the cell's shape in
// gMapShapeGrid takes Func_080eab98's tag for the cell's key at the new
// height, and its entry turns passable (byte 2 0, bit 7 of byte 3 set) or
// blocked (0xff, bit 7 clear) by the new flag. Actors at x and z 0 only
// toggle.
#include "types.h"
#include "object_runtime.h"
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
  u8 (*cells)[4];
  u8 unk34[4];
};

struct RestoreMap {
  u8 unk000[0x108];
  struct RestoreLayer layers[1];
};

extern u8 gMapShapeGrid[];

struct ObjectRuntime *Object_GetById(u32 actor);
s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
void GameFlag_SetBit(s32 flag);
s32 Func_080eab70(struct ObjectRuntime *object);
u32 Func_080202f0(s32 terrain, s32 x, s32 z);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);
s32 Func_080eab98(s32 unused, s32 key, s32 value);

void Func_080eaa14(struct ActorFlag *list) {
  u8 *event = Ram_HeapSlots->eventWork;

  *(struct ObjectRuntime **)(event + 0xcbc) = NULL;
  for (; list->actor != 0xffff; list++) {
    s32 change = 0;
    struct ObjectRuntime *object = Object_GetById(list->actor);
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
      u8 (*cells)[4] = ((struct RestoreMap *)Ram_HeapSlots->mapWork)
                           ->layers[object->terrainId]
                           .cells;
      u8 *shapes = &gMapShapeGrid[((s32)cells - 0x02010000) >> 2];
      s32 slot = Func_080eab70(object);
      u32 key = Func_080202f0(object->terrainId, object->x, object->z);
      s32 height =
          Map_GetTerrainHeightFar(object->terrainId, object->x, object->z);

      *(struct ObjectRuntime **)(event + 0xcbc) = object;
      height >>= 19;
      if (GameFlag_Test(flag)) {
        height -= 4;
        GameFlag_ClearBit(flag);
      } else {
        height += 4;
        GameFlag_SetBit(flag);
      }
      if ((object->x | object->z) != 0) {
        struct SwitchCell *cell = (struct SwitchCell *)cells[slot];

        shapes[slot] = Func_080eab98(object->terrainId, key, height);
        if (GameFlag_Test(flag)) {
          cell->block = 0;
          cell->raised = 1;
        } else {
          cell->block = 0xff;
          cell->raised = 0;
        }
      }
    }
  }
}
