#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "map.h"

extern u8 gMapShapeGrid[];

struct Actor *Actor_GetById(s32 id);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 Func_020010ec(s32 layer, s32 x, s32 z);
s32 Func_0200118c(s32 owner, s32 mode);
s32 Func_020011d4(struct Actor *object);
s32 Func_020011f4(s32 layer, s32 shape, s32 height);
struct Actor *Func_0200120c(void);

#define HELD_OBJECT(work) (*(struct Actor **)((u8 *)(work) + 0xcbc))

// A one-field view: the shape store is memory of its own to sched2, which sinks
// it past the cell's first read (as a plain store, 4 of 131 instructions differ
// in tla-en). A struct is padded to 4 bytes here, so the pointer stays plain.
struct ShapeCell {
  u8 shape;
};

/// Lowers the cells under the held actor, or the one the leader carries, for
/// each listed actor it is: the shape grid takes the shape for its new height
/// and the cell flags under it are cleared; the actor is kept at 0xcbc of the
/// event work. Overlay 655's Func_02000d78 and overlay 676's copy.
void Func_0200014c(u16 *list) {
  struct Actor *held = Func_0200120c();
  void *work = Ram_HeapSlots->eventWork;
  u8 *grid;

  if (held == NULL) {
    s32 id = Func_0200118c(gMainRoutineWork.currentOwner, 1);

    if (id != -1)
      held = Actor_GetById(id);
  }
  grid = FIXED_ADDRESS((u8 *), 0x02024000, gMapShapeGrid);
  HELD_OBJECT(work) = NULL;
  if (*list != 0xffff) {
  loop:
    if (held == Actor_GetById(*list)) {
      u8 *layer = &held->terrainId;
      u8 *cell = (u8 *)Ram_HeapSlots->mapWork->layers[*layer].cells;
      u8 *shape =
          grid + ((struct MapCell *)cell - (struct MapCell *)Ram_MapCellBuffer);
      s32 kind = Func_020010ec(*layer, held->x, held->z);
      s32 height = Map_GetTerrainHeight(*layer, held->x, held->z);
      s32 index = Func_020011d4(held);
      s32 result = Func_020011f4(*layer, kind, (height >> 19) - 4);

      shape += index;
      /* FAKEMATCH(temporary): index also holds its cell offset; as cell +=
       * index * 4, 35 of 131 instructions differ in tla-en. */
      index <<= 2;
      ((struct ShapeCell *)shape)->shape = result;
      cell += index;
      ((struct MapCell *)cell)->attributeB &= 63;
      cell += 0x200;
      ((struct MapCell *)cell)->attributeB &= 191;
      HELD_OBJECT(work) = held;
    }
    list++;
    if (*list != 0xffff)
      goto loop;
  }
}
