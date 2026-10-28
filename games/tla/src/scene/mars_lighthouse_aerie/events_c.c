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
  ((void (*)())Func_0200560c)();
  Call1((void (*)())Func_02005784, 0);
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Call1((void (*)())wait, 1);
  Call3((void (*)())Func_0200567c, gMainRoutineWork.currentOwner, PIXELS(506),
        PIXELS(280));
  Call1((void (*)())wait, 1);
  ((void (*)())Event_StartScreenTransition)();
  ((void (*)())Event_WaitTransitionFrames)();
  Call2((void (*)())Actor_SetSpritePriority, 3, 1);
  Call2((void (*)())Func_020053e4, (s32)Actor_GetById(7), (s32)gData_02006c20);
  Call2((void (*)())Func_020053e4, (s32)Actor_GetById(3), (s32)gData_02006b70);
  Call1((void (*)())FieldEvent_Wait, 120);
  Call1((void (*)())Func_020054e4, (s32)Actor_GetById(7));
  Call1((void (*)())Func_020054e4, (s32)Actor_GetById(3));
  Call1((void (*)())FieldEvent_Wait, 40);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 7, 4);
  Call2((void (*)())Func_02005704, 6, FACING_NORTH);
  Call1((void (*)())Func_020056dc, (s32)MsgWhatSTheMatter);
  Call2((void (*)())Func_020056f4, 6, 0);
  Call2((void (*)())Func_02005704, 2, FACING_NORTHWEST);
  Call2((void (*)())Func_020056f4, 0x8002, 0);
  Call3((void (*)())Actor_ArmCallback, 3, FACING_SOUTHEAST, 20);
  Func_02005714_(3, 0x109, 0x14);
  Call2((void (*)())Func_020056f4, 0x1003, 0);
  Call2((void (*)())Func_02005704, 0, FACING_NORTHEAST);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 7, 4);
  Call2((void (*)())Func_020056f4, 0x2007, 0);
  Call3((void (*)())Actor_ArmCallback, gMainRoutineWork.currentOwner,
        FACING_WEST, 0);
  Call2((void (*)())Func_02005704, 5, FACING_NORTHEAST);
  Func_02005714_(5, 0x107, 0);
  Call3((void (*)())Actor_Jump, 5, 2, 0xa);
  Call3((void (*)())Actor_Jump, 5, 4, 0xa);
  Call2((void (*)())Func_020056f4, 0x8005, 0);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_SOUTHWEST, 0);
  Call3((void (*)())Actor_ArmCallback, 6, FACING_NORTHWEST, 0);
  Call3((void (*)())Actor_ArmCallback, 0, FACING_WEST, 0);
  Call3((void (*)())Actor_ArmCallback, 1, FACING_WEST, 0);
  Call3((void (*)())Actor_ArmCallback, 2, FACING_WEST, 0);
  Call2((void (*)())Func_02005704, 3, FACING_WEST);
  Call2((void (*)())Func_020056f4, 0x8005, 0);
  Call2((void (*)())Actor_SetModeById, 7, 4);
  Call2((void (*)())Func_020056f4, 0x2007, 0);
  Func_02005714_(5, 0x109, 0x14);
  Call3((void (*)())Func_020056ec, 0x8005, 0, 0x28);
  Call2((void (*)())Func_020056f4, 0x2006, 0);
  Call2((void (*)())Func_02005704, 5, FACING_EAST);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 5, 4);
  Call2((void (*)())Func_020056f4, 0x8005, 0);
  Call3((void (*)())Actor_SetSpeedParameters, 5, 0x19999, 0xcccc);
  Call2((void (*)())Actor_SetModeById, 5, 5);
  Call3((void (*)())Actor_SetPositionAndCommit, 5, 460, 288);
  Call2((void (*)())Actor_SetModeById, 5, 0x16);
  Call2((void (*)())Func_020056f4, 5, 0);
  Func_02005714_(0x13, 0x109, 0);
  Call1((void (*)())Func_020057dc, 0x4e);
  Call2((void (*)())Func_02005724, 0x3333, 0x666);
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(512), -1, PIXELS(320),
        1);
  ((void (*)())Func_02005734)();
  Call2((void (*)())Func_020056f4, 0x2013, 0);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_SOUTH, 0);
  Call3((void (*)())Actor_ArmCallback, gMainRoutineWork.currentOwner,
        FACING_SOUTH, 0);
  Call3((void (*)())Actor_ArmCallback, 6, FACING_SOUTHEAST, 0);
  Call3((void (*)())Actor_ArmCallback, 0, FACING_SOUTHWEST, 0);
  Call3((void (*)())Actor_ArmCallback, 1, FACING_SOUTHWEST, 0);
  Call3((void (*)())Actor_ArmCallback, 2, FACING_SOUTHWEST, 0);
  Call2((void (*)())Func_02005704, 3, FACING_SOUTHWEST);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 19, 4);
  Call3((void (*)())Func_020056ec, 0x2013, 0, 0x14);
  Call2((void (*)())Actor_SetModeById, 5, 1);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Func_02005704, 5, FACING_SOUTHEAST);
  Call2((void (*)())Func_020056f4, 5, 0);
  Call3((void (*)())Func_020056ec, 0x2013, 0, 0x14);
  Call2((void (*)())Func_020056f4, 5, 0);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 19, 3);
  Call2((void (*)())Func_020056f4, 0x2013, 0);
  Call3((void (*)())Actor_ArmCallback, 5, FACING_EAST, 0);
  Call3((void (*)())Actor_ArmCallback, gMainRoutineWork.currentOwner,
        FACING_SOUTHWEST, 0);
  Call3((void (*)())Actor_ArmCallback, 0, FACING_EAST, 40);
  Call3((void (*)())Actor_SetSpeedParameters, 1, 0xcccc, 0x6666);
  Actor_GetById(1)->actionFlags &= 0xfe;
  Call3((void (*)())Actor_SetPositionAndReset, 1, 542, 328);
  Call1((void (*)())FieldEvent_Wait, 1);
  Actor_GetById(1)->actionFlags |= 1;
  Call2((void (*)())Func_02005724, 0xcccc, 0x1999);
  Func_0200573c()->motionFlags = 0;
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(540), PIXELS(76),
        PIXELS(340), 1);
  ((void (*)())Func_02005734)();
  Call2((void (*)())Func_02005704, 1, FACING_SOUTH);
  Call3((void (*)())Actor_SetPositionAndReset, 1, 542, 340);
  Call3((void (*)())Actor_Jump, 1, 2, 0xa);
  Call3((void (*)())Actor_Jump, 1, 4, 0xa);
  Call2((void (*)())Func_020056f4, 0x5001, 0);
  Call3((void (*)())Actor_ArmCallback, 1, FACING_SOUTHWEST, 40);
  Call3((void (*)())Actor_ArmCallback, 1, FACING_SOUTHEAST, 20);
  Call3((void (*)())Actor_Jump, 1, 4, 0xa);
  Call2((void (*)())Func_020056f4, 0x5001, 0);
  Call2((void (*)())Func_02005724, 0x4cccc, 0x9999);
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(478), PIXELS(96),
        PIXELS(312), 1);
  ((void (*)())Func_02005734)();
  Call3((void (*)())Actor_ArmCallback, 6, FACING_SOUTH, 40);
  Call2((void (*)())Func_02005704, 6, FACING_WEST);
  Func_02005714_(6, 0x103, 0x14);
  Call2((void (*)())Func_02005704, 6, FACING_SOUTHWEST);
  Call3((void (*)())Actor_Jump, 6, 4, 0xa);
  Call2((void (*)())Func_020056f4, 0x6006, 0);
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(480), -1, PIXELS(276),
        1);
  ((void (*)())Func_02005734)();
  Call3((void (*)())Actor_ArmCallback, 7, FACING_NORTHEAST, 20);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_NORTHWEST, 20);
  Call2((void (*)())Func_02005704, 7, FACING_NORTH);
  Call2((void (*)())Func_020056f4, 0x8007, 0);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 7, 4);
  Call2((void (*)())Func_020056f4, 0x8007, 0);
  Call2((void (*)())Func_020056f4, 0x5000, 0);
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(512), -1, PIXELS(312),
        1);
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
    /* FAKEMATCH: here the leader loads after both constants, pinned with the
     * actor (one reordered row through the forwarder). */
    register s32 third asm("r2") = 0x28;
    /* FAKEMATCH: the second argument, pinned with the third. */
    register s32 second asm("r1") = 0x105;
    /* FAKEMATCH: the leader, pinned after both. */
    register s32 actor asm("r0");

    actor = gMainRoutineWork.currentOwner;
    Func_02005714(actor, second, third);
  }
  Call2((void (*)())Func_02005704, 0, FACING_EAST);
  Func_02005714_(0, 0x109, 0x14);
  Call2((void (*)())Func_020056f4, 0x9000, 0);
  Call2((void (*)())Func_02005704, 0, FACING_NORTHWEST);
  {
    /* FAKEMATCH: pinned, the flags load before the actor (one reordered row
     * without). */
    register s32 flags asm("r1") = 0;

    FieldMessage_ShowAtActor(0x8000, flags);
  }
  if (Value2((s32 (*)())Event_AskYesNoWithActorAnimation,
             gMainRoutineWork.currentOwner, 0) == 0) {
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 5, 3);
    Call2((void (*)())Func_020056f4, 0x8005, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  } else {
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 5, 3);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    Call2((void (*)())Func_020056f4, 5, 0);
  }
  Call3((void (*)())Actor_SetSpeedParameters, 1, 0xcccc, 0x6666);
  Call3((void (*)())Actor_SetPositionAndReset, 1, 548, 320);
  Call2((void (*)())Func_02005704, 1, FACING_NORTHWEST);
  Call2((void (*)())Actor_SetModeById, 1, 3);
  Call2((void (*)())Func_020056f4, 1, 0);
  Call3((void (*)())Actor_ArmCallback, 6, FACING_NORTHEAST, 0);
  Func_02005714_(6, 0x109, 0x28);
  Call2((void (*)())Func_020056f4, 6, 0);
  Call3((void (*)())Actor_ArmCallback, 2, FACING_WEST, 0);
  Func_02005714_(2, 0x105, 0x28);
  Call2((void (*)())Func_020056f4, 0x8002, 0);
  Call2((void (*)())Actor_SetModeById, 7, 3);
  Call2((void (*)())Func_020056f4, 7, 0);
  Call2((void (*)())Actor_SetModeById, 3, 4);
  Call2((void (*)())Func_020056f4, 3, 0);
  Call3((void (*)())Actor_SetSpeedParameters, 19, 0xcccc, 0x6666);
  Call3((void (*)())Actor_SetPositionAndReset, 19, 498, 320);
  Call2((void (*)())Actor_SetModeById, 19, 3);
  Call2((void (*)())Func_020056f4, 0x2013, 0);
  Call2((void (*)())Func_02005704, gMainRoutineWork.currentOwner, FACING_SOUTH);
  Call2((void (*)())Motion_SetModeAndWaitAnimation,
        gMainRoutineWork.currentOwner, 3);
  Call1((void (*)())GameFlag_SetBit, 0xa3d);
  Call1((void (*)())Func_020057dc, 0x1c);
  Call3((void (*)())Actor_SetSpeedParameters, 0, 0xcccc, 0x6666);
  Call3((void (*)())Actor_SetPositionAndReset, 0, 540, 286);
  Call2((void (*)())Func_02005704, 0, FACING_SOUTHEAST);
  Call1((void (*)())Func_0200298c, 7);
  Call1((void (*)())Func_0200298c, 5);
  Call1((void (*)())Func_0200298c, 6);
  Call1((void (*)())Func_0200298c, 0x13);
  Call1((void (*)())Func_0200298c, 0);
  Call1((void (*)())Func_0200298c, 3);
  Call1((void (*)())Func_0200298c, 2);
  Call1((void (*)())Func_0200298c, 1);
  Call2((void (*)())Actor_SetSpritePriority, gMainRoutineWork.currentOwner, 1);
  Call1((void (*)())GameFlag_SetBit, 0x202);
  Call6((void (*)())Func_020055ac, 0x27, 0x28, 0x12, 0x10, 0x17, 0xa);
  Call1((void (*)())Func_020055fc, 1);
  Call1((void (*)())Party_RemoveOwnerRestored, 7);
  Call1((void (*)())Party_RemoveOwnerRestored, 5);
  Call1((void (*)())Party_RemoveOwnerRestored, 6);
  Call1((void (*)())Party_RemoveOwnerRestored, 0);
  Call1((void (*)())Party_RemoveOwnerRestored, 3);
  Call1((void (*)())Party_RemoveOwnerRestored, 2);
  Call1((void (*)())Party_RemoveOwnerRestored, 1);
  Actor_GetById(20)->collisionFlags |= 8;
  Actor_GetById(21)->collisionFlags |= 8;
  Call2((void (*)())Actor_SetPartAttribute, (s32)Actor_GetById(28), 0xf);
  Call3((void (*)())Func_0200567c, 28, PIXELS(568), PIXELS(274));
  Actor_GetById(17)->motionFlags = 0;
  Call4((void (*)())Actor_SetPositionAndResetMotion, (s32)Actor_GetById(17),
        PIXELS(492), PIXELS(96), PIXELS(284));
  Actor_GetById(29)->motionFlags = 0;
  Call4((void (*)())Actor_SetPositionAndResetMotion, (s32)Actor_GetById(29),
        PIXELS(510), PIXELS(96), PIXELS(295));
  Actor_GetById(30)->motionFlags = 0;
  Call4((void (*)())Actor_SetPositionAndResetMotion, (s32)Actor_GetById(30),
        PIXELS(540), PIXELS(96), PIXELS(300));
  Call1((void (*)())wait, 1);
  Call1((void (*)())GameFlag_SetBit, 0x17c);
  Call1((void (*)())GameFlag_SetBit, 0x144);
  ((void (*)())Func_02005614)();
}
