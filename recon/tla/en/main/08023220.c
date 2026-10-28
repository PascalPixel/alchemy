// Draft, differ 2 of 181 in tla-en under -mthumb-slow-loads (2026-10-26). No
// single empty asm, one-pass block, wrapping block or swap of neighbouring
// statements brings it closer (device search).
// Near miss, differ 8 of 181: ☀️'s FieldObject_Create (games/tbs/src/field/
// field.c) on The Lost Age's object, which also clears the four words at its
// end. Left: the native loads the halfword zero for collisionFlags from the
// pool right after the speedLimit store (sched2 here ranks it 6 against the
// step zero's 7 and takes it seven rows later), which also moves the pool one
// branch earlier (two pool offsets); and it stores the second sprite slot's
// address after the first slot (add r2, sl one row later). Measured: the
// same pin and barrier as the animationKind zero cannot hold it, as the
// barrier would stop the 0x10000 and the motionFlags address the native
// builds before the load; *entry = sprite; entry++; is the same as
// *entry++.
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
        /* FAKEMATCH: the native loads the halfword zero from the pool before
         * it addresses animationKind (differ 2 without the pin and barrier).
         */
        register u16 none asm("r3") = 0;

        /* FAKEMATCH: the barrier of the pin above. */
        asm volatile("" : "+l"(none));
        object->animationKind = none;
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
