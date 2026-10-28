#include "local.h"
#include "scene_tables.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_02004464();
s32 Func_020045ec();
s32 Camera_MoveWithinMapBounds();
s32 Actor_Stop();
s32 wait();
s32 Func_020044cc();
s32 Func_020044d4();
s32 Func_0200456c();
void Func_0200457c(void);
s32 FieldEvent_Wait();
s32 Func_0200451c();
s32 Func_02004534();
s32 Actor_SetModeById();
s32 Motion_SetModeAndWaitAnimation();
s32 Actor_ArmCallback();
s32 Func_0200454c();
s32 Func_02004564();
s32 Func_0200452c();
s32 Event_AskYesNoWithActorAnimation();
s32 Actor_SetSpeedParameters();
s32 Actor_SetPositionAndReset();
s32 Func_02004584();
s32 Actor_Jump();
s32 Actor_WatchAndSetScript();
s32 Actor_GetById();
s32 Actor_ResetAndSetPosition();
s32 Actor_WaitForMoveById();
s32 GameFlag_ClearBit();
s32 GameFlag_SetBit();
s32 Func_0200446c();
extern u8 MsgProx80[];

void Func_02001cf0(void) {
  s32 t83;
  s32 t144;
  s32 t159;
  s32 t202;
  s32 t207;
  s32 angleC000;
  s32 angleB000;
  s32 angle3000;

  Func_02004464();
  Call1(Func_020045ec, 0);
  Call4(Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Call1(Actor_Stop, 0x14);
  Call1(wait, 1);
  Call3(Func_020044cc, gMainRoutineWork.currentOwner, PIXELS(184), PIXELS(484));
  /* FAKEMATCH: the three facings are locals set before their first placement;
   * as constants each placement rebuilds them and the later turns cannot share
   * them (differ 76). */
  angleC000 = 0xc000;
  angleB000 = 0xb000;
  Call4(Func_020044d4, 0, PIXELS(168), PIXELS(484), angleC000);
  Call4(Func_020044d4, 27, PIXELS(200), PIXELS(484), angleB000);
  angle3000 = 0x3000;
  Call4(Func_020044d4, 17, PIXELS(138), PIXELS(412), angle3000);
  Call4(Func_020044d4, 20, PIXELS(220), PIXELS(600), angleB000);
  Call3(Func_020044cc, 5, PIXELS(184), PIXELS(684));
  Call1(wait, 1);
  EVENT_WORK->screenEffect = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Call2(Func_0200456c, 0xcccc, 0x1999);
  Call4(Camera_MoveWithinMapBounds, PIXELS(184), -1, PIXELS(448), 1);
  Func_0200457c();
  Call1(FieldEvent_Wait, 20);
  Call1(Func_0200451c, (s32)MsgProx80);
  Call2(Func_02004534, 26, 0);
  Call2(Actor_SetModeById, 0, 3);
  Call2(Actor_SetModeById, 27, 3);
  Call2(Motion_SetModeAndWaitAnimation, gMainRoutineWork.currentOwner, 3);
  Call3(Actor_ArmCallback, 26, 0x5000, 0);
  Prox_Jump(26, 0x108, 20);
  Call2(Func_02004534, 26, 0);
  Call3(Actor_ArmCallback, gMainRoutineWork.currentOwner, FACING_WEST, 0);
  Call3(Actor_ArmCallback, 0, 0, 20);
  Prox_Jump(gMainRoutineWork.currentOwner, 0x108, 0);
  Prox_Jump(0, 0x108, 40);
  Call3(Actor_ArmCallback, 0, FACING_NORTH, 0);
  Call2(Func_0200454c, gMainRoutineWork.currentOwner, FACING_NORTH);
  Call2(Func_02004534, 0, 0);
  Call2(Func_02004564, 0x19, 0x102);
  Call1(FieldEvent_Wait, 20);
  Call2(Func_02004534, 25, 0);
  Call2(Actor_SetModeById, 27, 4);
  Call2(Func_02004534, 27, 0);
  Call2(Actor_SetModeById, 19, 4);
  Call2(Func_02004534, 0x4013, 0);
  Prox_Jump(0, 0x109, 20);
  Call2(Func_02004534, 0, 0);
  Call3(Actor_ArmCallback, 27, FACING_WEST, 20);
  Prox_Jump(27, 0x108, 0);
  Call2(Func_02004534, 27, 0);
  Call2(Func_02004534, 17, 0);
  Call3(Actor_ArmCallback, 27, 0xb000, 0);
  Prox_Jump(27, 0x109, 20);
  Call2(Func_02004534, 27, 0);
  Call2(Func_0200454c, 26, 0x3000);
  Call2(Actor_SetModeById, 26, 4);
  Call2(Func_02004534, 26, 0);
  Call2(Func_02004534, 0, 0);
  Call2(Func_02004564, 0x13, 0x102);
  Call1(FieldEvent_Wait, 20);
  Call2(Func_02004534, 0x4013, 0);
  Call2(Func_0200454c, 0, FACING_NORTHEAST);
  Call2(Motion_SetModeAndWaitAnimation, 0, 3);
  Call2(Func_02004534, 0, 0);
  Call3(Actor_ArmCallback, 26, 0x5000, 0);
  Call2(Func_02004564, 0x1a, 0x102);
  Call1(FieldEvent_Wait, 20);
  Call2(Func_02004534, 26, 0);
  Prox_Jump(27, 0x109, 20);
  Call2(Func_02004534, 27, 0);
  Call3(Actor_ArmCallback, 25, 0x3000, 0);
  Prox_Jump(25, 0x103, 20);
  Call2(Func_02004534, 25, 0);
  Prox_Jump(27, 0x100, 20);
  Call2(Func_02004534, 27, 0);
  Call3(Actor_ArmCallback, 26, 0x3000, 0);
  Call2(Func_02004534, 26, 0);
  Call3(Actor_ArmCallback, 0, FACING_NORTH, 0);
  Call2(Func_02004534, 0, 0);
  Call3(Actor_ArmCallback, 25, 0x5000, 0);
  Prox_Jump(25, 0x105, 20);
  Call2(Func_02004534, 25, 0);
  Call2(Actor_SetModeById, 26, 4);
  Call3(Func_0200452c, 0x1a, 0, 0x14);
  Call2(Func_0200454c, 0, 0);
  Prox_Jump(0, 0x108, 0);
  Prox_MessageOpen(0x4000, 0);
  Call3(Actor_ArmCallback, gMainRoutineWork.currentOwner, FACING_WEST, 0);
  t83 = Value2(Event_AskYesNoWithActorAnimation, gMainRoutineWork.currentOwner, 0);
  if (t83 == 0) {
    Call2(Motion_SetModeAndWaitAnimation, 26, 3);
    Call2(Func_02004534, 26, 0);
    EVENT_WORK->messageId++;
  } else {
    EVENT_WORK->messageId++;
    Call2(Motion_SetModeAndWaitAnimation, 26, 4);
    Call2(Func_02004534, 26, 0);
  }
  Call2(Func_0200454c, 27, FACING_WEST);
  Call2(Actor_SetModeById, 27, 4);
  Call2(Func_02004534, 27, 0);
  Call3(Actor_ArmCallback, 25, 0x3000, 0);
  Prox_Jump(25, 0x108, 20);
  Call2(Func_02004534, 25, 0);
  Call3(Actor_ArmCallback, 27, 0xb000, 20);
  Call2(Motion_SetModeAndWaitAnimation, 27, 3);
  Call2(Func_0200454c, 27, FACING_WEST);
  Call2(Func_02004534, 0x401b, 0);
  Call3(Actor_ArmCallback, gMainRoutineWork.currentOwner, 0, 0);
  Call2(Func_0200454c, 0, 0);
  Call2(Actor_SetModeById, gMainRoutineWork.currentOwner, 3);
  Call2(Motion_SetModeAndWaitAnimation, 0, 3);
  Call2(Func_02004534, 0x4000, 0);
  Call2(Func_02004564, 0x1a, 0x102);
  Call1(FieldEvent_Wait, 20);
  Call2(Func_02004534, 26, 0);
  Call3(Actor_ArmCallback, gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Call3(Actor_ArmCallback, 0, FACING_NORTH, 0);
  Call3(Actor_ArmCallback, 27, 0xb000, 0);
  Call2(Func_0200454c, 25, FACING_WEST);
  Call2(Motion_SetModeAndWaitAnimation, 25, 3);
  Call2(Func_0200454c, 25, 0x5000);
  Call2(Func_02004534, 25, 0);
  Prox_Jump(27, 0x109, 20);
  Call2(Func_02004534, 27, 0);
  Call2(Actor_SetModeById, 27, 4);
  Call2(Func_02004534, 27, 0);
  Call3(Actor_ArmCallback, gMainRoutineWork.currentOwner, 0, 0);
  Call3(Actor_ArmCallback, 0, 0, 0);
  Call3(Actor_ArmCallback, 25, 0x3000, 0);
  Prox_Jump(gMainRoutineWork.currentOwner, 0x101, 0);
  Prox_Jump(0, 0x101, 0);
  Prox_Jump(26, 0x101, 0);
  Prox_Jump(25, 0x101, 40);
  Call2(Motion_SetModeAndWaitAnimation, 27, 3);
  Call2(Func_02004534, 27, 0);
  Call2(Actor_SetModeById, 0, 3);
  Call2(Actor_SetModeById, gMainRoutineWork.currentOwner, 3);
  Call2(Actor_SetModeById, 26, 3);
  Call2(Actor_SetModeById, 25, 3);
  Call3(Actor_ArmCallback, 27, FACING_WEST, 0);
  Prox_Jump(27, 0x108, 20);
  Call2(Func_02004534, 27, 0);
  Prox_Jump(26, 0x109, 20);
  Call3(Func_0200452c, 0x1a, 0, 0x14);
  Call2(Func_02004534, 5, 0);
  Call2(Func_02004564, 0x1b, 0x102);
  Call3(Actor_ArmCallback, gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Call3(Actor_ArmCallback, 0, FACING_SOUTH, 0);
  Call2(Func_0200454c, 27, 0x5000);
  Call2(Func_02004534, 0x401b, 0);
  Call2(Func_0200454c, 0, 0);
  Prox_MessageOpen(0x4000, 0);
  Call3(Actor_ArmCallback, gMainRoutineWork.currentOwner, FACING_WEST, 0);
  t144 = Value2(Event_AskYesNoWithActorAnimation, gMainRoutineWork.currentOwner, 0);
  if (t144 == 0) {
    Call2(Func_0200456c, 0x26666, 0x4ccc);
    Call4(Camera_MoveWithinMapBounds, PIXELS(184), -1, PIXELS(512), 1);
    Call2(Func_0200454c, 0, FACING_SOUTH);
    Call3(Actor_SetSpeedParameters, 0, 0x19999, 0xcccc);
    Call3(Actor_SetPositionAndReset, 0, 168, 520);
    Call2(Func_02004534, 0x4000, 0);
    Call2(Func_02004534, 0x2005, 0);
    EVENT_WORK->messageId += 2;
  } else {
    EVENT_WORK->messageId += 2;
    Call2(Func_0200456c, 0x26666, 0x4ccc);
    Call4(Camera_MoveWithinMapBounds, PIXELS(184), -1, PIXELS(512), 1);
    Call2(Func_0200454c, 0, FACING_SOUTH);
    Call3(Actor_SetSpeedParameters, 0, 0x19999, 0xcccc);
    Call3(Actor_SetPositionAndReset, 0, 168, 520);
    Call2(Func_02004534, 0x4000, 0);
    Call2(Func_02004534, 0x2005, 0);
  }
  t159 = Func_02004584();
  FIELD_AT((t159 + 0x55), u8, 0) = 0;
  Call4(Camera_MoveWithinMapBounds, PIXELS(158), -1, PIXELS(432), 1);
  Func_0200457c();
  Call3(Actor_Jump, 17, 4, 10);
  Call2(Func_02004534, 17, 0);
  Call2(Func_0200456c, 0x13333, 0x2666);
  Call4(Camera_MoveWithinMapBounds, PIXELS(172), -1, PIXELS(520), 1);
  Call2(Actor_WatchAndSetScript, 0, 0x11);
  Call2(Actor_WatchAndSetScript, gMainRoutineWork.currentOwner, 0x11);
  Call2(Actor_WatchAndSetScript, 0x1b, 0x11);
  Call3(Actor_SetSpeedParameters, 17, 0x19999, 0xcccc);
  Call3(Actor_SetPositionAndReset, 17, 140, 478);
  Call3(Actor_SetPositionAndReset, 17, 150, 540);
  Call3(Actor_SetPositionAndReset, 17, 160, 560);
  Call3(Actor_SetPositionAndReset, 17, 190, 640);
  Call1(Actor_Stop, 0);
  Call1(Actor_Stop, gMainRoutineWork.currentOwner);
  Call1(Actor_Stop, 0x1b);
  Call1(wait, 1);
  Call3(Func_020044cc, 17, 0, 0);
  Call1(FieldEvent_Wait, 10);
  Call2(Func_0200456c, 0x26666, 0x4ccc);
  Call4(Camera_MoveWithinMapBounds, PIXELS(184), -1, PIXELS(448), 1);
  Call3(Actor_SetPositionAndReset, 0, 168, 484);
  Call2(Func_0200454c, 0, 0);
  Call2(Func_02004534, 0x4000, 0);
  Call3(Actor_ArmCallback, gMainRoutineWork.currentOwner, FACING_WEST, 0);
  Call2(Func_0200454c, 27, FACING_WEST);
  Call2(Func_02004534, 0x401b, 0);
  Call3(Actor_ArmCallback, 25, 0x5000, 0);
  Call2(Motion_SetModeAndWaitAnimation, 25, 3);
  Call2(Func_02004534, 25, 0);
  Call3(Actor_ArmCallback, 0, FACING_NORTH, 0);
  Call3(Actor_ArmCallback, 27, 0xb000, 0);
  Call2(Func_0200454c, gMainRoutineWork.currentOwner, FACING_NORTH);
  Call2(Motion_SetModeAndWaitAnimation, 26, 3);
  Call2(Func_02004534, 26, 0);
  Call2(Actor_SetModeById, 0, 3);
  Call2(Actor_SetModeById, gMainRoutineWork.currentOwner, 3);
  Call2(Motion_SetModeAndWaitAnimation, 27, 3);
  Call3(Actor_SetSpeedParameters, 0, 0x19999, 0xcccc);
  Call3(Actor_SetSpeedParameters, 27, 0x19999, 0xcccc);
  Call2(Actor_SetModeById, 0, 2);
  t202 = Value1(Actor_GetById, gMainRoutineWork.currentOwner);
  if (t202 != 0) {
    Call3(Actor_ResetAndSetPosition, 0, FIELD_AT(t202, s16, 10),
          FIELD_AT(t202, s16, 18));
  }
  Call1(Actor_WaitForMoveById, 0);
  Call3(Func_020044cc, 0, 0, 0);
  Call2(Actor_SetModeById, 27, 2);
  t207 = Value1(Actor_GetById, gMainRoutineWork.currentOwner);
  if (t207 != 0) {
    Call3(Actor_ResetAndSetPosition, 27, FIELD_AT(t207, s16, 10),
          FIELD_AT(t207, s16, 18));
  }
  Call1(Actor_WaitForMoveById, 27);
  Call3(Func_020044cc, 27, 0, 0);
  {
    struct EventWork *work = EVENT_WORK;

    work->screenEffect = 0x209;
    work->delay = 0x10;
  }
  Call1(GameFlag_ClearBit, 0x12f);
  Call1(GameFlag_SetBit, 0x9c5);
  Func_0200446c();
}
