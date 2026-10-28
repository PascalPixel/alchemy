#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Actor_GetById();
s32 Func_02004464();
s32 Func_020045ec();
s32 Actor_SetSingleChildFlags();
s32 Actor_SetPositionAndResetMotion();
s32 wait();
s32 Event_StartScreenTransition();
s32 Motion_SetModeAndWaitAnimation();
s32 Actor_SetSpeedParameters();
s32 Actor_OffsetPositionAndResetMotion();
s32 Actor_WaitForMoveById();
s32 Actor_SetModeById();
s32 Func_0200446c();
extern u8 gMainRoutineWork[];

void Func_02001c24(s32 a0) {
  s32 t0;

  t0 = Value1(Actor_GetById, FIELD_AT(gMainRoutineWork, s32, 532));
  Func_02004464();
  Call1(Func_020045ec, 0);
  if (a0 != 0) {
    Call2(Actor_SetSingleChildFlags,
          Value1(Actor_GetById, FIELD_AT(gMainRoutineWork, s32, 532)), 0);
    Call4(Actor_SetPositionAndResetMotion, t0, (FIELD_AT(t0, s32, 8) + -65536),
          FIELD_AT(t0, s32, 12), (FIELD_AT(t0, s32, 16) + -393216));
    Call1(wait, 1);
    Event_StartScreenTransition();
    Call2(Motion_SetModeAndWaitAnimation, FIELD_AT(gMainRoutineWork, s32, 532),
          0x17);
  } else {
    Call4(Actor_SetPositionAndResetMotion, t0,
          (FIELD_AT(t0, s32, 8) + 0x10000), FIELD_AT(t0, s32, 12),
          (FIELD_AT(t0, s32, 16) + -393216));
    Call1(wait, 1);
    Event_StartScreenTransition();
    Call2(Motion_SetModeAndWaitAnimation, FIELD_AT(gMainRoutineWork, s32, 532),
          0x18);
  }
  Call2(Actor_SetSingleChildFlags,
        Value1(Actor_GetById, FIELD_AT(gMainRoutineWork, s32, 532)), 1);
  Call3(Actor_SetSpeedParameters, FIELD_AT(gMainRoutineWork, s32, 532),
        0x8000, 0x4000);
  Call3(Actor_OffsetPositionAndResetMotion,
        FIELD_AT(gMainRoutineWork, s32, 532), 0, 8);
  Call1(Actor_WaitForMoveById,
        FIELD_AT(gMainRoutineWork, s32, 532));
  Call2(Actor_SetModeById, FIELD_AT(gMainRoutineWork, s32, 532), 1);
  Func_0200446c();
}
