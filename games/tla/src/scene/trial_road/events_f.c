#include "trial_road.h"
#include "call.h"

void Func_020008a4(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 0 of 16 instructions
   * differ in tla-en. */
  if (GameFlag_Test(Func_020059b8(gMainRoutineWork.currentOwner) +
                    SPLIT_CONSTANT(0x203)))
    Func_020059c0();
}

void Func_020008d0(s32 a, s32 id) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 0 of 23 instructions
   * differ in tla-en. */
  struct Actor *object = Actor_GetById(id);

  Func_0200070c(id, 16);
  GameFlag_ClearBit(id + SPLIT_CONSTANT(0x203));
  if (object != NULL)
    object->priorityFlags &= 0xfd;
}

void Func_02000904(s32 a, s32 id) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 0 of 14 instructions
   * differ in tla-en. */
  OBJECT_BYTE(Actor_GetById(id - 5), 98) = 0;
  GameFlag_SetBit(id + SPLIT_CONSTANT(0x313));
}

void Func_02000924(s32 id) {
  struct Actor *object = Actor_GetById(id);

  if (object != NULL) {
    Func_0200070c(id, -16);
    object->collisionFlags = 0;
    object->priorityFlags |= 2;
    Func_020057c8(object, 6);
  }
}

void Func_0200095c(s32 id) {
  struct Actor *object = Actor_GetById(id);

  if (object != NULL) {
    Func_0200070c(id, 16);
    object->collisionFlags = 1;
    object->priorityFlags &= 0xfd;
    Func_020057c8(object, 5);
  }
}
