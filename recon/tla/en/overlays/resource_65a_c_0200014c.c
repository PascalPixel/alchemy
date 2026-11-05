// Draft, differ 27 of 131 (tla-en, 2026-11-05).
// Under -Os, 5 November: differ 41 of 131 (tla-en); older figures below predate it.
// Draft, differ 67 of 131, skeleton gap 10 (tla-en, 2026-11-01)
// Overlay 655's Func_02000d78 and overlay 676's copy, with this overlay's
// import stubs: lowers the cells under the held actor (or the one the leader
// carries) and keeps it at 0xcbc of the event work.
// Left: the listing keeps index in r6 and adds it to the cell pointer in place
// in r10; here entry ties to index (r5), so level and index swap, kind takes
// r10, and the two adds and their reloads differ (the rest of the gap).
// With one cell pointer throughout, that pseudo stays local and takes r4 by
// caller-save (20 refs across 4 calls), list leaves the stack and the top test
// cross-jumps into the bottom; splitting entry and cell is what keeps list
// spilled. The label loop keeps &held->terrainId out of loop.c's hoisting;
// index <<= 2 keeps *shape from combining into a register-offset store.
// Separate kind, height and result keep them local (r5, r6, r8 as listed).
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "map.h"
#include "iwram_call.h"
#include "call.h"

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
// it past the cell's first read. A struct is padded to 4 bytes here, so the
// pointer stays plain.
struct ShapeCell {
  u8 shape;
};

void Func_0200014c(u16 *list) {
  struct Actor *held = Func_0200120c();
  void *work = Ram_HeapSlots->eventWork;

  if (held == NULL) {
    s32 id = Func_0200118c(gMainRoutineWork.currentOwner, 1);

    if (id != -1)
      held = Actor_GetById(id);
  }
  HELD_OBJECT(work) = NULL;
  if (*list != 0xffff) {
  loop:
    if (held == Actor_GetById(*list)) {
      u8 *layer = &held->terrainId;
      u8 *cell = (u8 *)Ram_HeapSlots->mapWork->layers[*layer].cells;
      u8 *shape = gMapShapeGrid +
                  ((struct MapCell *)cell - (struct MapCell *)Ram_MapCellBuffer);
      s32 kind = Func_020010ec(*layer, held->x, held->z);
      s32 height = Map_GetTerrainHeight(*layer, held->x, held->z);
      s32 index = Func_020011d4(held);
      s32 result = Func_020011f4(*layer, kind, (height >> 19) - 4);

      shape += index;
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
