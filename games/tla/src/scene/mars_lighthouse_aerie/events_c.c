#include "mars_lighthouse_aerie.h"
#include "event_work.h"
#include "scene_tables.h"

extern const u8 MsgWhatSTheMatter[];
void Event_StartScreenTransition();
void Event_WaitTransitionFrames();
void Func_020053e4();
void Func_020054e4();
void Func_020055fc();
void Func_02005734();
void Party_RemoveOwnerRestored();
extern u8 gData_02006b70[];
extern u8 gData_02006c20[];


static __inline__ void Func_02005714_(s32 actor, s32 a, s32 b) {
  /* FAKEMATCH: pinned, the second and third arguments load before the actor, as
   * the native does at every call here (one reordered row per call without). */
  register s32 second asm("r1") = a;
  s32 third = b;

  Func_080d47b4(actor, second, third);
}

void Func_0200298c(s32 id) {
  struct Actor *object = Actor_Get(id);

  object->collisionFlags = 0;
  object->motionFlags = 0;
  FieldActor_SetSpritePriority(id, 1);
}

/// The scene that opens with MsgWhatSTheMatter; either answer to its prompt
/// steps the event's message number.
void Func_020029ac(void) {
  Func_0200560c();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  System_Wait(1);
  Actor_SetPosition(gMainRoutineWork.currentOwner, PIXELS(506), PIXELS(280));
  System_Wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldActor_SetSpritePriority(3, 1);
  Func_020053e4((s32)Actor_Get(7), (s32)gData_02006c20);
  Func_020053e4((s32)Actor_Get(3), (s32)gData_02006b70);
  FieldEvent_Wait(120);
  Func_020054e4((s32)Actor_Get(7));
  Func_020054e4((s32)Actor_Get(3));
  FieldEvent_Wait(40);
  Actor_SetAnimationAndWait(7, 4);
  Actor_FaceDirectionAndWait(6, FACING_NORTH);
  FieldEvent_SetMessageId(MsgWhatSTheMatter);
  Event_ShowMessage(6, 0);
  Actor_FaceDirectionAndWait(2, FACING_NORTHWEST);
  Event_ShowMessage(0x8002, 0);
  Actor_FaceDirection(3, FACING_SOUTHEAST, 20);
  Func_02005714_(3, 0x109, 0x14);
  Event_ShowMessage(0x1003, 0);
  Actor_FaceDirectionAndWait(0, FACING_NORTHEAST);
  Actor_SetAnimationAndWait(7, 4);
  Event_ShowMessage(0x2007, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(5, FACING_NORTHEAST);
  Func_02005714_(5, 0x107, 0);
  FieldActor_Jump(5, 2, 0xa);
  FieldActor_Jump(5, 4, 0xa);
  Event_ShowMessage(0x8005, 0);
  Actor_FaceDirection(7, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(6, FACING_NORTHWEST, 0);
  Actor_FaceDirection(0, FACING_WEST, 0);
  Actor_FaceDirection(1, FACING_WEST, 0);
  Actor_FaceDirection(2, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(3, FACING_WEST);
  Event_ShowMessage(0x8005, 0);
  Actor_SetAnimation(7, 4);
  Event_ShowMessage(0x2007, 0);
  Func_02005714_(5, 0x109, 0x14);
  FieldEvent_ShowMessageThenWait(0x8005, 0, 0x28);
  Event_ShowMessage(0x2006, 0);
  Actor_FaceDirectionAndWait(5, FACING_EAST);
  Actor_SetAnimationAndWait(5, 4);
  Event_ShowMessage(0x8005, 0);
  Actor_SetSpeed(5, 0x19999, 0xcccc);
  Actor_SetAnimation(5, 5);
  Actor_MoveToAndWait(5, 460, 288);
  Actor_SetAnimation(5, 0x16);
  Event_ShowMessage(5, 0);
  Func_02005714_(0x13, 0x109, 0);
  Event_PlaySound(0x4e);
  FieldCamera_SetSpeed(0x3333, 0x666);
  FieldCamera_MoveTo(PIXELS(512), -1, PIXELS(320), 1);
  Func_02005734();
  Event_ShowMessage(0x2013, 0);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_FaceDirection(6, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(0, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(1, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(2, FACING_SOUTHWEST, 0);
  Actor_FaceDirectionAndWait(3, FACING_SOUTHWEST);
  Actor_SetAnimationAndWait(19, 4);
  FieldEvent_ShowMessageThenWait(0x2013, 0, 0x14);
  Actor_SetAnimation(5, 1);
  FieldEvent_Wait(20);
  Actor_FaceDirectionAndWait(5, FACING_SOUTHEAST);
  Event_ShowMessage(5, 0);
  FieldEvent_ShowMessageThenWait(0x2013, 0, 0x14);
  Event_ShowMessage(5, 0);
  Actor_SetAnimationAndWait(19, 3);
  Event_ShowMessage(0x2013, 0);
  Actor_FaceDirection(5, FACING_EAST, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(0, FACING_EAST, 40);
  Actor_SetSpeed(1, 0xcccc, 0x6666);
  Actor_Get(1)->actionFlags &= 0xfe;
  Actor_WalkToAndWait(1, 542, 328);
  FieldEvent_Wait(1);
  Actor_Get(1)->actionFlags |= 1;
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(PIXELS(540), PIXELS(76), PIXELS(340), 1);
  Func_02005734();
  Actor_FaceDirectionAndWait(1, FACING_SOUTH);
  Actor_WalkToAndWait(1, 542, 340);
  FieldActor_Jump(1, 2, 0xa);
  FieldActor_Jump(1, 4, 0xa);
  Event_ShowMessage(0x5001, 0);
  Actor_FaceDirection(1, FACING_SOUTHWEST, 40);
  Actor_FaceDirection(1, FACING_SOUTHEAST, 20);
  FieldActor_Jump(1, 4, 0xa);
  Event_ShowMessage(0x5001, 0);
  FieldCamera_SetSpeed(0x4cccc, 0x9999);
  FieldCamera_MoveTo(PIXELS(478), PIXELS(96), PIXELS(312), 1);
  Func_02005734();
  Actor_FaceDirection(6, FACING_SOUTH, 40);
  Actor_FaceDirectionAndWait(6, FACING_WEST);
  Func_02005714_(6, 0x103, 0x14);
  Actor_FaceDirectionAndWait(6, FACING_SOUTHWEST);
  FieldActor_Jump(6, 4, 0xa);
  Event_ShowMessage(0x6006, 0);
  FieldCamera_MoveTo(PIXELS(480), -1, PIXELS(276), 1);
  Func_02005734();
  Actor_FaceDirection(7, FACING_NORTHEAST, 20);
  Actor_FaceDirection(7, FACING_NORTHWEST, 20);
  Actor_FaceDirectionAndWait(7, FACING_NORTH);
  Event_ShowMessage(0x8007, 0);
  Actor_SetAnimationAndWait(7, 4);
  Event_ShowMessage(0x8007, 0);
  Event_ShowMessage(0x5000, 0);
  FieldCamera_MoveTo(PIXELS(512), -1, PIXELS(312), 1);
  Actor_FaceDirection(7, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(3, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(19, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(7, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(1, FACING_NORTH, 40);
  Actor_FaceDirectionAndWait(0, FACING_NORTH);
  FieldEvent_ShowMessageThenWait(0x8000, 0, 0x14);
  {
    s32 third = 0x28;
    s32 second = 0x105;
    s32 actor;

    actor = gMainRoutineWork.currentOwner;
    FieldActor_ShowEmote(actor, second, third);
  }
  Actor_FaceDirectionAndWait(0, FACING_EAST);
  Func_02005714_(0, 0x109, 0x14);
  Event_ShowMessage(0x9000, 0);
  Actor_FaceDirectionAndWait(0, FACING_NORTHWEST);
  {
    s32 flags = 0;

    Event_OpenMessage(0x8000, flags);
  }
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Actor_SetAnimationAndWait(5, 3);
    Event_ShowMessage(0x8005, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  } else {
    Actor_SetAnimationAndWait(5, 3);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    Event_ShowMessage(5, 0);
  }
  Actor_SetSpeed(1, 0xcccc, 0x6666);
  Actor_WalkToAndWait(1, 548, 320);
  Actor_FaceDirectionAndWait(1, FACING_NORTHWEST);
  Actor_SetAnimation(1, 3);
  Event_ShowMessage(1, 0);
  Actor_FaceDirection(6, FACING_NORTHEAST, 0);
  Func_02005714_(6, 0x109, 0x28);
  Event_ShowMessage(6, 0);
  Actor_FaceDirection(2, FACING_WEST, 0);
  Func_02005714_(2, 0x105, 0x28);
  Event_ShowMessage(0x8002, 0);
  Actor_SetAnimation(7, 3);
  Event_ShowMessage(7, 0);
  Actor_SetAnimation(3, 4);
  Event_ShowMessage(3, 0);
  Actor_SetSpeed(19, 0xcccc, 0x6666);
  Actor_WalkToAndWait(19, 498, 320);
  Actor_SetAnimation(19, 3);
  Event_ShowMessage(0x2013, 0);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_SOUTH);
  Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
  Event_SetFlag(0xa3d);
  Event_PlaySound(0x1c);
  Actor_SetSpeed(0, 0xcccc, 0x6666);
  Actor_WalkToAndWait(0, 540, 286);
  Actor_FaceDirectionAndWait(0, FACING_SOUTHEAST);
  Func_0200298c(7);
  Func_0200298c(5);
  Func_0200298c(6);
  Func_0200298c(0x13);
  Func_0200298c(0);
  Func_0200298c(3);
  Func_0200298c(2);
  Func_0200298c(1);
  FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  Event_SetFlag(0x202);
  MapGrid_CopyCellAttributes(0x27, 0x28, 0x12, 0x10, 0x17, 0xa);
  Func_020055fc(1);
  Party_RemoveOwnerRestored(7);
  Party_RemoveOwnerRestored(5);
  Party_RemoveOwnerRestored(6);
  Party_RemoveOwnerRestored(0);
  Party_RemoveOwnerRestored(3);
  Party_RemoveOwnerRestored(2);
  Party_RemoveOwnerRestored(1);
  Actor_Get(20)->collisionFlags |= 8;
  Actor_Get(21)->collisionFlags |= 8;
  Actor_SetSpriteColor(Actor_Get(28), 0xf);
  Actor_SetPosition(28, PIXELS(568), PIXELS(274));
  Actor_Get(17)->motionFlags = 0;
  FieldActor_SetPositionAndResetMotion(Actor_Get(17), PIXELS(492), PIXELS(96), PIXELS(284));
  Actor_Get(29)->motionFlags = 0;
  FieldActor_SetPositionAndResetMotion(Actor_Get(29), PIXELS(510), PIXELS(96), PIXELS(295));
  Actor_Get(30)->motionFlags = 0;
  FieldActor_SetPositionAndResetMotion(Actor_Get(30), PIXELS(540),
                                  PIXELS(96), PIXELS(300));
  System_Wait(1);
  Event_SetFlag(0x17c);
  Event_SetFlag(0x144);
  Func_02005614();
}
