#include "types.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "actor.h"
#include "field_sprite.h"
#include "map.h"
#include "dma.h"
#include "io_reg.h"

void Actor_SetSingleChildFlagsFar(struct Actor *object, s32 value);

extern struct Actor gSnapshotObjects[];

struct Actor *ObjectTable_Get(u32 objectId);
void ObjectTable_Restore(void) {
  struct Actor *copy = gSnapshotObjects;
  u8 *indices = (u8 *)gSnapshotObjects - 32;
  u8 *frames = (u8 *)&gSnapshotObjects[32];
  u8 *nextFrames = frames + 32;
  u8 *priorities = frames + 64;
  s32 count;
  struct Actor *object;
  struct FieldSprite *sprite;
  s32 *view;
  struct Actor *camera;
  s32 index;
  s32 priority;
  s32 y;

  count = 0;
  index = *indices++;
  while (index != 255) {
    object = ObjectTable_Get(index);
    if (object != NULL) {
      sprite = object->animation;
      Dma_Set(copy, object, DMA_COPY_WORDS(sizeof(struct Actor) / 4), REG_DMA3);
      object->animation = sprite;
      Actor_SetMode(object, *frames);
      Actor_SetSingleChildFlagsFar(object, *nextFrames);
      priority = *priorities;
      sprite->priority = priority;
      sprite->partPriority = priority;
      if (index == Party_GetLeaderActor()) {
        camera = ((struct EventWork *)Ram_HeapSlots->eventWork)->viewCenter;
        view = Ram_HeapSlots->mapWork->origin;
        y = object->y;
        camera->x = object->x;
        camera->terrainHeight = y;
        camera->y = y;
        camera->z = object->z;
        view[1] = y;
        Actor_ResetMotion(object);
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
