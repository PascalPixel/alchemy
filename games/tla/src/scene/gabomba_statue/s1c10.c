#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "task.h"
#include "event_inline.h"

extern u32 gData_02005910[];
extern u8 Data_0202c001[];
void Actor_SetSingleChildFlags(struct Actor *object, u32 value);
void Func_02001b98(void);

#define MARK(at, cellPicture, shape)                                           \
  (cell = (at), layerA[cell] = (cellPicture), layerB[cell] = (cellPicture),    \
   word = &shapesA[cell], *word |= (shape), word = &shapesB[cell],             \
   *word |= (shape))

void Func_02001c10(void) {
  u32 *dest = (u32 *)(Data_0202c001 + 0x1cf);
  u32 *src = gData_02005910;
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  u8 *layerA = *(u8 **)(map + 0x13c);
  u8 *layerB = *(u8 **)(map + 0x174);
  u32 *shapesA = *(u32 **)(map + 0x138);
  u32 *shapesB = *(u32 **)(map + 0x170);
  struct Actor *object;
  s16 *frames;
  u32 *word;
  u32 i;
  s32 x, z, cell, found;

  i = 0;
  do {
    *dest++ = *src++;
  } while (++i <= 11);
  for (i = 8; (s32)i <= 63; i++) {
    object = Actor_GetById(i);
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
      word = &shapesA[cell];
      *word |= 0x80500000;
      word = &shapesB[cell];
      *word |= 0x80500000;
      found = 1;
      break;
    case 0:
      MARK(x + (z << 7) - 129, 118, 0x510000);
      MARK(x + (z << 7) - 128, 117, 0x80510000);
      MARK(x + (z << 7) - 127, 119, 0x510000);
      MARK(x + (z << 7) - 1, 117, 0x80510000);
      MARK(x + (z << 7), 117, 0x510000);
      MARK(x + (z << 7) + 1, 117, 0x80510000);
      MARK(x + (z << 7) + 127, 120, 0x510000);
      MARK(x + (z << 7) + 128, 117, 0x80510000);
      MARK(x + (z << 7) + 129, 121, 0x510000);
      found = 1;
      break;
    case 2:
      MARK(x + (z << 7) - 2, 127, 0x540000);
      MARK(x + (z << 7) - 1, 122, 0x529000);
      MARK(x + (z << 7), 117, 0x80526000);
      MARK(x + (z << 7) + 1, 123, 0x529000);
      MARK(x + (z << 7) + 2, 127, 0x540000);
      found = 1;
      break;
    case 4:
      MARK(x + (z << 7) - 256, 127, 0x540000);
      MARK(x + (z << 7) - 128, 124, 0x539000);
      MARK(x + (z << 7), 117, 0x539000);
      MARK(x + (z << 7) + 128, 125, 0x539000);
      MARK(x + (z << 7) + 256, 126, 0x80550000);
      found = 1;
      break;
    case 3:
      MARK(x + (z << 7) - 256, 127, 0x540000);
      MARK(x + (z << 7) - 128, 124, 0x539000);
      MARK(x + (z << 7), 117, 0x539000);
      MARK(x + (z << 7) + 128, 125, 0x539000);
      MARK(x + (z << 7) + 256, 127, 0x540000);
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
      Actor_SetSingleChildFlags(object, 0);
    }
  }
  Task_Add(Func_02001b98, TASK_PASS_VBLANK);
}
