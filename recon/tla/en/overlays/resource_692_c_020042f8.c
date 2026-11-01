// Draft, differ 68 of 150, skeleton gap 18 (tla-en, 2026-11-01)
// The trap table's setup for maps with objects 8 to 10 (shared with overlay 697
// and the other trap maps): those three are placed and put away first; then
// each trap whose second flag is set is put away, and any other marks its cell,
// writes a shape for its height into the shape grid and, when its first flag is
// set, copies object 9's look.
//
// Settled from the listing: the byte at 92 is unk5c[0], not movementState (91);
// terrainId is a pointer local (r7 in the native); flag + 1 is taken right
// after the flag load (the native copies it from the reload register still
// holding it); the grid address is added before the calls, in the index's
// register, so shapes can die and hand r10 to shape.
//
// Left is allocation. The native has object and height r5, cells r6, terrainId
// r7, index r8, id r9, shapes and shape r10, table r11, map, flag and the cell
// on the stack at 12, 8 and 4, and sp + 0 never touched. This draft has cells
// r7, terrainId r10, shapes and shape r8, index r6, and a frame of 8. The gap
// starts there: terrainId in a high register gives the movs #34 reload form.
//
// The likely mechanism, not reproduced: local-alloc gives the cell r6 ahead of
// the index, which is why the index lands in r8; then reload kicks the cell out
// of r6 to make a spill register, leaving its slot and the unused sp + 0, and
// reloads it through r6 (Func_02003f44 shows the same two slots). A separate
// cell pseudo reaches r6 here, but its add then sinks past the calls and cells
// loses r6 (differ 90). cells += index alone keeps the add early, and the cell
// merges with cells. Also tried, no better: an 8-byte struct local holding the
// cell (73; it gives the frame's pair but folds or reloads with adds #3), a
// two-step offset, a twice-set cell, index += shapes, declaration order. Next:
// find what keeps the add in place with a separate cell, then what forces the
// spill. Func_02003f44 and overlay 697's Func_020042f0 should move with it.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "actor.h"
#include "main_routine_work.h"

extern u8 gMapShapeGrid[];

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void Func_02004da8(s32 id, s32 owner);
void Func_020042e0(s32 id);
void wait(s32 frames);
void Func_02004da0(s32 id, s32 a, s32 b);
s32 Func_02004ed0(struct Actor *object);
s32 Func_02004d20(s32 layer, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02004df8(s32 id);
u8 Func_02004ef0(s32 layer, s32 shape, s32 height);
void ObjectVisual_CopyAttributes(s32 id, s32 from);

void Func_020042f8(const u16 *table) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 flag;
  u8 *cell;
  struct Actor *object;
  u8 *terrainId;
  struct MapCell *cells;
  u8 *shapes;
  u8 *out;
  s32 id;
  s32 index;
  s32 shape;
  s32 height;
  s32 level;
  s32 putAwayFlag;

  Func_02004da8(8, gMainRoutineWork.currentOwner);
  Func_02004da8(9, gMainRoutineWork.currentOwner);
  Func_02004da8(10, gMainRoutineWork.currentOwner);
  wait(1);
  Func_020042e0(8);
  Func_020042e0(9);
  Func_020042e0(10);
  Actor_SetModeById(9, 0);
  wait(1);
  Func_02004da0(8, 0, 0);
  Func_02004da0(9, 0, 0);
  Func_02004da0(10, 0, 0);
  wait(1);
  goto test;
next:
  id = table[0];
  object = Actor_GetById(id);
  flag = table[1];
  putAwayFlag = flag + 1;
  terrainId = &object->terrainId;
  cells = map->layers[*terrainId].cells;
  shapes = gMapShapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);
  object->unk5c[0] = 3;
  if (GameFlag_Test(putAwayFlag)) {
    Func_02004da0(id, 0, 0);
  } else {
    index = Func_02004ed0(object);
    cells += index;
    cell = (u8 *)cells;
    out = shapes + index;
    shape = Func_02004d20(*terrainId, object->x, object->z);
    height = Map_GetTerrainHeight(*terrainId, object->x, object->z);
    Func_02004df8(id);
    level = (height >> 19) + 4;
    cell[3] = 128;
    *out = Func_02004ef0(*terrainId, shape, level);
    if (GameFlag_Test(flag))
      ObjectVisual_CopyAttributes(id, 9);
  }
  table += 2;
test:
  if (*table != 0xffff)
    goto next;
  wait(10);
}
