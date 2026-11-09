#include "types.h"
#include "main_routine_work.h"
#include "call.h"
#include "scene_tables.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Animation_SetIndexAndInitializeObjects();
void Camera_FollowActor();
void Camera_MoveWithinMapBounds();
void Event_Wait();
void Func_020001bc();
s32 Func_02001bbc();
void Func_02001bcc();
void Func_02001bd4();
void Actor_SetHorizontalPositionWithTerrain();
void Event_SetMessageId();
void Event_ShowMessageThenWait();
void Event_ShowMessageAndWait();
void Actor_ArmCallbackAndRefresh();
void Func_080d47b4();
void Func_02001cc4();
void Event_ResolvePendingActions();
void Func_02001d2c();
void Func_02001d34();
void GameFlag_SetBit();
s32 GameFlag_Test();
void Motion_SetModeAndWaitAnimation();
void Actor_ArmCallback();
void Actor_CommitPositionAndActivate();
void Actor_Jump();
void Actor_SetPositionAndReset();
void Actor_SetSpeedParameters();
s32 Actor_GetById();
void Actor_SetModeById();
extern u8 MsgAreYouKids[];
extern u8 MsgTheDoorDoesn[];
extern u8 MsgWhatSThe[];

void Func_0200062c(void) {
  s32 t1, t2, t3, t4, t12, t30, t31, v5;

  t1 = Value1(Actor_GetById, gMainRoutineWork.currentOwner);
  /* FAKEMATCH(barrier): an empty asm ends the scheduling block here, as
   * the listing orders it; without it, 2 of 276 instructions differ in
   * tla-en. */
  asm("");
  if (FIELD_AT(t1, s32, 12) > 0x600000)
    goto L_02008654;
  t2 = Value1(GameFlag_Test, 0x969);
  if (t2 == 0)
    goto L_02008666;
L_02008654:
  FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x15a), s16, 0) = 1;
  goto L_020088d4;
L_02008666:
  t3 = Value1(GameFlag_Test, 0x966);
  if (t3 == 0)
    goto L_02008682;
  t4 = Value1(GameFlag_Test, 0x968);
  if (t4 == 0)
    goto L_02008694;
L_02008682:
  Call1(Event_SetMessageId, (s32)&MsgTheDoorDoesn);
  Call2(Event_ShowMessageAndWait, -1, 0);
  goto L_020088d4;
L_02008694:
  Call1(GameFlag_SetBit, 0x968);
  Func_02001bcc();
  Call1(Event_ResolvePendingActions, 0);
  Call1(Event_SetMessageId, (s32)&MsgAreYouKids);
  Call3(Actor_SetHorizontalPositionWithTerrain, 12, PIXELS(872), PIXELS(880));
  t12 = Value1(Actor_GetById, 12);
  Call2(Animation_SetIndexAndInitializeObjects, t12, 43);
  Call2(Actor_SetModeById, 12, 3);
  Call3(Actor_ArmCallback, 12, FACING_SOUTH + FACING_STEP, 0);
  Call3(Event_ShowMessageThenWait, 12, 0, 10);
  Call4(Camera_MoveWithinMapBounds, PIXELS(872), -1, PIXELS(776), 1);
  Func_02001cc4();
  Call1(Event_Wait, 20);
  Value3((s32 (*)())Func_080d47b4, 12, 0x109, 30);
  Call3(Event_ShowMessageThenWait, 12, 0, 10);
  Call2(Motion_SetModeAndWaitAnimation, 12, 4);
  Call3(Event_ShowMessageThenWait, 12, 0, 10);
  Call4(Camera_MoveWithinMapBounds, PIXELS(872), -1, PIXELS(856), 1);
  Func_02001cc4();
  Call1(Event_Wait, 20);
  Call3(Event_ShowMessageThenWait, 12, 0, 10);
  Call4(Camera_MoveWithinMapBounds, PIXELS(872), -1, PIXELS(928), 1);
  Func_02001cc4();
  t30 = Value1(Func_02001bbc, 133);
  v5 = t30;
  if (v5 == -1)
    goto L_02008778;
  goto L_020088cc;
L_02008778:
  t31 = Value1(GameFlag_Test, 0x962);
  if (t31 == 0)
    goto L_02008788;
  goto L_020088cc;
L_02008788:
  Event_SetMessageId((s32)&MsgWhatSThe);
  Event_Wait(20);
  Call3(Actor_SetPositionAndReset, 4, 856, 952);
  Actor_ArmCallbackAndRefresh(4, FACING_NORTH);
  Event_Wait(10);
  Value3((s32 (*)())Func_080d47b4, 4, 0x105, 60);
  Call4(Camera_MoveWithinMapBounds, PIXELS(872), v5, PIXELS(792), 1);
  Func_02001cc4();
  Event_Wait(20);
  Value3((s32 (*)())Func_080d47b4, 12, 0x101, 30);
  Event_ShowMessageThenWait(12, 0, 10);
  Call4(Camera_MoveWithinMapBounds, PIXELS(872), v5, PIXELS(856), 1);
  Actor_Jump(12, 4, 0);
  Actor_SetPositionAndReset(12, 872, 912);
  Event_Wait(20);
  Call3(Actor_SetSpeedParameters, 12, 0x10000, 0x8000);
  Call3(Actor_CommitPositionAndActivate, 12, -32, 0);
  Call2(Actor_ArmCallbackAndRefresh, 12, FACING_SOUTHEAST + FACING_STEP);
  Call4(Camera_MoveWithinMapBounds, PIXELS(848), v5, PIXELS(904), 1);
  Func_02001cc4();
  Event_Wait(15);
  Event_ShowMessageThenWait(12, 0, 5);
  Call3(Actor_SetHorizontalPositionWithTerrain, 28, PIXELS(840), PIXELS(920));
  Call3(Actor_SetHorizontalPositionWithTerrain, 27, PIXELS(840), PIXELS(936));
  Value3((s32 (*)())Func_02001d2c, 1, 28, 27);
  Func_02001d34(1, 28, 27);
  Event_Wait(30);
  Event_ShowMessageThenWait(12, 0, 5);
  Actor_CommitPositionAndActivate(12, 32, 0);
  Camera_FollowActor(4, 1);
  Call3(Actor_CommitPositionAndActivate, 12, 0, -32);
  GameFlag_SetBit(0x96f);
L_020088cc:
  Func_020001bc();
  Func_02001bd4();
L_020088d4:
}
