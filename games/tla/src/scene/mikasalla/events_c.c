#include "local.h"
#include "event_inline.h"

void Func_020004ec(void) {
  s32 id;

  Func_02000648();
  Event_ResolvePendingActions(0);
  Actor_WalkByAndWait(4, 0, -14);
  id = 4;
  Actor_CommitPositionAndActivate(id, 16, 0);
  Func_02000438();
}

void Func_02000514(void) {
  s32 id, x, z;

  Func_02000648();
  Event_ResolvePendingActions(0);
  Actor_WalkByAndWait(4, -2, -6);
  x = 14;
  z = 8;
  id = 4;
  /* FAKEMATCH(barrier): the id held at an empty asm, so it comes between the
   * distances and their negation; as no asm, not measured. */
  __asm__ volatile("" : "+r"(id));
  Actor_CommitPositionAndActivate(id, -x, -z);
  Func_02000438();
}

void Func_02000544(void) {
  s32 id, a;

  Func_02000648();
  Event_ResolvePendingActions(0);
  a = 192;
  id = 4;
  /* FAKEMATCH(barrier): the id held at an empty asm, so it comes between the
   * angle byte and its shift; as no asm, not measured. */
  __asm__ volatile("" : "+r"(id));
  Actor_ArmCallback(id, a << 8, 0);
  Func_02000438();
}

void Func_02000564(void) {
  s32 x = ((s16 *)&Actor_GetById(4)->x)[1];

  Func_02000648();
  Event_ResolvePendingActions(0);
  if (x % 16 <= 7) {
    Actor_WalkByAndWait(4, -4, -4);
    Actor_WalkByAndWait(4, -4, -8);
  } else {
    Actor_WalkByAndWait(4, 4, -4);
    Actor_WalkByAndWait(4, 4, -8);
  }
  Actor_WalkToAndWait(4, 424, 99);
  Func_02000438();
}

void Func_020005d4(void) {
  Func_02000648();
  Event_ResolvePendingActions(0);
  Actor_WalkByAndWait(4, 0, -4);
  Actor_WalkToAndWait(4, 424, 99);
  Func_02000438();
}
