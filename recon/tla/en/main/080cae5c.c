// Draft, differ 14 of 129, skeleton gap 7 (tla-en, 2026-11-01)
// Twin of The Broken Seal's Actor_Restore (games/tbs/src/field/field_sequence/
// actors.c): same snapshot layout, same loop, same camera re-anchor for the
// leader. This game adds the x, z and Actor_ResetMotion on that path, always
// sets the animation, and asks Party_GetLeaderActor rather than reading it.
// Left: sched2 order of the pointer setup before the loop only. The sched2
// dump ranks the frames chain (r2: movs #128, priority 11) above the indices
// chain (r3: negs, 10). The listing's order needs the store to [sp] to cost
// the first ldrb from indices about 3 cycles, which puts indices above frames
// and also explains the listing's movs #129 before sub sp; the
// -mthumb-slow-loads store rule skips extended loads, so it costs 1.
// Tried, all still differ 14: Actor_Restore's struct ActorSnapshot and its
// initializers, its temporary for the animation byte, u8 or u32 index, split
// or combined index reads, count initialized at its declaration, no empty asm
// before any one statement, any order of the five pointer initializations,
// and a 4-minute permuter run. Moving indices first or the index read before
// count = 0 is worse.
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
