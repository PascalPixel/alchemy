#include "types.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "object_runtime.h"
#include "field_sprite.h"
#include "map.h"
#include "dma.h"

void ObjectDispatch_SetSingleChildFlagsFar(struct ObjectRuntime *object,
                                           s32 value);

extern struct ObjectRuntime gSnapshotObjects[];

struct ObjectRuntime *ObjectTable_Get(u32 objectId);
void ObjectTable_Restore(void) {
  struct ObjectRuntime *copy = gSnapshotObjects;
  u8 *indices = (u8 *)gSnapshotObjects - 32;
  u8 *frames = (u8 *)&gSnapshotObjects[32];
  u8 *nextFrames = frames + 32;
  u8 *priorities = frames + 64;
  s32 count;
  struct ObjectRuntime *object;
  struct FieldSprite *sprite;
  s32 *view;
  struct ObjectRuntime *camera;
  s32 index;
  s32 priority;
  s32 y;

  count = 0;
  index = *indices++;
  /* FAKEMATCH: a scheduling barrier the native order needs. */
  __asm__("");
  while (index != 255) {
    object = ObjectTable_Get(index);
    if (object != NULL) {
      sprite = object->animation;
      Dma_Set(copy, object, 0x84000000 | (sizeof(struct ObjectRuntime) / 4),
              (volatile u32 *)0x040000d4);
      object->animation = sprite;
      Object_SetMode(object, *frames);
      ObjectDispatch_SetSingleChildFlagsFar(object, *nextFrames);
      priority = *priorities;
      sprite->priority = priority;
      sprite->partPriority = priority;
      if (index == EventRuntime_GetControlledOwner()) {
        camera = ((struct EventWork *)Ram_HeapSlots->eventWork)->viewCenter;
        view = Ram_HeapSlots->mapWork->origin;
        y = object->y;
        camera->x = object->x;
        camera->terrainHeight = y;
        camera->y = y;
        camera->z = object->z;
        view[1] = y;
        Object_ResetMotion(object);
      }
    }
    copy++;
    frames++;
    nextFrames++;
    count++;
    priorities++;
    if (count > 31)
      break;
    index = *indices++;
  }
}
