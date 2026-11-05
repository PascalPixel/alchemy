// Draft, differ 58 of 150, skeleton gap 32 (tla-en, 2026-11-01)
// The variant of Func_020001d0 that first gathers people 8 to 10 on the leader
// and copies person 9's look onto each pillar whose second flag is set. The
// same function is overlay 68F's Func_020025e0 and 691's Func_020030bc.
// Left is allocation: the listing has the terrain pointer in r7, the cells in
// r6, the index in r8, and the cell pointer in a stack slot; here r6 goes to
// the index, so the terrain pointer falls to a high register. Keeping a
// separate `cell = cells + index` ties its shift temporary to the cell pointer
// (local-alloc), which takes r6 from the cells and pushes `list` to the stack
// (differ 90); the listing's temporary stays in r3, so its cell pointer is
// probably not local to the block, but reusing the variable before the branch
// ranked it too high (differ 83). A real while or for duplicates the test.
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "map.h"

extern u8 gMapShapeGrid[];

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void wait(s32 frames);
void Func_020014a8(s32 id, s32 target);
void Func_0200056c(s32 id);
void Actor_SetModeById(u32 id, s32 mode);
void Func_020014a0(s32 id, s32 x, s32 z);
s32 Func_02001508(struct Actor *object);
s32 Func_02001478(s32 layer, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_020014c0(s32 id);
s32 Func_02001528(s32 layer, s32 shape, s32 height);
void ObjectVisual_CopyAttributes(u32 targetId, u32 sourceId);

void Func_02000584(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;

  Func_020014a8(8, gMainRoutineWork.currentOwner);
  Func_020014a8(9, gMainRoutineWork.currentOwner);
  Func_020014a8(10, gMainRoutineWork.currentOwner);
  wait(1);
  Func_0200056c(8);
  Func_0200056c(9);
  Func_0200056c(10);
  Actor_SetModeById(9, 0);
  wait(1);
  Func_020014a0(8, 0, 0);
  Func_020014a0(9, 0, 0);
  Func_020014a0(10, 0, 0);
  wait(1);
  while (list[0] != 0xffff) {
  {
    u32 id = list[0];
    struct Actor *object = Actor_GetById(id);
    s32 flag = list[1];
    u8 *terrain = &object->terrainId;
    s32 next = flag + 1;
    struct MapCell *cells = map->layers[*terrain].cells;
    u8 *shapes = gMapShapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);

    object->unk5c[0] = 3;
    if (GameFlag_Test(next)) {
      Func_020014a0(id, 0, 0);
    } else {
      s32 index = Func_02001508(object);
      u8 *shape = shapes + index;
      s32 kind, ground;

      cells += index;
      kind = Func_02001478(*terrain, object->x, object->z);
      ground = Map_GetTerrainHeight(*terrain, object->x, object->z);

      Func_020014c0(id);
      cells->attributeB = 128;
      *shape = Func_02001528(*terrain, kind, (ground >> 19) + 4);
      if (GameFlag_Test(flag))
        ObjectVisual_CopyAttributes(id, 9);
    }
    list += 2;
  }
  }
  wait(10);
}
