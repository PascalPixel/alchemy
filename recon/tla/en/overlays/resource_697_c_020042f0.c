// Draft, differ 31 of 130, skeleton gap 7 (tla-en, 2026-11-01): overlay 697's copy
// of the trap table's setup, resource_692_c_02003f44.c's twin. A trap whose second
// flag is set is put away; any other marks its cell (attribute byte 128), writes a
// shape for its height into the shape grid, and takes mode 0 when its first flag
// is set. Then the leader is lifted onto the ground if it stands below it.
// Left: the put-away branch rereads table[0] where the listing loads the id from
// its stack slot. Written as id, the id has four references, outranks table for the
// last register and table falls to the stack (gap 33, and the entry test cross-jumps
// into the one at the bottom). With three, every register but the stack slots'
// order and one load's scheduling agrees. So the source's id is lower priority, or
// table higher, than these forms give. Tried for that: u16, s16 and u32 id, a
// struct entry, table++ twice, a temporary for id, id assigned in the argument,
// rereading in Func_02005958's call instead (gap 13), real loops (hoist the marker).
// slot and cell are taken early on purpose: separate pointers, set above the calls,
// keep their adds where the listing has them.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "actor.h"
#include "main_routine_work.h"

extern u8 gMapShapeGrid[];

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void Func_020058e0(s32 id, s32 a, s32 b);
s32 Func_02005a18(struct Actor *object);
s32 Func_02005848(s32 layer, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02005958(s32 id);
u8 Func_02005a20(s32 layer, s32 shape, s32 height);
void Func_020057c8(struct Actor *object, s32 mode);

void Func_020042f0(const u16 *table) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *object;
  s32 id;
  s32 flag;
  s32 nextFlag;
  struct MapCell *cells;
  u8 *shapes;
  u8 *cell;
  s32 index;
  struct Actor *leader;
  s32 ground;

  if (*table == 0xffff)
    goto done;
next:
  id = table[0];
  object = Actor_GetById(id);
  flag = table[1];
  nextFlag = flag + 1;
  cells = map->layers[object->terrainId].cells;
  shapes = gMapShapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);
  if (GameFlag_Test(nextFlag)) {
    Func_020058e0(id, 0, 0);
  } else {
    s32 shape;
    u8 *slot;
    s32 height;

    index = Func_02005a18(object);
    cell = (u8 *)&cells[index];
    slot = shapes + index;
    shape = Func_02005848(object->terrainId, object->x, object->z);
    height = Map_GetTerrainHeight(object->terrainId, object->x, object->z);
    Func_02005958(id);
    cell[3] = 128;
    *slot = Func_02005a20(object->terrainId, shape, (height >> 19) + 4);
    if (GameFlag_Test(flag))
      Func_020057c8(object, 0);
  }
  table += 2;
  if (*table != 0xffff)
    goto next;
done:
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  ground = Map_GetTerrainHeight(leader->terrainId, leader->x, leader->z);
  if (leader->y < ground) {
    leader->terrainHeight = ground;
    leader->y = ground;
  }
}
