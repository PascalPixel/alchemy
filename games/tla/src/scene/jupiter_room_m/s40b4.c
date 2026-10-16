#include "jupiter_room_m.h"

void Func_020040b4(void) {
  struct ObjectRuntime *object = Object_GetById(gPartyState.currentOwner);
  s32 x = object->x;
  s32 z = object->z;

  ObjectMotion_SetSpeedParameters(gPartyState.currentOwner, 0x40000, 0x20000);
  /* FAKEMATCH: the loop note ends the scheduling block: ⚓️ loads x and z before
   * it copies the object to r10 (one reordered row). */
  do {
  } while (0);
  x >>= 20;
  z >>= 20;
  ObjectMotion_SetPositionAndCommit(gPartyState.currentOwner, (x << 4) + 8,
                                    (z << 4) + 8);
  WaitFrames(1);
  Object_SetPositionAndResetMotion(object, (x << 20) + 0x80000, object->y,
                                   (z << 20) + 0x80000);
  FieldEvent_WaitFrames(4);
  Func_020048fc()->y -= 0x80000;
}
