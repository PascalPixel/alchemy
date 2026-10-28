#include "islets.h"

void Func_020024c8(void) {
  Call6((void (*)())Func_02003e24, 5, 2, 1, 2, 5, 28);
  GameFlag_SetBit(0x213);
}

void Func_020024f0(void) {
  struct Actor *object;

  if (gInput.held & 0x40) {
    object = Actor_GetById(gSequenceParentWork.currentOwner);
    Func_02003e64();
    Func_02003fa4(0);
    object->motionFlags = 0;
    Call3((void (*)())Actor_SetPositionAndReset,
          gSequenceParentWork.currentOwner, 72, 280);
    object->motionFlags = 3;
    Func_02003e6c();
  }
}

void Func_0200253c(void) {
  struct Actor *object;
  u16 *state;
  /* FAKEMATCH(pin): the state word in r3, held by the barrier below; as a
   * plain u32 local, 2 of 41 instructions differ in tla-en. */
  register u32 value asm("r3");

  if (gInput.held & 0x80) {
    state = gSceneState;
    object = Actor_GetById(gSequenceParentWork.currentOwner);
    value = *state;
    /* FAKEMATCH(barrier): clobbering r2 loads the word before 0xfff6 is built
     * in r2; as the plain load, 2 of 41 instructions differ in tla-en. */
    __asm__("" : "+l"(value) : : "r2");
    if ((u16)(value + 0xfff6) > 200) {
      Func_02003e64();
      Func_02003fa4(0);
      object->motionFlags = 0;
      Call3((void (*)())Actor_SetPositionAndReset,
            gSequenceParentWork.currentOwner, 72, 296);
      object->motionFlags = 3;
      Func_02003e6c();
    }
  }
}
