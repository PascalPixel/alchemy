#include "shaman_village.h"

u8 *HeapSlot_AllocFar(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
void Func_02004884(s32 resource);

struct ShamanVillageObject *Func_02001fec(void) {
  struct ShamanVillageObject *object =
      Import_ActorCreate(489, 0x780000, 0x400000, 0x1080000);
  u8 *buffer = NULL;

  if (object == NULL)
    return NULL;
  {
    struct FieldSprite *sprite = object->sprite;

    sprite->fullColor = 0;
    sprite->palette = 0;
    sprite->priority = 1;
    object->motionFlags = 0;
    object->field5c = 1;
    sprite->nextFrame = 0;
    sprite->unk1b[0] = 0;
    buffer = HeapSlot_AllocFar(68, 0x608);
    Func_02004884(65);
    VramCache_Load(sprite->vramBlock, 128, buffer + 0x400);
    HeapSlot_Release(68);
  }
  return object;
}
