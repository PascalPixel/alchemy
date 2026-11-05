// Draft, differ 60 of 150, skeleton gap 15 (tla-en, 2026-11-01)
// Control flow, calls and data access are right; the gap is one allocation.
// The native spills the cell pointer (cells + index, sp+4; an unused slot at
// sp+0, likely index * 4 spilled too), so cells takes r6, the terrainId address
// r7, shape r10 and index and the shape pointer r8, frame 16. Here the frame is
// 8. With separate `cell` and `slot` locals, local-alloc ties index * 4 to cell
// in r6, which conflicts with cells and pushes `list` onto the stack: differ 90,
// though the --rtl dumps give index r8 and shape r10 as the native has them.
// The lead: `cell` must not tie to index * 4 (the native has it in r3) and
// must still lose its register. Tried without gain: cell = cells then +=, index
// + cells, a named offset, index reused as the pointer, shape reused from
// index, one scope for all locals, u32 id and flag, a terrainId pointer local
// (differ 62), a two-word struct for cell (folds to the same code).
// nextFlag is a plain local, not a forced one: the native reads flag + 1 before
// the terrainId, the reload copy `adds r0, r2, #0` shows it, and the neighbour
// Func_020024e8 keeps flag + 1 in its own variable; as an inline argument,
// differ 68. Func_0200222c has the same loop body and frame shape.
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "map.h"

extern u8 gMapShapeGrid[];

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void wait(s32 frames);
void Func_02003088(s32 id, s32 x, s32 z);
void Func_02003090(s32 id, s32 target);
void Func_020025c8(s32 id);
s32 Func_02003160(struct Actor *object);
s32 Func_02003010(s32 layer, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_020030c0(s32 id);
s32 Func_02003180(s32 layer, s32 shape, s32 height);
void ObjectVisual_CopyAttributes(u32 targetId, u32 sourceId);

void Func_020025e0(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;

  Func_02003090(8, gMainRoutineWork.currentOwner);
  Func_02003090(9, gMainRoutineWork.currentOwner);
  Func_02003090(10, gMainRoutineWork.currentOwner);
  wait(1);
  Func_020025c8(8);
  Func_020025c8(9);
  Func_020025c8(10);
  Actor_SetModeById(9, 0);
  wait(1);
  Func_02003088(8, 0, 0);
  Func_02003088(9, 0, 0);
  Func_02003088(10, 0, 0);
  wait(1);
  goto test;
  do {
    s32 id = list[0];
    struct Actor *object = Actor_GetById(id);
    s32 flag = list[1];
    s32 nextFlag = flag + 1;
    struct MapCell *cells = map->layers[object->terrainId].cells;
    u8 *shapes = gMapShapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);

    object->unk5c[0] = 3;
    if (GameFlag_Test(nextFlag)) {
      Func_02003088(id, 0, 0);
    } else {
      s32 index = Func_02003160(object);
      s32 shape;
      s32 ground;
      cells += index;
      shapes += index;
      shape = Func_02003010(object->terrainId, object->x, object->z);
      ground = Map_GetTerrainHeight(object->terrainId, object->x, object->z);
      Func_020030c0(id);
      cells->attributeB = 128;
      *shapes = Func_02003180(object->terrainId, shape, (ground >> 19) + 4);
      if (GameFlag_Test(flag))
        ObjectVisual_CopyAttributes(id, 9);
    }
    list += 2;
  test:;
  } while (list[0] != 0xffff);
  wait(10);
}
