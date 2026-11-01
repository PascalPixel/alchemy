// Draft, differ 58 of 150, skeleton gap 32 (tla-en, 2026-11-01): overlay 697's
// copy of the trap table's setup, as in resource_692_c_020042f8.c (overlay 692's
// Func_020042f8). The write of 3 is at offset 92, unk5c[0], not movementState.
// Left: terrain falls to sl and cells to r7 where the listing has r7 and r6, which
// turns the terrain pointer's two adds into movs and adds and costs mov through
// sl at each read. The listing also keeps the cell pointer in a stack slot
// (adds r6, r6, r3; str r6, [sp, #4]; a later ldr r6), with sp+0 unused, so its
// adds stays above the calls. That slot looks like reload evicting the cell
// pointer and its shifted index from r6; a block-local pointer takes r6 and
// pushes shapeEntry to r8 and shape to sl as the listing has them, but then its
// index conflicts with cells, table loses its register to the stack and the gap
// rises to 48. Tried: a separate cell pointer before and after shapeEntry, an
// offset variable, cell = cells then +=, shapes += index, unsigned types.
// nextFlag computed early is what places adds r0 above the terrain read.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "actor.h"
#include "main_routine_work.h"

extern u8 gMapShapeGrid[];

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void Func_020058f0(s32 id, s32 owner);
void Func_0200468c(s32 id);
void Task_Wait(s32 frames);
void Func_020058e0(s32 id, s32 a, s32 b);
s32 Func_02005a18(struct Actor *object);
s32 Func_02005848(s32 layer, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02005958(s32 id);
u8 Func_02005a20(s32 layer, s32 shape, s32 height);
void ObjectVisual_CopyAttributes(s32 id, s32 from);

void Func_020046a4(const u16 *table) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *object;
  struct MapCell *cells;
  u8 *shapes;
  u8 *terrain;
  u8 *shapeEntry;
  s32 nextFlag;
  s32 id;
  s32 flag;
  s32 index;
  s32 shape;
  s32 height;

  Func_020058f0(8, gMainRoutineWork.currentOwner);
  Func_020058f0(9, gMainRoutineWork.currentOwner);
  Func_020058f0(10, gMainRoutineWork.currentOwner);
  Task_Wait(1);
  Func_0200468c(8);
  Func_0200468c(9);
  Func_0200468c(10);
  Actor_SetModeById(9, 0);
  Task_Wait(1);
  Func_020058e0(8, 0, 0);
  Func_020058e0(9, 0, 0);
  Func_020058e0(10, 0, 0);
  Task_Wait(1);
  goto test;
next:
  id = table[0];
  object = Actor_GetById(id);
  flag = table[1];
  terrain = &object->terrainId;
  nextFlag = flag + 1;
  cells = map->layers[*terrain].cells;
  shapes = gMapShapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);
  object->unk5c[0] = 3;
  if (GameFlag_Test(nextFlag)) {
    Func_020058e0(id, 0, 0);
  } else {
    index = Func_02005a18(object);
    shapeEntry = shapes + index;
    cells += index;
    shape = Func_02005848(*terrain, object->x, object->z);
    height = Map_GetTerrainHeight(*terrain, object->x, object->z);
    Func_02005958(id);
    cells->attributeB = 128;
    *shapeEntry = Func_02005a20(*terrain, shape, (height >> 19) + 4);
    if (GameFlag_Test(flag))
      ObjectVisual_CopyAttributes(id, 9);
  }
  table += 2;
test:
  if (*table != 0xffff)
    goto next;
  Task_Wait(10);
}
