#include "types.h"
#include "adventure.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Animation_SetIndexAndInitializeObjects();
void Camera_FollowActor();
void Camera_MoveWithinMapBounds();
void FieldEvent_WaitFrames();
void Func_020001bc();
s32 Func_02001bbc();
void Func_02001bcc();
void Func_02001bd4();
void Func_02001c2c();
void Func_02001c6c();
void Func_02001c7c();
void Func_02001c84();
void Func_02001c94();
void Func_02001ca4();
void Func_02001cc4();
void Func_02001d04();
void Func_02001d2c();
void Func_02001d34();
void GameFlag_SetBit();
s32 GameFlag_Test();
void Motion_SetModeAndWaitAnimation();
void ObjectMotion_ArmCallback();
void ObjectMotion_CommitPositionAndActivate();
void ObjectMotion_Launch();
void ObjectMotion_SetPositionAndReset();
void ObjectMotion_SetSpeedParameters();
s32 Object_GetById();
void Object_SetModeById();
extern u8 MsgAreYouKids[];
extern u8 MsgTheDoorDoesn[];
extern u8 MsgWhatSThe[];

void Func_0200062c(void) {
  s32 t1, t2, t3, t4, t12, t30, t31, v5;

  t1 = Value1(Object_GetById, gAdventure.currentOwner);
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
  Call1(Func_02001c6c, (s32)&MsgTheDoorDoesn);
  Call2(Func_02001c84, -1, 0);
  goto L_020088d4;
L_02008694:
  Call1(GameFlag_SetBit, 0x968);
  Func_02001bcc();
  Call1(Func_02001d04, 0);
  Call1(Func_02001c6c, (s32)&MsgAreYouKids);
  Call3(Func_02001c2c, 12, 0x3680000, 0x3700000);
  t12 = Value1(Object_GetById, 12);
  Call2(Animation_SetIndexAndInitializeObjects, t12, 43);
  Call2(Object_SetModeById, 12, 3);
  Call3(ObjectMotion_ArmCallback, 12, 0x5000, 0);
  Call3(Func_02001c7c, 12, 0, 10);
  Call4(Camera_MoveWithinMapBounds, 0x3680000, -1, 0x3080000, 1);
  Func_02001cc4();
  Call1(FieldEvent_WaitFrames, 20);
  Value3((s32 (*)())Func_02001ca4, 12, 0x109, 30);
  Call3(Func_02001c7c, 12, 0, 10);
  Call2(Motion_SetModeAndWaitAnimation, 12, 4);
  Call3(Func_02001c7c, 12, 0, 10);
  Call4(Camera_MoveWithinMapBounds, 0x3680000, -1, 0x3580000, 1);
  Func_02001cc4();
  Call1(FieldEvent_WaitFrames, 20);
  Call3(Func_02001c7c, 12, 0, 10);
  Call4(Camera_MoveWithinMapBounds, 0x3680000, -1, 0x3a00000, 1);
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
  Call1(Func_02001c6c, (s32)&MsgWhatSThe);
  Call1(FieldEvent_WaitFrames, 20);
  Call3(ObjectMotion_SetPositionAndReset, 4, 0x358, 0x3b8);
  Call2(Func_02001c94, 4, 0xc000);
  Call1(FieldEvent_WaitFrames, 10);
  Value3((s32 (*)())Func_02001ca4, 4, 0x105, 60);
  Call4(Camera_MoveWithinMapBounds, 0x3680000, v5, 0x3180000, 1);
  Func_02001cc4();
  Call1(FieldEvent_WaitFrames, 20);
  Value3((s32 (*)())Func_02001ca4, 12, 0x101, 30);
  Call3(Func_02001c7c, 12, 0, 10);
  Call4(Camera_MoveWithinMapBounds, 0x3680000, v5, 0x3580000, 1);
  Call3(ObjectMotion_Launch, 12, 4, 0);
  Call3(ObjectMotion_SetPositionAndReset, 12, 0x368, 0x390);
  Call1(FieldEvent_WaitFrames, 20);
  Call3(ObjectMotion_SetSpeedParameters, 12, 0x10000, 0x8000);
  Call3(ObjectMotion_CommitPositionAndActivate, 12, -32, 0);
  Call2(Func_02001c94, 12, 0x3000);
  Call4(Camera_MoveWithinMapBounds, 0x3500000, v5, 0x3880000, 1);
  Func_02001cc4();
  Call1(FieldEvent_WaitFrames, 15);
  Call3(Func_02001c7c, 12, 0, 5);
  Call3(Func_02001c2c, 28, 0x3480000, 0x3980000);
  Call3(Func_02001c2c, 27, 0x3480000, 0x3a80000);
  Value3((s32 (*)())Func_02001d2c, 1, 28, 27);
  Call3(Func_02001d34, 1, 28, 27);
  Call1(FieldEvent_WaitFrames, 30);
  Call3(Func_02001c7c, 12, 0, 5);
  Call3(ObjectMotion_CommitPositionAndActivate, 12, 32, 0);
  Call2(Camera_FollowActor, 4, 1);
  Call3(ObjectMotion_CommitPositionAndActivate, 12, 0, -32);
  Call1(GameFlag_SetBit, 0x96f);
L_020088cc:
  Func_020001bc();
  Func_02001bd4();
L_020088d4:
}
