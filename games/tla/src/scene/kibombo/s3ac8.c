#include "kibombo.h"
#include "input.h"

void Func_02003ac8(void) {
  struct KibomboObject *leader = Object_GetById(gPartyState.currentOwner);
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;

  if (x == 42 && z == 23 && (gInput.held & 64) == 0)
    return;
  if (x == 43 && z == 22 && (gInput.held & 32) == 0)
    return;
  Func_02003a08(680, 352);
}

void Func_02003b20(void) {
  struct KibomboObject *leader = Object_GetById(gPartyState.currentOwner);
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;

  if (z == 38) {
    if (x == 23 && (gInput.held & 16) == 0)
      return;
    if (x == 25 && (gInput.held & 32) == 0)
      return;
  } else if (x == 24 && z == 39 && (gInput.held & 64) == 0) {
    return;
  }
  Func_02003a08(392, 608);
}

void Func_02003b88(void) {
  struct KibomboObject *leader = Object_GetById(gPartyState.currentOwner);
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;

  if (z == 54) {
    if (x == 42 && (gInput.held & 16) == 0)
      return;
    if (x == 44 && (gInput.held & 32) == 0)
      return;
  } else if (x == 43 && z == 55 && (gInput.held & 64) == 0) {
    return;
  }
  Func_02003a08(696, 864);
}

void Func_02003bf0(void) {
  if (GameFlag_Test(0x202))
    Func_02003898();
  Func_02003a08(376, 288);
}

// Fades the screen in and runs Func_02002d64 each frame for up to 160 frames,
// or until A, B or Start is held, then fades out.
void Func_02003c14(void) {
  u32 i;

  Func_02005a9c();
  Func_02005c3c(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  KIBOMBO_TASK_ADD(Func_02002d64, 0x480);
  Battle_WaitMode0(20);
  // As a for loop, loop.c keeps gInput's address and the mask in registers
  // across the wait; the native builds both again each frame (differ 18).
  i = 0;
  goto test;
next:
  i++;
test:
  if (i > 159)
    goto done;
  Battle_WaitMode0(1);
  if ((gInput.held & 11) == 0)
    goto next;
done:
  Scheduler_RemoveCallbackFar(Func_02002d64);
  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->field1b4 = 32;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02005bc4(11);
}
