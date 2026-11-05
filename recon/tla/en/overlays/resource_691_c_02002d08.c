// Measured under -Os, 5 November 2026: differ 69 (tla-en); notes below may predate it.
// Draft, differ 67 of 130, skeleton gap 13 (tla-en, 2026-11-01): on entering,
// sets each listed pillar from its two flags (pushed: moved to its place; else
// its cell raised and its shape redrawn), then lifts the leader onto the ground
// under it. The same function as overlay 67F's Func_020001d0 and overlay 68F's
// Func_0200222c; only the called helpers' names differ between the copies.
// Closed: the loop shape. The top test reads the parameter and a second local
// walks the list (list refs 15, so list beats id and loses to terrainId: r11,
// id on the stack); `list += 2` in both arms with `continue` cross-jumps into
// the listing's one tail; `index += (s32)shapes` gives its `add r8, r6`.
// Left, all one cause: the native's `cell` lives at sp+4, this draft's in r6.
// Here cell is a block-local pseudo, so local-alloc takes r6 before global-alloc
// runs (local-alloc.c needs REG_N_DEATHS == 1), and shapes loses to the stack.
// The native keeps r6 from object and id but gives it to shapes, and its frame
// has a fifth slot, sp+0, that nothing reads; together those point at a local in
// r6 whose code is gone, or at a second life of cell. The `lsls`/`adds` run on
// r3 and r5 because cell is spilled: fix that and the rest should follow.
// Tried: cell as s32 from `index * 4` then `+= cells` (75 in the old loop, 67
// here), from `<< 2` (two sets, still local), reusing cell for `(ground >> 19) +
// 4` (two deaths, so global, but its priority 0.88 still wins r6: 73), `cell =
// cells` first and `cells +=` then a copy (cse folds both), a `u8 *p` at the top
// (87), `index +=` early (84), volatile cell (its slot lands at the top: 97).
#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "map.h"

extern u8 gMapShapeGrid[];

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void Func_02003b0c(s32 id, s32 x, s32 z);
s32 Func_02003c04(struct Actor *object);
s32 Func_02003adc(s32 layer, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02003b3c(s32 id);
s32 Func_02003c24(s32 layer, s32 shape, s32 height);
void Func_02003a54(struct Actor *object, s32 mode);

void Func_02002d08(u16 *entries) {
  u16 *list;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *leader;
  s32 height;

  if (*entries != 0xffff) {
    list = entries;
    do {
      u32 id = list[0];
      struct Actor *object = Actor_GetById(id);
      s32 flag = list[1];
      struct MapCell *cells = map->layers[object->terrainId].cells;
      u8 *shapes =
          gMapShapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);

      if (GameFlag_Test(flag + 1)) {
        Func_02003b0c(id, 0, 0);
        list += 2;
        continue;
      } else {
        s32 index = Func_02003c04(object);
        struct MapCell *cell;
        s32 shape;
        s32 ground;

        cell = cells + index;
        shape = Func_02003adc(object->terrainId, object->x, object->z);
        ground = Map_GetTerrainHeight(object->terrainId, object->x, object->z);

        Func_02003b3c(id);
        cell->attributeB = 128;
        index += (s32)shapes;
        *(u8 *)index =
            Func_02003c24(object->terrainId, shape, (ground >> 19) + 4);
        if (GameFlag_Test(flag))
          Func_02003a54(object, 0);
      }
      list += 2;
    } while (*list != 0xffff);
  }
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  height = Map_GetTerrainHeight(leader->terrainId, leader->x, leader->z);
  if (leader->y < height) {
    leader->terrainHeight = height;
    leader->y = height;
  }
}
