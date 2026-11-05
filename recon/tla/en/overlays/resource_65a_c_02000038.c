// Under -Os, 5 November: differ 44 of 123 (tla-en); older figures below predate it.
// Draft, differ 90 of 123, skeleton gap 15 (tla-en, 2026-11-01)
// Raises each listed actor's cells (the shape grid and the cell flags of its
// cell and the one below); the same function as overlay 655's Func_02000c64
// under this overlay's import stubs. What is left is allocation, plus one
// order: local-alloc gives shapes r4 and saves it around the flags call, so
// shape lands in r9, object in r6 and every mov becomes adds. The native has
// shapes r5, cells r6, object r8, level r10, index r9, shape on the stack, and
// no r4 at all, as if no pseudo found CALLER_SAVE_PROFITABLE (the lreg dump
// shows loop-weighted refs of 20 for object, 12 for level). The native also
// stores *shapes after loading the next id, which sched2 cannot do here while
// the two may alias. Declaration order, scopes, for, while and label loops,
// a const list, struct MapCell pointers and an id temporary gained nothing.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "map.h"
#include "iwram_call.h"
#include "call.h"

extern u8 gMapShapeGrid[];

struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 Func_020010ec(s32 layer, s32 x, s32 z);
s32 Func_020011d4(struct Actor *object);
s32 Func_020011f4(s32 layer, s32 shape, s32 height);

void Func_02000038(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *leader;
  s32 height;

  while (*list != 0xffff) {
    struct Actor *object = Actor_GetById(*list);
    u8 *layer = &object->terrainId;
    s32 shape = Func_020010ec(*layer, object->x, object->z);
    s32 level = Map_GetTerrainHeight(*layer, object->x, object->z);
    s32 index;
    u8 *cells;
    u8 *shapes;
    u8 newShape;

    level >>= 19;
    index = Func_020011d4(object);
    cells = (u8 *)map->layers[*layer].cells;
    shapes = gMapShapeGrid +
             (((struct MapCell *)cells) - (struct MapCell *)Ram_MapCellBuffer);
    object->motionFlags = 0;
    object->collisionFlags = 0;
    level += 4;
    Actor_SetSingleChildFlags(object, 0);
    newShape = Func_020011f4(*layer, shape, level);
    shapes += index;
    index <<= 2;
    cells += index;
    cells += 0x200;
    cells[3] |= 192;
    list++;
    *shapes = newShape;
    cells[3] |= 64;
  }
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  height = Map_GetTerrainHeight(leader->terrainId, leader->x, leader->z);
  if (leader->y < height) {
    leader->terrainHeight = height;
    leader->y = height;
  }
}
