#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "event_inline.h"

extern struct Actor *gData_02004420;

void *HeapSlot_AllocFar(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);

void Func_02003a30(void) {
  struct Actor **slot = &gData_02004420;
  /* FAKEMATCH(temporary): one zero held in a local, kept in r7 for the three
   * byte stores below as the native does; as plain zeros, built in r2 where
   * they are stored, 22 of 48 instructions differ in tla-en. */
  s32 zero = 0;
  struct Actor *object = ((struct Actor *)Import_ActorCreate(489, 0, 0, 0));

  *slot = object;
  if (object != 0) {
    struct FieldSprite *sprite = object->animation;
    u8 *buffer;

    sprite->fullColor = 0;
    sprite->palette = 0;
    object->motionFlags = zero;
    object->unk5c[0] = 1;
    sprite->nextFrame = zero;
    sprite->unk1b[0] = zero;
    buffer = HeapSlot_AllocFar(68, 0x608);
    Icon_DrawItem(199);
    VramCache_Load(sprite->vramBlock, 128, buffer + 0x400);
    HeapSlot_Release(68);
  }
}
