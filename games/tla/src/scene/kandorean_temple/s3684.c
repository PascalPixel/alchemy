#include "local.h"

void Func_02000cd8(s32 id, s32 dy, s32 scale);
extern s32 gData_020062a0;
extern s32 gData_020062a4;

/// The leader and actor 13 rise into view while actor 15 scales.
void Func_02003684(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *first = Actor_GetById(13);
  /* FAKEMATCH: the second actor pinned to r6 leaves the leader in r8 as the
   * native has; unpinned, the two trade registers (differ 24). */
  register struct Actor *second asm("r6") = Actor_GetById(15);

  Func_020051c0();
  Func_02005370(0);
  Func_02005388(162);
  Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
  wait_(1);
  leader->motionFlags = 0;
  Actor_SetPositionAndResetMotion_(Actor_GetById(gMainRoutineWork.currentOwner),
                                    PIXELS(440), PIXELS(-20), PIXELS(200));
  Actor_SetSingleChildFlags_(Actor_GetById(gMainRoutineWork.currentOwner),
                                      0);
  Actor_SetPositionAndResetMotion_(Actor_GetById_(13), PIXELS(440), PIXELS(-20),
                                    PIXELS(204));
  Actor_SetPositionAndResetMotion_(Actor_GetById_(15), PIXELS(440), PIXELS(-20),
                                    PIXELS(200));
  Actor_SetSpritePriority_(13, 2);
  Actor_SetSpritePriority_(15, 3);
  {
    u8 *flags = &second->priorityFlags;
    s32 c = 2 | *flags;

    *flags = c;
  }
  second->scaleY = 0x400;
  Actor_SetModeById_(13, 4);
  Actor_SetModeById_(15, 2);
  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 521;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  // A goto loop, as in Func_0200393c.
rise:
  first->y += PIXELS(4);
  leader->y += PIXELS(4);
  Func_02000cd8(15, 0x4ccc, 0x4ccc);
  wait_(1);
  /* FAKEMATCH: as in Func_0200393c, a volatile read for the test. */
  if (*(volatile s32 *)&first->y < 0)
    goto rise;
  Func_02005178_(0, 0, 1, 1, 27, 12);
  gData_020062a0 = 2000;
  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 516;
  Func_020051c8();
}

/// As Func_02003684, for actors 14 and 16 at another place.
void Func_020037e0(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *first = Actor_GetById(14);
  /* FAKEMATCH: as in Func_02003684, the second actor pinned to r6. */
  register struct Actor *second asm("r6") = Actor_GetById(16);

  Func_020051c0();
  Func_02005370(0);
  Func_02005388(162);
  Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
  wait_(1);
  leader->motionFlags = 0;
  Actor_SetPositionAndResetMotion_(Actor_GetById(gMainRoutineWork.currentOwner),
                                    PIXELS(600), PIXELS(-20), PIXELS(232));
  Actor_SetSingleChildFlags_(Actor_GetById(gMainRoutineWork.currentOwner),
                                      0);
  Actor_SetPositionAndResetMotion_(Actor_GetById_(14), PIXELS(600), PIXELS(-20),
                                    PIXELS(236));
  Actor_SetPositionAndResetMotion_(Actor_GetById_(16), PIXELS(600), PIXELS(-20),
                                    PIXELS(232));
  Actor_SetSpritePriority_(14, 2);
  Actor_SetSpritePriority_(16, 3);
  {
    u8 *flags = &second->priorityFlags;
    s32 c = 2 | *flags;

    *flags = c;
  }
  second->scaleY = 0x400;
  Actor_SetModeById_(14, 4);
  Actor_SetModeById_(16, 2);
  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 521;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  // A goto loop, as in Func_0200393c.
rise:
  first->y += PIXELS(4);
  leader->y += PIXELS(4);
  Func_02000cd8(16, 0x4ccc, 0x4ccc);
  wait_(1);
  /* FAKEMATCH: as in Func_0200393c, a volatile read for the test. */
  if (*(volatile s32 *)&first->y < 0)
    goto rise;
  Func_02005178_(0, 0, 1, 1, 37, 14);
  gData_020062a4 = 2000;
  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 516;
  Func_020051c8();
}

/// Actor 13 rises out of view while actor 15 scales, then the scene moves on.
void Func_0200393c(void) {
  struct Actor *object = Actor_GetById(13);

  Func_020051c0();
  Func_02005370(0);
  // A goto loop: as a do-while, loop.c hoists the scale constant into r6
  // (differ 19).
rise:
  object->y -= PIXELS(4);
  Func_02000cf8(15, 0x4ccc, 0x4ccc);
  wait(1);
  /* FAKEMATCH: a volatile read for the test keeps GCSE from carrying y around
   * the loop in r2; the native loads it again at the top (differ 13 plain). */
  if (*(volatile s32 *)&object->y > -0x140000)
    goto rise;
  Func_02005230(15, 0, 0);
  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 521;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_020052f8(6);
}

/// As Func_0200393c, for actors 14 and 16.
void Func_020039a8(void) {
  struct Actor *object = Actor_GetById(14);

  Func_020051c0();
  Func_02005370(0);
  // A goto loop, as in Func_0200393c.
rise:
  object->y -= PIXELS(4);
  Func_02000cf8(16, 0x4ccc, 0x4ccc);
  wait(1);
  /* FAKEMATCH: as in Func_0200393c, a volatile read for the test (differ 13
   * plain). */
  if (*(volatile s32 *)&object->y > -0x140000)
    goto rise;
  Func_02005230(16, 0, 0);
  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 521;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_020052f8(7);
}
