#include "types.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "object_runtime.h"

void EventRuntime_SetMessage(s32 message) {
  struct EventWork *event = Ram_HeapSlots->eventWork;

  event->messageId = message;
}

s32 ObjectTable_ReadActiveValue(s32 key) {
  struct EventWork *event = Ram_HeapSlots->eventWork;
  s32 result = -1;
  struct ObjectRuntime *object = event->objects[key & 0x0fff];

  if (object != NULL && object->animationKind == 1)
    result = *((struct BattleAnimationState *)object->animation)->value28;
  return result;
}

s32 ObjectTable_FindActiveByValue(s32 value) {
  struct EventWork *event = Ram_HeapSlots->eventWork;
  s32 result = -1;
  s32 index = 8;
  struct ObjectRuntime *object = event->objects[index];

  if (object != NULL && object->animationKind == 1 &&
      *((struct BattleAnimationState *)object->animation)->value28 == value) {
    result = index;
  } else {
  next:
    index++;
    if (index <= 79) {
      object = event->objects[index];
      if (object == NULL || object->animationKind != 1 ||
          *((struct BattleAnimationState *)object->animation)->value28 != value)
        goto next;
      result = index;
    }
  }
  return result;
}
