#include "scene.h"

void Func_02002d6c(struct FieldActor *object, s32 x, s32 z, s32 mode) {
  struct MainRoutineWork *party;
  s32 zero;
  u8 *flags;

  Func_02004040();
  Func_020041b8(0);
  flags = &object->motionFlags;
  zero = 0;
  party = &gMainRoutineWork;
  *flags = zero;
  Actor_SetSpeedParameters(party->currentOwner, 0x10000, 0x8000);
  Actor_SetPositionAndReset(party->currentOwner, x, z);
  Func_020040e8(party->currentOwner, 0xc000);
  Actor_SetSingleChildFlags(Actor_GetById(party->currentOwner), 0);
  Actor_SetModeById(party->currentOwner, 13);
  Func_02003fe0(object, object->x, object->y - 0xa0000, object->z);
  Func_02003fe8(object);
  Actor_SetModeById(party->currentOwner, 10);
  Func_020041f8(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02004138(mode);
}
