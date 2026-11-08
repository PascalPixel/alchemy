#include "scene.h"

void Func_020000b0(void) { WORD_AT(Ram_HeapSlots->mapWork, 240) = 0x02ee0000; }

void Func_020000c4(void) { WORD_AT(Ram_HeapSlots->mapWork, 240) = 0x01600000; }

void Func_020000d4(void) {}

void Func_020000d8(s32 unused, s32 id) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 8 of 23 instructions
   * differ in tla-en. */
  struct FieldActor *object = Actor_GetById(id);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;

  if (x == 8 && z == 36)
    GameFlag_SetBit(SPLIT_CONSTANT(0x900, 97));
  if (GameFlag_Test(SPLIT_CONSTANT(0x900, 97)))
    Func_02000838();
}
