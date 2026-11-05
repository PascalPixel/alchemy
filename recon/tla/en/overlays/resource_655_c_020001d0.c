// Draft, differ 46 of 130 (tla-en, 2026-11-05). Older notes below may predate this figure.
// Under -Os, 5 November: differ 46 of 130 (tla-en); older figures below predate it.
// Draft, differ 18 of 130, skeleton gap 2 (tla-en, 2026-11-01)
// The Dehkan Plateau prepare step for a list of (actor, flag) pairs: an actor
// whose second flag is set is put away, otherwise its cell is marked and the
// shape grid raised under it. Registers, frame and slots now agree with the
// listing; no devices.
// Left: the reload scratch at three sites is r2 where the native has r3 and
// back (offset's shift, the two terrain reads after it), plus `str r2, [sp, #8]`
// one place late (a sched2 tie). The scratch cause: the first arm's `list += 2`
// reloads its 4 into r3, so the next reload rotates to r2. The native reaches
// the shift with the rotation on r2, yet list needs that second `list += 2`:
// it makes refs 15, the only count that ranks list between object+34's pseudo
// (r9) and id (spilled). 12 measured losing r11 to id, 16 taking r9; by the
// priority formula 14 loses and 17 takes r9 too (not measured).
// Next: find two more list refs that reload nothing before the shift.
// What each form is for (each measured, keep them):
// - first is read before `list = data`, or CSE puts the entry test on list and
//   makes refs 16.
// - end is one constant for its whole life, spilled, so both tests rebuild
//   0xffff as a reload; that sets the rotation on entering the loop.
// - struct ListEntry reads the two tests in another alias set, or gcse reuses
//   them for id at the top of the loop.
// - nextFlag is set before the lookup: `adds r0, r2, #0` needs flag still in r2.
// - index += shapes sits early, tied to r8; sched2 sinks it to the `add r8, r6`.
//   A pointer to the shape cell reaches the same place only with a wrong tie.
// - cell and offset take r6 in local-alloc; shapes then evicts them (global.c,
//   density 8/23 against 4/11), which is why cell lives at sp+4 and the unread
//   sp+0 slot exists. Hence offset before the sum, cell declared first.
#include "types.h"
#include "call.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "map.h"

struct ListEntry {
  u16 id;
  u16 flag;
};

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
  s32 first = ((const struct ListEntry *)data)->id;
  const u16 *list = data;
  struct Actor *leader;
  s32 height;
  s32 end = 0xffff;

  if (first != end) {
    do {
      s32 id = list[0];
      struct Actor *object = Actor_GetById(id);
      s32 flag = list[1];
      struct MapCell *cells = map->layers[object->terrainId].cells;
      s32 nextFlag = flag + 1;
      u8 *shapes =
          gMapShapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);

      if (GameFlag_Test(nextFlag)) {
        Func_02003c68(id, 0, 0);
        list += 2;
      } else {
        s32 index = Func_02003db0(object);
        s32 offset = index * 4;
        u8 *cell;
        s32 shape, terrainHeight;

        index += (s32)shapes;
        cell = (u8 *)cells + offset;
        shape = Func_02003c10(object->terrainId, object->x, object->z);
        terrainHeight =
            Map_GetTerrainHeight(object->terrainId, object->x, object->z);
        Func_02003cc0(id);
        cell[3] = 128;
        *(u8 *)index = Func_02003dd0(object->terrainId, shape,
                                     (terrainHeight >> 19) + 4);
        if (GameFlag_Test(flag))
          Func_02003ba0(object, 0);
        list += 2;
      }
    } while (((const struct ListEntry *)list)->id != end);
  }
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  height = Map_GetTerrainHeight(leader->terrainId, leader->x, leader->z);
  if (leader->y < height) {
    leader->terrainHeight = height;
    leader->y = height;
  }
}
