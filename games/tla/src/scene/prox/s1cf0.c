#include "local.h"

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
s32 WaitFrames();
s32 Func_020044cc();
s32 Func_020044d4();
s32 Func_0200456c();
void Func_0200457c(void);
s32 FieldEvent_WaitFrames();
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
  Call1(WaitFrames, 1);
  Call3(Func_020044cc, gAdventure.currentOwner, 0xb80000, 0x1e40000);
  /* FAKEMATCH: the three facings are locals set before their first placement;
   * as constants each placement rebuilds them and the later turns cannot share
   * them (differ 76). */
  angleC000 = 0xc000;
  angleB000 = 0xb000;
  Call4(Func_020044d4, 0, 0xa80000, 0x1e40000, angleC000);
  Call4(Func_020044d4, 0x1b, 0xc80000, 0x1e40000, angleB000);
  angle3000 = 0x3000;
  Call4(Func_020044d4, 0x11, 0x8a0000, 0x19c0000, angle3000);
  Call4(Func_020044d4, 0x14, 0xdc0000, 0x2580000, angleB000);
  Call3(Func_020044cc, 5, 0xb80000, 0x2ac0000);
  Call1(WaitFrames, 1);
  EVENT_WORK->screenEffect = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Call2(Func_0200456c, 0xcccc, 0x1999);
  Call4(Camera_MoveWithinMapBounds, 0xb80000, -1, 0x1c00000, 1);
  Func_0200457c();
  Call1(FieldEvent_WaitFrames, 0x14);
  Call1(Func_0200451c, (s32)MsgProx80);
  Call2(Func_02004534, 0x1a, 0);
  Call2(Actor_SetModeById, 0, 3);
  Call2(Actor_SetModeById, 0x1b, 3);
  Call2(Motion_SetModeAndWaitAnimation, gAdventure.currentOwner, 3);
  Call3(Actor_ArmCallback, 0x1a, 0x5000, 0);
  Prox_Jump(0x1a, 0x108, 0x14);
  Call2(Func_02004534, 0x1a, 0);
  Call3(Actor_ArmCallback, gAdventure.currentOwner, 0x8000, 0);
  Call3(Actor_ArmCallback, 0, 0, 0x14);
  Prox_Jump(gAdventure.currentOwner, 0x108, 0);
  Prox_Jump(0, 0x108, 0x28);
  Call3(Actor_ArmCallback, 0, 0xc000, 0);
  Call2(Func_0200454c, gAdventure.currentOwner, 0xc000);
  Call2(Func_02004534, 0, 0);
  Call2(Func_02004564, 0x19, 0x102);
  Call1(FieldEvent_WaitFrames, 0x14);
  Call2(Func_02004534, 0x19, 0);
  Call2(Actor_SetModeById, 0x1b, 4);
  Call2(Func_02004534, 0x1b, 0);
  Call2(Actor_SetModeById, 0x13, 4);
  Call2(Func_02004534, 0x4013, 0);
  Prox_Jump(0, 0x109, 0x14);
  Call2(Func_02004534, 0, 0);
  Call3(Actor_ArmCallback, 0x1b, 0x8000, 0x14);
  Prox_Jump(0x1b, 0x108, 0);
  Call2(Func_02004534, 0x1b, 0);
  Call2(Func_02004534, 0x11, 0);
  Call3(Actor_ArmCallback, 0x1b, 0xb000, 0);
  Prox_Jump(0x1b, 0x109, 0x14);
  Call2(Func_02004534, 0x1b, 0);
  Call2(Func_0200454c, 0x1a, 0x3000);
  Call2(Actor_SetModeById, 0x1a, 4);
  Call2(Func_02004534, 0x1a, 0);
  Call2(Func_02004534, 0, 0);
  Call2(Func_02004564, 0x13, 0x102);
  Call1(FieldEvent_WaitFrames, 0x14);
  Call2(Func_02004534, 0x4013, 0);
  Call2(Func_0200454c, 0, 0xe000);
  Call2(Motion_SetModeAndWaitAnimation, 0, 3);
  Call2(Func_02004534, 0, 0);
  Call3(Actor_ArmCallback, 0x1a, 0x5000, 0);
  Call2(Func_02004564, 0x1a, 0x102);
  Call1(FieldEvent_WaitFrames, 0x14);
  Call2(Func_02004534, 0x1a, 0);
  Prox_Jump(0x1b, 0x109, 0x14);
  Call2(Func_02004534, 0x1b, 0);
  Call3(Actor_ArmCallback, 0x19, 0x3000, 0);
  Prox_Jump(0x19, 0x103, 0x14);
  Call2(Func_02004534, 0x19, 0);
  Prox_Jump(0x1b, 0x100, 0x14);
  Call2(Func_02004534, 0x1b, 0);
  Call3(Actor_ArmCallback, 0x1a, 0x3000, 0);
  Call2(Func_02004534, 0x1a, 0);
  Call3(Actor_ArmCallback, 0, 0xc000, 0);
  Call2(Func_02004534, 0, 0);
  Call3(Actor_ArmCallback, 0x19, 0x5000, 0);
  Prox_Jump(0x19, 0x105, 0x14);
  Call2(Func_02004534, 0x19, 0);
  Call2(Actor_SetModeById, 0x1a, 4);
  Call3(Func_0200452c, 0x1a, 0, 0x14);
  Call2(Func_0200454c, 0, 0);
  Prox_Jump(0, 0x108, 0);
  Prox_MessageOpen(0x4000, 0);
  Call3(Actor_ArmCallback, gAdventure.currentOwner, 0x8000, 0);
  t83 = Value2(Event_AskYesNoWithActorAnimation, gAdventure.currentOwner, 0);
  if (t83 == 0) {
    Call2(Motion_SetModeAndWaitAnimation, 0x1a, 3);
    Call2(Func_02004534, 0x1a, 0);
    EVENT_WORK->messageId++;
  } else {
    EVENT_WORK->messageId++;
    Call2(Motion_SetModeAndWaitAnimation, 0x1a, 4);
    Call2(Func_02004534, 0x1a, 0);
  }
  Call2(Func_0200454c, 0x1b, 0x8000);
  Call2(Actor_SetModeById, 0x1b, 4);
  Call2(Func_02004534, 0x1b, 0);
  Call3(Actor_ArmCallback, 0x19, 0x3000, 0);
  Prox_Jump(0x19, 0x108, 0x14);
  Call2(Func_02004534, 0x19, 0);
  Call3(Actor_ArmCallback, 0x1b, 0xb000, 0x14);
  Call2(Motion_SetModeAndWaitAnimation, 0x1b, 3);
  Call2(Func_0200454c, 0x1b, 0x8000);
  Call2(Func_02004534, 0x401b, 0);
  Call3(Actor_ArmCallback, gAdventure.currentOwner, 0, 0);
  Call2(Func_0200454c, 0, 0);
  Call2(Actor_SetModeById, gAdventure.currentOwner, 3);
  Call2(Motion_SetModeAndWaitAnimation, 0, 3);
  Call2(Func_02004534, 0x4000, 0);
  Call2(Func_02004564, 0x1a, 0x102);
  Call1(FieldEvent_WaitFrames, 0x14);
  Call2(Func_02004534, 0x1a, 0);
  Call3(Actor_ArmCallback, gAdventure.currentOwner, 0xc000, 0);
  Call3(Actor_ArmCallback, 0, 0xc000, 0);
  Call3(Actor_ArmCallback, 0x1b, 0xb000, 0);
  Call2(Func_0200454c, 0x19, 0x8000);
  Call2(Motion_SetModeAndWaitAnimation, 0x19, 3);
  Call2(Func_0200454c, 0x19, 0x5000);
  Call2(Func_02004534, 0x19, 0);
  Prox_Jump(0x1b, 0x109, 0x14);
  Call2(Func_02004534, 0x1b, 0);
  Call2(Actor_SetModeById, 0x1b, 4);
  Call2(Func_02004534, 0x1b, 0);
  Call3(Actor_ArmCallback, gAdventure.currentOwner, 0, 0);
  Call3(Actor_ArmCallback, 0, 0, 0);
  Call3(Actor_ArmCallback, 0x19, 0x3000, 0);
  Prox_Jump(gAdventure.currentOwner, 0x101, 0);
  Prox_Jump(0, 0x101, 0);
  Prox_Jump(0x1a, 0x101, 0);
  Prox_Jump(0x19, 0x101, 0x28);
  Call2(Motion_SetModeAndWaitAnimation, 0x1b, 3);
  Call2(Func_02004534, 0x1b, 0);
  Call2(Actor_SetModeById, 0, 3);
  Call2(Actor_SetModeById, gAdventure.currentOwner, 3);
  Call2(Actor_SetModeById, 0x1a, 3);
  Call2(Actor_SetModeById, 0x19, 3);
  Call3(Actor_ArmCallback, 0x1b, 0x8000, 0);
  Prox_Jump(0x1b, 0x108, 0x14);
  Call2(Func_02004534, 0x1b, 0);
  Prox_Jump(0x1a, 0x109, 0x14);
  Call3(Func_0200452c, 0x1a, 0, 0x14);
  Call2(Func_02004534, 5, 0);
  Call2(Func_02004564, 0x1b, 0x102);
  Call3(Actor_ArmCallback, gAdventure.currentOwner, 0x4000, 0);
  Call3(Actor_ArmCallback, 0, 0x4000, 0);
  Call2(Func_0200454c, 0x1b, 0x5000);
  Call2(Func_02004534, 0x401b, 0);
  Call2(Func_0200454c, 0, 0);
  Prox_MessageOpen(0x4000, 0);
  Call3(Actor_ArmCallback, gAdventure.currentOwner, 0x8000, 0);
  t144 = Value2(Event_AskYesNoWithActorAnimation, gAdventure.currentOwner, 0);
  if (t144 == 0) {
    Call2(Func_0200456c, 0x26666, 0x4ccc);
    Call4(Camera_MoveWithinMapBounds, 0xb80000, -1, 0x2000000, 1);
    Call2(Func_0200454c, 0, 0x4000);
    Call3(Actor_SetSpeedParameters, 0, 0x19999, 0xcccc);
    Call3(Actor_SetPositionAndReset, 0, 0xa8, 0x208);
    Call2(Func_02004534, 0x4000, 0);
    Call2(Func_02004534, 0x2005, 0);
    EVENT_WORK->messageId += 2;
  } else {
    EVENT_WORK->messageId += 2;
    Call2(Func_0200456c, 0x26666, 0x4ccc);
    Call4(Camera_MoveWithinMapBounds, 0xb80000, -1, 0x2000000, 1);
    Call2(Func_0200454c, 0, 0x4000);
    Call3(Actor_SetSpeedParameters, 0, 0x19999, 0xcccc);
    Call3(Actor_SetPositionAndReset, 0, 0xa8, 0x208);
    Call2(Func_02004534, 0x4000, 0);
    Call2(Func_02004534, 0x2005, 0);
  }
  t159 = Func_02004584();
  FIELD_AT((t159 + 0x55), u8, 0) = 0;
  Call4(Camera_MoveWithinMapBounds, 0x9e0000, -1, 0x1b00000, 1);
  Func_0200457c();
  Call3(Actor_Jump, 0x11, 4, 0xa);
  Call2(Func_02004534, 0x11, 0);
  Call2(Func_0200456c, 0x13333, 0x2666);
  Call4(Camera_MoveWithinMapBounds, 0xac0000, -1, 0x2080000, 1);
  Call2(Actor_WatchAndSetScript, 0, 0x11);
  Call2(Actor_WatchAndSetScript, gAdventure.currentOwner, 0x11);
  Call2(Actor_WatchAndSetScript, 0x1b, 0x11);
  Call3(Actor_SetSpeedParameters, 0x11, 0x19999, 0xcccc);
  Call3(Actor_SetPositionAndReset, 0x11, 0x8c, 0x1de);
  Call3(Actor_SetPositionAndReset, 0x11, 0x96, 0x21c);
  Call3(Actor_SetPositionAndReset, 0x11, 0xa0, 0x230);
  Call3(Actor_SetPositionAndReset, 0x11, 0xbe, 0x280);
  Call1(Actor_Stop, 0);
  Call1(Actor_Stop, gAdventure.currentOwner);
  Call1(Actor_Stop, 0x1b);
  Call1(WaitFrames, 1);
  Call3(Func_020044cc, 0x11, 0, 0);
  Call1(FieldEvent_WaitFrames, 0xa);
  Call2(Func_0200456c, 0x26666, 0x4ccc);
  Call4(Camera_MoveWithinMapBounds, 0xb80000, -1, 0x1c00000, 1);
  Call3(Actor_SetPositionAndReset, 0, 0xa8, 0x1e4);
  Call2(Func_0200454c, 0, 0);
  Call2(Func_02004534, 0x4000, 0);
  Call3(Actor_ArmCallback, gAdventure.currentOwner, 0x8000, 0);
  Call2(Func_0200454c, 0x1b, 0x8000);
  Call2(Func_02004534, 0x401b, 0);
  Call3(Actor_ArmCallback, 0x19, 0x5000, 0);
  Call2(Motion_SetModeAndWaitAnimation, 0x19, 3);
  Call2(Func_02004534, 0x19, 0);
  Call3(Actor_ArmCallback, 0, 0xc000, 0);
  Call3(Actor_ArmCallback, 0x1b, 0xb000, 0);
  Call2(Func_0200454c, gAdventure.currentOwner, 0xc000);
  Call2(Motion_SetModeAndWaitAnimation, 0x1a, 3);
  Call2(Func_02004534, 0x1a, 0);
  Call2(Actor_SetModeById, 0, 3);
  Call2(Actor_SetModeById, gAdventure.currentOwner, 3);
  Call2(Motion_SetModeAndWaitAnimation, 0x1b, 3);
  Call3(Actor_SetSpeedParameters, 0, 0x19999, 0xcccc);
  Call3(Actor_SetSpeedParameters, 0x1b, 0x19999, 0xcccc);
  Call2(Actor_SetModeById, 0, 2);
  t202 = Value1(Actor_GetById, gAdventure.currentOwner);
  if (t202 != 0) {
    Call3(Actor_ResetAndSetPosition, 0, FIELD_AT(t202, s16, 10),
          FIELD_AT(t202, s16, 18));
  }
  Call1(Actor_WaitForMoveById, 0);
  Call3(Func_020044cc, 0, 0, 0);
  Call2(Actor_SetModeById, 0x1b, 2);
  t207 = Value1(Actor_GetById, gAdventure.currentOwner);
  if (t207 != 0) {
    Call3(Actor_ResetAndSetPosition, 0x1b, FIELD_AT(t207, s16, 10),
          FIELD_AT(t207, s16, 18));
  }
  Call1(Actor_WaitForMoveById, 0x1b);
  Call3(Func_020044cc, 0x1b, 0, 0);
  {
    struct EventWork *work = EVENT_WORK;

    work->screenEffect = 0x209;
    work->delay = 0x10;
  }
  Call1(GameFlag_ClearBit, 0x12f);
  Call1(GameFlag_SetBit, 0x9c5);
  Func_0200446c();
}
