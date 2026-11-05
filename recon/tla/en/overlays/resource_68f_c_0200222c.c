// Measured under -Os, 5 November 2026: differ 67 (tla-en); notes below may predate it.
// Draft, differ 68 of 130, skeleton gap 12 (tla-en, 2026-11-01): on entering,
// sets each listed pillar from its two flags (pushed: moved to its place; else
// its cell raised and its shape redrawn), then lifts the leader onto the ground
// under it. The same function as overlay 67F's Func_020001d0 and overlay 691's
// Func_02002d08; only the called helpers' names differ. The shape address and
// the height's two statements are as matched s2c1c.c writes them; neither moves
// the code.
// Left, one cause: the native's cell is at sp+4 and r6 holds shapes; here cell
// (4 refs, block 4 only) goes to local-alloc, which takes r5, r6, r8 and r10,
// shapes loses to the stack and the frame is 16, not 20. The native's local-
// alloc leaves r6 alone, so its cell lives outside it. Its `adds r5, r5, r3;
// str r5, [sp, #4]` is no output reload, which would pick a free low register;
// r5 is really written, then stored. Its sp+0 is never read or written.
// Second cause: `mov r11, r0` comes before the entry compare. One variable gives
// that but has 16 refs (priority 1.17), beats the terrain address for r9 and
// swaps r9 and r11; the two here have 15 (0.57). Reading the first entry into an
// s32 before `list = entries` gives both (same differ, gap 11) but is a forced
// temporary, so it stays out.
// Tried, no gain: cell in an 8-byte struct or array (cse forwards through it, a
// BLKmode struct lands above the spills, 101), cell = 0 at each scope, set in
// the other arm, s32 cell, `cells += index` (80), `cells +=` then a copy (80),
// shapes[index] (84), no `continue` (list spilled, 89).
// `index += (s32)shapes` is differ 67, one better, but casts to a type the object
// is not. Next lead: what keeps cell out of local-alloc (2 or 0 deaths, or a
// second block) without reusing a local.
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "map.h"

extern u8 gMapShapeGrid[];

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void Func_02003088(s32 id, s32 x, s32 z);
s32 Func_02003160(struct Actor *object);
s32 Func_02003010(s32 layer, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_020030c0(s32 id);
s32 Func_02003180(s32 layer, s32 shape, s32 height);
void Func_02002f80(struct Actor *object, s32 mode);

void Func_0200222c(u16 *entries) {
  u16 *list;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *leader;
  s32 ground;

  if (*entries != 0xffff) {
    list = entries;
    do {
      u32 id = list[0];
      struct Actor *object = Actor_GetById(id);
      s32 flag = list[1];
      struct MapCell *cells = map->layers[object->terrainId].cells;
      u8 *shapes = FIXED_ADDRESS((u8 *), 0x02024000, gMapShapeGrid) +
                   (cells - (struct MapCell *)Ram_MapCellBuffer);

      if (GameFlag_Test(flag + 1)) {
        Func_02003088(id, 0, 0);
        continue;
        list += 2;
      } else {
        s32 index = Func_02003160(object);
        struct MapCell *cell;
        s32 shape;
        s32 height;

        cell = cells + index;
        shape = Func_02003010(object->terrainId, object->x, object->z);
        height =
            Map_GetTerrainHeight(object->terrainId, object->x, object->z) >> 19;

        height += 4;

        Func_020030c0(id);
        cell->attributeB = 128;
        shapes += index;
        *shapes = Func_02003180(object->terrainId, shape, height);
        if (GameFlag_Test(flag))
          Func_02002f80(object, 0);
      }
      list += 2;
    } while (*list != 0xffff);
  }
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  ground = Map_GetTerrainHeight(leader->terrainId, leader->x, leader->z);
  if (leader->y < ground) {
    leader->terrainHeight = ground;
    leader->y = ground;
  }
}
