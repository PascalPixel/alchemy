// Draft, differ 907 of 522: copies gData_02005910's twelve words to
// Data_0202c001 + 0x1cf, then for objects 8 to 63 with one sprite whose
// sprite's frame list (its word 0x28) begins with picture 357 to 362 marks
// the cells under it in both map layers (the map work's layers at 0x13c
// and 0x174, metatiles 116 to 127) and ORs its shapes into both shape
// grids (0x138 and 0x170): a 3 by 3 platform for 357, one cell for 358, a
// row of five for 359, a column of five for 360 and 361 (361 ending in
// 126 instead of 127), none for 362. Each such object is then made
// still, its flags cleared. Then it schedules Func_02001b98.
// Left: a rough reading: the alignment is poor; the native keeps the four grids
// in r9, r10, r8 and r5 and the object in r11 with a 4-byte frame (the draft
// spills a layer), shares the last cell's shape stores between cases by
// cross-jumping and keeps the shapes in r0, r4 and lr across a case.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

extern u32 gData_02005910[];
extern u8 Data_0202c001[];
struct ObjectRuntime *Object_GetById(u32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        u32 value);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Func_02001b98(void);

#define MARK(at, metatile, shape)                                              \
  (layerA[at] = (metatile), layerB[at] = (metatile), shapesA[at] |= (shape),   \
   shapesB[at] |= (shape))

void Func_02001c10(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  u8 *layerA = *(u8 **)(map + 0x13c);
  u8 *layerB = *(u8 **)(map + 0x174);
  u32 *shapesA = *(u32 **)(map + 0x138);
  u32 *shapesB = *(u32 **)(map + 0x170);
  u32 *dest = (u32 *)(Data_0202c001 + 0x1cf);
  u32 *src = gData_02005910;
  struct ObjectRuntime *object;
  s16 *frames;
  u32 i;
  s32 x, z, cell, found;

  i = 0;
  do {
    *dest++ = *src++;
  } while (++i <= 11);
  for (i = 8; (s32)i <= 63; i++) {
    object = Object_GetById(i);
    if (object == 0)
      continue;
    if (object->animationKind != 1)
      continue;
    frames = *(s16 **)((u8 *)object->animation + 40);
    if (frames == 0)
      continue;
    found = 0;
    x = object->x >> 20;
    z = object->z >> 20;
    switch (*frames - 357) {
    case 1:
      cell = x + (z << 7);
      layerA[cell] = 116;
      layerB[cell] = 117;
      shapesA[cell] |= 0x80500000;
      shapesB[cell] |= 0x80500000;
      found = 1;
      break;
    case 0:
      cell = x + (z << 7);
      MARK(cell - 129, 118, 0x510000);
      MARK(cell - 128, 117, 0x80510000);
      MARK(cell - 127, 119, 0x510000);
      MARK(cell - 1, 117, 0x80510000);
      MARK(cell, 117, 0x510000);
      MARK(cell + 1, 117, 0x80510000);
      MARK(cell + 127, 120, 0x510000);
      MARK(cell + 128, 117, 0x80510000);
      MARK(cell + 129, 121, 0x510000);
      found = 1;
      break;
    case 2:
      cell = x + (z << 7);
      MARK(cell - 2, 127, 0x540000);
      MARK(cell - 1, 122, 0x529000);
      MARK(cell, 117, 0x80526000);
      MARK(cell + 1, 123, 0x529000);
      MARK(cell + 2, 127, 0x540000);
      found = 1;
      break;
    case 4:
      cell = x + (z << 7);
      MARK(cell - 256, 127, 0x540000);
      MARK(cell - 128, 124, 0x539000);
      MARK(cell, 117, 0x539000);
      MARK(cell + 128, 125, 0x539000);
      MARK(cell + 256, 126, 0x80550000);
      found = 1;
      break;
    case 3:
      cell = x + (z << 7);
      MARK(cell - 256, 127, 0x540000);
      MARK(cell - 128, 124, 0x539000);
      MARK(cell, 117, 0x539000);
      MARK(cell + 128, 125, 0x539000);
      MARK(cell + 256, 127, 0x540000);
      found = 1;
      break;
    case 5:
      found = 1;
      break;
    }
    if (found) {
      object->collisionFlags = 0;
      object->motionFlags = 0;
      object->priorityFlags = 0;
      ObjectDispatch_SetSingleChildFlags(object, 0);
    }
  }
  Scheduler_AddOrUpdateCallback(Func_02001b98, 0x480);
}
