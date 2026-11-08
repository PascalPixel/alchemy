#include "mars_lighthouse_aerie.h"
#include "event_work.h"
#include "scene_tables.h"

extern const u8 MsgWhatSTheMatter[];
struct Actor *Func_0200573c(void);
void Event_StartScreenTransition();
void Event_WaitTransitionFrames();
void Func_020053e4();
void Func_020054e4();
void Func_020055ac();
void Func_020055fc();
void Func_0200567c();
void Func_020056ec();
void Func_02005724();
void Func_02005734();
void Func_020057dc();
void GameFlag_SetBit();
void Event_AskYesNoWithActorAnimation();
void Camera_MoveWithinMapBounds();
void Motion_SetModeAndWaitAnimation();
void Actor_Jump();
void Actor_SetSpritePriority();
void Actor_SetPositionAndCommit();
void Actor_SetModeById();
void Actor_SetPositionAndResetMotion();
void Party_RemoveOwnerRestored();
void FieldMessage_ShowAtActor(s32 actor, s32 flags);
void wait();
extern u8 gData_02006b70[];
extern u8 gData_02006c20[];

void Func_02005714(s32 actor, s32 a, s32 b);

static __inline__ void Func_02005714_(s32 actor, s32 a, s32 b) {
  /* FAKEMATCH: pinned, the second and third arguments load before the actor, as
   * the native does at every call here (one reordered row per call without). */
  register s32 second asm("r1") = a;
  /* FAKEMATCH: the third argument, pinned with the second. */
  register s32 third asm("r2") = b;

  Func_02005714(actor, second, third);
}

void Func_0200298c(s32 id) {
  struct Actor *object = Actor_GetById(id);

  object->collisionFlags = 0;
  object->motionFlags = 0;
  Actor_SetSpritePriority(id, 1);
}

/// The scene that opens with MsgWhatSTheMatter; either answer to its prompt
/// steps the event's message number.
void Func_020029ac(void) {
  Func_0200560c();
  Func_02005784(0);
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  wait(1);
  Func_0200567c(gMainRoutineWork.currentOwner, PIXELS(506), PIXELS(280));
  wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetSpritePriority(3, 1);
  Func_020053e4((s32)Actor_GetById(7), (s32)gData_02006c20);
  Func_020053e4((s32)Actor_GetById(3), (s32)gData_02006b70);
  FieldEvent_Wait(120);
  Func_020054e4((s32)Actor_GetById(7));
  Func_020054e4((s32)Actor_GetById(3));
  FieldEvent_Wait(40);
  Motion_SetModeAndWaitAnimation(7, 4);
  Call2((void (*)())Func_02005704, 6, FACING_NORTH);
  Func_020056dc(MsgWhatSTheMatter);
  Func_020056f4(6, 0);
  Call2((void (*)())Func_02005704, 2, FACING_NORTHWEST);
  Func_020056f4(0x8002, 0);
  Call3((void (*)())Actor_ArmCallback, 3, FACING_SOUTHEAST, 20);
  Func_02005714_(3, 0x109, 0x14);
  Func_020056f4(0x1003, 0);
  Call2((void (*)())Func_02005704, 0, FACING_NORTHEAST);
  Motion_SetModeAndWaitAnimation(7, 4);
  Call2((void (*)())Func_020056f4, 0x2007, 0);
  Call3((void (*)())Actor_ArmCallback, gMainRoutineWork.currentOwner,
        FACING_WEST, 0);
  Call2((void (*)())Func_02005704, 5, FACING_NORTHEAST);
  Func_02005714_(5, 0x107, 0);
  Actor_Jump(5, 2, 0xa);
  Actor_Jump(5, 4, 0xa);
  Call2((void (*)())Func_020056f4, 0x8005, 0);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_SOUTHWEST, 0);
  Call3((void (*)())Actor_ArmCallback, 6, FACING_NORTHWEST, 0);
  Call3((void (*)())Actor_ArmCallback, 0, FACING_WEST, 0);
  Call3((void (*)())Actor_ArmCallback, 1, FACING_WEST, 0);
  Call3((void (*)())Actor_ArmCallback, 2, FACING_WEST, 0);
  Call2((void (*)())Func_02005704, 3, FACING_WEST);
  Call2((void (*)())Func_020056f4, 0x8005, 0);
  Actor_SetModeById(7, 4);
  Func_020056f4(0x2007, 0);
  Func_02005714_(5, 0x109, 0x14);
  Call3((void (*)())Func_020056ec, 0x8005, 0, 0x28);
  Func_020056f4(0x2006, 0);
  Func_02005704(5, FACING_EAST);
  Motion_SetModeAndWaitAnimation(5, 4);
  Call2((void (*)())Func_020056f4, 0x8005, 0);
  Call3((void (*)())Actor_SetSpeedParameters, 5, 0x19999, 0xcccc);
  Actor_SetModeById(5, 5);
  Call3((void (*)())Actor_SetPositionAndCommit, 5, 460, 288);
  Actor_SetModeById(5, 0x16);
  Func_020056f4(5, 0);
  Func_02005714_(0x13, 0x109, 0);
  Func_020057dc(0x4e);
  Func_02005724(0x3333, 0x666);
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(512), -1, PIXELS(320),
        1);
  Func_02005734();
  Call2((void (*)())Func_020056f4, 0x2013, 0);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_SOUTH, 0);
  Call3((void (*)())Actor_ArmCallback, gMainRoutineWork.currentOwner,
        FACING_SOUTH, 0);
  Call3((void (*)())Actor_ArmCallback, 6, FACING_SOUTHEAST, 0);
  Call3((void (*)())Actor_ArmCallback, 0, FACING_SOUTHWEST, 0);
  Call3((void (*)())Actor_ArmCallback, 1, FACING_SOUTHWEST, 0);
  Call3((void (*)())Actor_ArmCallback, 2, FACING_SOUTHWEST, 0);
  Call2((void (*)())Func_02005704, 3, FACING_SOUTHWEST);
  Motion_SetModeAndWaitAnimation(19, 4);
  Call3((void (*)())Func_020056ec, 0x2013, 0, 0x14);
  Actor_SetModeById(5, 1);
  FieldEvent_Wait(20);
  Call2((void (*)())Func_02005704, 5, FACING_SOUTHEAST);
  Func_020056f4(5, 0);
  Call3((void (*)())Func_020056ec, 0x2013, 0, 0x14);
  Func_020056f4(5, 0);
  Motion_SetModeAndWaitAnimation(19, 3);
  Func_020056f4(0x2013, 0);
  Actor_ArmCallback(5, FACING_EAST, 0);
  Call3((void (*)())Actor_ArmCallback, gMainRoutineWork.currentOwner,
        FACING_SOUTHWEST, 0);
  Actor_ArmCallback(0, FACING_EAST, 40);
  Call3((void (*)())Actor_SetSpeedParameters, 1, 0xcccc, 0x6666);
  Actor_GetById(1)->actionFlags &= 0xfe;
  Call3((void (*)())Actor_SetPositionAndReset, 1, 542, 328);
  FieldEvent_Wait(1);
  Actor_GetById(1)->actionFlags |= 1;
  Func_02005724(0xcccc, 0x1999);
  Func_0200573c()->motionFlags = 0;
  Camera_MoveWithinMapBounds(PIXELS(540), PIXELS(76), PIXELS(340), 1);
  Func_02005734();
  Call2((void (*)())Func_02005704, 1, FACING_SOUTH);
  Call3((void (*)())Actor_SetPositionAndReset, 1, 542, 340);
  Actor_Jump(1, 2, 0xa);
  Actor_Jump(1, 4, 0xa);
  Call2((void (*)())Func_020056f4, 0x5001, 0);
  Call3((void (*)())Actor_ArmCallback, 1, FACING_SOUTHWEST, 40);
  Call3((void (*)())Actor_ArmCallback, 1, FACING_SOUTHEAST, 20);
  Actor_Jump(1, 4, 0xa);
  Func_020056f4(0x5001, 0);
  Func_02005724(0x4cccc, 0x9999);
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(478), PIXELS(96),
        PIXELS(312), 1);
  Func_02005734();
  Call3((void (*)())Actor_ArmCallback, 6, FACING_SOUTH, 40);
  Call2((void (*)())Func_02005704, 6, FACING_WEST);
  Func_02005714_(6, 0x103, 0x14);
  Call2((void (*)())Func_02005704, 6, FACING_SOUTHWEST);
  Actor_Jump(6, 4, 0xa);
  Func_020056f4(0x6006, 0);
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(480), -1, PIXELS(276),
        1);
  Func_02005734();
  Call3((void (*)())Actor_ArmCallback, 7, FACING_NORTHEAST, 20);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_NORTHWEST, 20);
  Call2((void (*)())Func_02005704, 7, FACING_NORTH);
  Call2((void (*)())Func_020056f4, 0x8007, 0);
  Motion_SetModeAndWaitAnimation(7, 4);
  Func_020056f4(0x8007, 0);
  Func_020056f4(0x5000, 0);
  Camera_MoveWithinMapBounds(PIXELS(512), -1, PIXELS(312), 1);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_SOUTHEAST, 0);
  Call3((void (*)())Actor_ArmCallback, gMainRoutineWork.currentOwner,
        FACING_SOUTHEAST, 0);
  Call3((void (*)())Actor_ArmCallback, 3, FACING_SOUTHWEST, 0);
  Call3((void (*)())Actor_ArmCallback, 19, FACING_NORTH + FACING_STEP, 0);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_SOUTHEAST, 0);
  Call3((void (*)())Actor_ArmCallback, 1, FACING_NORTH, 40);
  Call2((void (*)())Func_02005704, 0, FACING_NORTH);
  Call3((void (*)())Func_020056ec, 0x8000, 0, 0x14);
  {
    s32 third = 0x28;
    s32 second = 0x105;
    s32 actor;

    actor = gMainRoutineWork.currentOwner;
    Func_02005714(actor, second, third);
  }
  Func_02005704(0, FACING_EAST);
  Func_02005714_(0, 0x109, 0x14);
  Func_020056f4(0x9000, 0);
  Call2((void (*)())Func_02005704, 0, FACING_NORTHWEST);
  {
    /* FAKEMATCH: pinned, the flags load before the actor (one reordered row
     * without). */
    register s32 flags asm("r1") = 0;

    FieldMessage_ShowAtActor(0x8000, flags);
  }
  if (Value2((s32 (*)())Event_AskYesNoWithActorAnimation,
             gMainRoutineWork.currentOwner, 0) == 0) {
    Motion_SetModeAndWaitAnimation(5, 3);
    Func_020056f4(0x8005, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  } else {
    Motion_SetModeAndWaitAnimation(5, 3);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    Func_020056f4(5, 0);
  }
  Call3((void (*)())Actor_SetSpeedParameters, 1, 0xcccc, 0x6666);
  Call3((void (*)())Actor_SetPositionAndReset, 1, 548, 320);
  Call2((void (*)())Func_02005704, 1, FACING_NORTHWEST);
  Actor_SetModeById(1, 3);
  Func_020056f4(1, 0);
  Call3((void (*)())Actor_ArmCallback, 6, FACING_NORTHEAST, 0);
  Func_02005714_(6, 0x109, 0x28);
  Func_020056f4(6, 0);
  Call3((void (*)())Actor_ArmCallback, 2, FACING_WEST, 0);
  Func_02005714_(2, 0x105, 0x28);
  Func_020056f4(0x8002, 0);
  Actor_SetModeById(7, 3);
  Func_020056f4(7, 0);
  Actor_SetModeById(3, 4);
  Func_020056f4(3, 0);
  Call3((void (*)())Actor_SetSpeedParameters, 19, 0xcccc, 0x6666);
  Call3((void (*)())Actor_SetPositionAndReset, 19, 498, 320);
  Actor_SetModeById(19, 3);
  Func_020056f4(0x2013, 0);
  Func_02005704(gMainRoutineWork.currentOwner, FACING_SOUTH);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  GameFlag_SetBit(0xa3d);
  Func_020057dc(0x1c);
  Call3((void (*)())Actor_SetSpeedParameters, 0, 0xcccc, 0x6666);
  Call3((void (*)())Actor_SetPositionAndReset, 0, 540, 286);
  Func_02005704(0, FACING_SOUTHEAST);
  Func_0200298c(7);
  Func_0200298c(5);
  Func_0200298c(6);
  Func_0200298c(0x13);
  Func_0200298c(0);
  Func_0200298c(3);
  Func_0200298c(2);
  Func_0200298c(1);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  GameFlag_SetBit(0x202);
  Call6((void (*)())Func_020055ac, 0x27, 0x28, 0x12, 0x10, 0x17, 0xa);
  Func_020055fc(1);
  Party_RemoveOwnerRestored(7);
  Party_RemoveOwnerRestored(5);
  Party_RemoveOwnerRestored(6);
  Party_RemoveOwnerRestored(0);
  Party_RemoveOwnerRestored(3);
  Party_RemoveOwnerRestored(2);
  Party_RemoveOwnerRestored(1);
  Actor_GetById(20)->collisionFlags |= 8;
  Actor_GetById(21)->collisionFlags |= 8;
  Actor_SetPartAttribute(Actor_GetById(28), 0xf);
  Func_0200567c(28, PIXELS(568), PIXELS(274));
  Actor_GetById(17)->motionFlags = 0;
  Call4((void (*)())Actor_SetPositionAndResetMotion, (s32)Actor_GetById(17),
        PIXELS(492), PIXELS(96), PIXELS(284));
  Actor_GetById(29)->motionFlags = 0;
  Call4((void (*)())Actor_SetPositionAndResetMotion, (s32)Actor_GetById(29),
        PIXELS(510), PIXELS(96), PIXELS(295));
  Actor_GetById(30)->motionFlags = 0;
  Actor_SetPositionAndResetMotion(Actor_GetById(30), PIXELS(540),
                                  PIXELS(96), PIXELS(300));
  wait(1);
  GameFlag_SetBit(0x17c);
  GameFlag_SetBit(0x144);
  Func_02005614();
}
