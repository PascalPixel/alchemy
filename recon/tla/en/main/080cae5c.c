// Near miss, differ 14 of 129 (tla-en, 2026-10-31): sched2 order of the
// pointer setup before the loop only. The sched2 dump ranks the frames chain
// (r2: movs #128, priority 11) above the indices chain (r3: negs, 10); the
// stores to the two stack slots cost the first ldrb from indices only 1,
// since the -mthumb-slow-loads store rule skips extended loads, and the
// listing's order needs the indices chain ranked at least two higher. No
// empty asm before any one statement lowers it, nor does any order of the
// five pointer initializations, a u8 or u32 index, or a 4-minute permuter run.
#include "types.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "actor.h"
#include "field_sprite.h"
#include "map.h"
#include "dma.h"
#include "io_reg.h"

void Actor_SetSingleChildFlagsFar(struct Actor *object,
                                           s32 value);

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
