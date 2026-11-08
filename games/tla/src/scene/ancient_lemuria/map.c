#include "scene.h"

void Func_020000b0(void) { WORD_AT(Ram_HeapSlots->mapWork, 240) = 0x02ee0000; }

void Func_020000c4(void) { WORD_AT(Ram_HeapSlots->mapWork, 240) = 0x01600000; }

void Func_020000d4(void) {}

void Func_020000d8(s32 unused, s32 id) {
  struct FieldActor *object = Actor_GetById(id);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;

  if (x == 8 && z == 36)
    GameFlag_SetBit(0x961);
  if (GameFlag_Test(0x961))
    Func_02000838();
}
