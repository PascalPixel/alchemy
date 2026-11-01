#include "types.h"
#include "actor.h"
#include "sprite_metadata.h"
#include "actor_view.h"
#include "sprite.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "dma.h"

void *ResourceObject_Create(s32 id);
void *Actor_FindFreeObject(void);

struct Actor *Func_08023220(s32 id, s32 x, s32 y, s32 z) {
  struct Actor *object;
  void *sprite;
  s32 kind;
  void **entry;
  volatile u32 zero;

  kind = id / 4096;
  id &= 0xfff;
  object = Actor_FindFreeObject();
  if (object != NULL) {
    object->radius = 16;
    switch (kind) {
    case 0:
      sprite = ResourceObject_Create(id);
      if (sprite != NULL) {
        object->animationKind = 1;
        object->animation = sprite;
        object->radius = Resource_GetMetadataRecordFar(id)->boxY >> 1;
      } else {
        object->animationKind = 0;
      }
      break;
    case 2:
      entry = (void **)((u32 *)Ram_HeapSlots->objectSystem +
                        Ram_HeapSlots->objectSystem->count++ + 2);
      object->animationKind = kind;
      object->animation = entry;
      zero = 0;
      Dma_Set(&zero, entry, 0x85000004, REG_DMA3);
      sprite = ResourceObject_Create(id);
      if (sprite != NULL) {
        object->radius = Resource_GetMetadataRecordFar(id)->boxY >> 1;
        *entry = sprite;
        entry++;
      }
      sprite = ResourceObject_Create(id + 1);
      if (sprite != NULL)
        *entry = sprite;
      break;
    }
  }
  if (object != NULL) {
    Actor_SetPositionAndResetMotion(object, x, y, z);
    object->script = (s32 *)gActorScript_EndScript;
    object->speedLimit = 0x20000;
    object->step = 0;
    object->scaleX = 0x10000;
    object->scaleY = 0x10000;
    object->acceleration = 0x10000;
    object->motionFlags = 3;
    object->gravity = 0x10000;
    object->bounce = 0x4000;
    object->collisionFlags = 0;
    object->actionFlags = 1;
    object->angle = 0x4000;
    object->unk4c = 0;
    object->action = x / 65536;
    object->unk66 = z / 65536;
    *(s32 *)&object->unk70[12] = 0;
    *(s32 *)&object->unk70[0] = 0;
    *(s32 *)&object->unk70[4] = 0;
    *(s32 *)&object->unk70[8] = 0;
  }
  return object;
}
