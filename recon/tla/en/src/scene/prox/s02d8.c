#include "types.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "actor.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

s32 SceneActor_CalculateFixedPointDistance(s32 *a, s32 *b);
s32 ArcTan2(s32 y, s32 x);
void Func_020043c4(struct Actor *object, s32 mode);

s32 Func_020002d8(struct Actor *object, struct Actor *target, s32 range,
                  s32 force) {
  s32 result = 0;
  s32 *targetPos;
  s32 *objectPos;

  if (BYTE_AT(object, 99) != 0) {
    object->movementState = 1;
    return 1;
  }
  targetPos = &target->x;
  objectPos = &object->x;
  if (SceneActor_CalculateFixedPointDistance(targetPos, objectPos) < range ||
      force != 0) {
    u32 angle = (u16)ArcTan2(target->z - object->z, *targetPos - *objectPos);
    u32 left = (angle - 0x1000) & 0xf000;
    u32 right = (angle + 0x1000) & 0xf000;
    u32 ahead = angle & 0xf000;
    u32 facing = object->angle & 0xf000;

    if (ahead == facing || right == facing || left == facing || force != 0) {
      /* FAKEMATCH(temporary): the count and movementState's address held in
       * locals before the test; as blockedSteps read in the test and
       * object->movementState at each store, 16 of 107 instructions differ in
       * tla-en. */
      s16 count = ((struct EventWork *)Ram_HeapSlots->eventWork)->blockedSteps;
      u8 *movementState = &object->movementState;

      if (count > 120) {
        *movementState = 0;
        Func_020043c4(object, 2);
      } else {
        *movementState = 1;
        Func_020043c4(object, 1);
        result = 1;
      }
    }
  } else {
    object->movementState = 0;
    Func_020043c4(object, 2);
  }
  return result;
}
