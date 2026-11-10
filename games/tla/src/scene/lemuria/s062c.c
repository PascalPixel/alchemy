#include "types.h"
#include "main_routine_work.h"
#include "call.h"
#include "scene_tables.h"
#include "event_inline.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Animation_SetIndexAndInitializeObjects();
void Func_020001bc();
s32 Func_02001bbc();
void Func_02001bcc();
void Func_02001bd4();
void Func_02001cc4();
void Func_02001d2c();
void Func_02001d34();
extern u8 MsgAreYouKids[];
extern u8 MsgTheDoorDoesn[];
extern u8 MsgWhatSThe[];

void Func_0200062c(void) {
  s32 t1, t2, t3, t4, t12, t30, t31, v5;

  t1 = (s32)Actor_Get(gMainRoutineWork.currentOwner);
  /* FAKEMATCH(dead): an empty once-only block ends the scheduling block here,
   * as the listing orders it; without it, 2 of 276 instructions differ in
   * tla-en. */
  do {
  } while (0);
  if (FIELD_AT(t1, s32, 12) > 0x600000)
    goto L_02008654;
  t2 = Event_GetFlag(0x969);
  if (t2 == 0)
    goto L_02008666;
L_02008654:
  FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x15a), s16, 0) = 1;
  goto L_020088d4;
L_02008666:
  t3 = Event_GetFlag(0x966);
  if (t3 == 0)
    goto L_02008682;
  t4 = Event_GetFlag(0x968);
  if (t4 == 0)
    goto L_02008694;
L_02008682:
  FieldEvent_SetMessageId((s32)&MsgTheDoorDoesn);
  Event_ShowMessage(-1, 0);
  goto L_020088d4;
L_02008694:
  Event_SetFlag(0x968);
  Func_02001bcc();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)&MsgAreYouKids);
  Actor_SetPosition(12, PIXELS(872), PIXELS(880));
  t12 = (s32)Actor_Get(12);
  Call2(Animation_SetIndexAndInitializeObjects, t12, 43);
  Actor_SetAnimation(12, 3);
  Actor_FaceDirection(12, FACING_SOUTH + FACING_STEP, 0);
  FieldEvent_ShowMessageThenWait(12, 0, 10);
  FieldCamera_MoveTo(PIXELS(872), -1, PIXELS(776), 1);
  Func_02001cc4();
  FieldEvent_Wait(20);
  Actor_ShowEmote(12, 0x109, 30);
  FieldEvent_ShowMessageThenWait(12, 0, 10);
  Actor_SetAnimationAndWait(12, 4);
  FieldEvent_ShowMessageThenWait(12, 0, 10);
  FieldCamera_MoveTo(PIXELS(872), -1, PIXELS(856), 1);
  Func_02001cc4();
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(12, 0, 10);
  FieldCamera_MoveTo(PIXELS(872), -1, PIXELS(928), 1);
  Func_02001cc4();
  t30 = Value1(Func_02001bbc, 133);
  v5 = t30;
  if (v5 == -1)
    goto L_02008778;
  goto L_020088cc;
L_02008778:
  t31 = Event_GetFlag(0x962);
  if (t31 == 0)
    goto L_02008788;
  goto L_020088cc;
L_02008788:
  FieldEvent_SetMessageId((s32)&MsgWhatSThe);
  FieldEvent_Wait(20);
  Actor_WalkToAndWait(4, 856, 952);
  Actor_FaceDirectionAndWait(4, FACING_NORTH);
  FieldEvent_Wait(10);
  Actor_ShowEmote(4, 0x105, 60);
  FieldCamera_MoveTo(PIXELS(872), v5, PIXELS(792), 1);
  Func_02001cc4();
  FieldEvent_Wait(20);
  Actor_ShowEmote(12, 0x101, 30);
  FieldEvent_ShowMessageThenWait(12, 0, 10);
  FieldCamera_MoveTo(PIXELS(872), v5, PIXELS(856), 1);
  FieldActor_Jump(12, 4, 0);
  Actor_WalkToAndWait(12, 872, 912);
  FieldEvent_Wait(20);
  Actor_SetSpeed(12, 0x10000, 0x8000);
  Actor_WalkByAndWait(12, -32, 0);
  Actor_FaceDirectionAndWait(12, FACING_SOUTHEAST + FACING_STEP);
  FieldCamera_MoveTo(PIXELS(848), v5, PIXELS(904), 1);
  Func_02001cc4();
  FieldEvent_Wait(15);
  FieldEvent_ShowMessageThenWait(12, 0, 5);
  Actor_SetPosition(28, PIXELS(840), PIXELS(920));
  Actor_SetPosition(27, PIXELS(840), PIXELS(936));
  Value3((s32 (*)())Func_02001d2c, 1, 28, 27);
  Func_02001d34(1, 28, 27);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(12, 0, 5);
  Actor_WalkByAndWait(12, 32, 0);
  FieldCamera_FollowActor(4, 1);
  Actor_WalkByAndWait(12, 0, -32);
  Event_SetFlag(0x96f);
L_020088cc:
  Func_020001bc();
  Func_02001bd4();
L_020088d4:
}
