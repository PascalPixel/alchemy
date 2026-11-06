// Matches: differ 0 of 130 (tla-en, 2026-11-06).
// Under -Os, 5 November: differ 46 of 130 (tla-en); the -O2 fitting before it
// (two `list += 2`, offset and index sums, a spilled end) no longer applies.
// The Dehkan Plateau prepare step for a list of (actor, flag) pairs: an actor
// whose second flag is set is put away, otherwise its cell is marked and the
// shape grid raised under it. No devices.
// What each form is for (each measured at -Os):
// - one `list += 2` after the if/else: the native shares the step.
// - nextFlag is its own local before the cell lookup: `adds r0, r2, #0` needs
//   flag copied there; `GameFlag_Test(flag + 1)` differs 58.
// - cell and shapeCell are taken right after the index call; the attribute
//   store through struct MapCell's attributeB rather than `((u8 *)cell)[3]`
//   gives the native's schedule (4 differ as a byte pointer).
#include "types.h"
#include "call.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "map.h"

extern u8 gMapShapeGrid[];

s32 GameFlag_Test(s32 flag);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02003ba0(struct Actor *object, s32 mode);
s32 Func_02003c10(s32 layer, s32 x, s32 z);
void Func_02003c68(s32 id, s32 x, s32 z);
void Func_02003cc0(s32 id);
s32 Func_02003db0(struct Actor *object);
s32 Func_02003dd0(s32 layer, s32 shape, s32 height);

void Func_020001d0(const void *data) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  const u16 *list = data;
  struct Actor *leader;
  s32 height;

  if (list[0] != 0xffff) {
    do {
      s32 id = list[0];
      struct Actor *object = Actor_GetById(id);
      s32 flag = list[1];
      s32 nextFlag = flag + 1;
      struct MapCell *cells = map->layers[object->terrainId].cells;
      u8 *shapes =
          gMapShapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);

      if (GameFlag_Test(nextFlag)) {
        Func_02003c68(id, 0, 0);
      } else {
        s32 index = Func_02003db0(object);
        struct MapCell *cell = &cells[index];
        u8 *shapeCell = &shapes[index];
        s32 shape = Func_02003c10(object->terrainId, object->x, object->z);
        s32 terrainHeight =
            Map_GetTerrainHeight(object->terrainId, object->x, object->z);

        Func_02003cc0(id);
        cell->attributeB = 128;
        *shapeCell = Func_02003dd0(object->terrainId, shape,
                                   (terrainHeight >> 19) + 4);
        if (GameFlag_Test(flag))
          Func_02003ba0(object, 0);
      }
      list += 2;
    } while (list[0] != 0xffff);
  }
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  height = Map_GetTerrainHeight(leader->terrainId, leader->x, leader->z);
  if (leader->y < height) {
    leader->terrainHeight = height;
    leader->y = height;
  }
}
