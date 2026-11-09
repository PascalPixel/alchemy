#include "types.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "event_inline.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Event_FinishScreenTransition();
void Event_WaitTransitionFrames();
void Func_02001730();
void Func_0200182c();
extern u8 MsgIfYouWantTo[];

void Func_02001ec0(void) {
  s32 t1, t24, t25, t33, t34, t42, t43, t51, t52, t63, t64, t65, t66, v5, v6,
      v7, v8;

  t1 = ((s32)Actor_GetById(8));
  v6 = t1;
  Actor_FaceDirection(8, 0x3000, 0);
  Actor_ArmCallback(10, 0, 0);
  Actor_FaceDirection(9, 0x3000, 0);
  Actor_FaceDirection(11, 0x5000, 0);
  Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, FACING_SOUTHWEST);
  Event_SetMessageId((s32)&MsgIfYouWantTo);
  v5 = v6;
  Event_ShowMessageAndWait(8, 0);
  v5 = (v5 + 102);
  Func_0200182c();
  FIELD_AT(v5, s16, 0) = 1;
L_02009f28:
  wait(1);
  v7 = v6;
  v7 = (v7 + 100);
  if (FIELD_AT(v7, s16, 0) != 218)
    goto L_02009f28;
  FIELD_AT(v5, s16, 0) = 2;
  wait(1);
  Event_ShowMessageAndWait(8, 0);
  FIELD_AT(v5, s16, 0) = 1;
L_02009f50:
  wait(1);
  if (FIELD_AT(v7, s16, 0) != 254)
    goto L_02009f50;
  FIELD_AT(v5, s16, 0) = 2;
  wait(1);
  Event_ShowMessageAndWait(8, 0);
  FIELD_AT(v5, s16, 0) = 1;
L_02009f74:
  wait(1);
  if (FIELD_AT(v7, s16, 0) != 0x102)
    goto L_02009f74;
  FIELD_AT(v5, s16, 0) = 2;
  wait(1);
  Event_ShowMessageAndWait(8, 0);
  FIELD_AT(v5, s16, 0) = 1;
L_02009f9c:
  wait(1);
  if (FIELD_AT(v7, s16, 0) != 0x144)
    goto L_02009f9c;
  FIELD_AT(v5, s16, 0) = 3;
  wait(1);
  Event_ShowMessageAndWait(8, 0);
  Func_02001730();
  Actor_FaceDirectionAndWait(8, 0x5000);
  t24 = ((s32)Actor_GetById(8));
  FIELD_AT(t24, u8, 90) = (254 & FIELD_AT(t24, u8, 90));
  t25 = ((s32)Actor_GetById(8));
  v6 = t25;
  v8 = FIELD_AT(v6, s32, 16);
  v7 = FIELD_AT(v6, s32, 8);
  Actor_SetSpeed(8, 0x26666, 0x13333);
  Actor_Jump(8, 6, 0);
  Actor_MoveToAndWait(8, 200, 0x29e);
  Actor_Jump(8, 4, 0);
  Actor_SetMoveTarget(v6, v7, 0, v8);
  Event_Wait(5);
  Actor_FaceDirectionAndWait(10, 0x5000);
  t33 = ((s32)Actor_GetById(10));
  FIELD_AT(t33, u8, 90) = (254 & FIELD_AT(t33, u8, 90));
  t34 = ((s32)Actor_GetById(10));
  v6 = t34;
  v8 = FIELD_AT(v6, s32, 16);
  v7 = FIELD_AT(v6, s32, 8);
  Actor_SetSpeed(10, 0x26666, 0x13333);
  Actor_Jump(10, 6, 0);
  Actor_MoveToAndWait(10, 200, 0x29e);
  Actor_Jump(10, 4, 0);
  Actor_SetMoveTarget(v6, v7, 0, v8);
  Event_Wait(5);
  Actor_FaceDirectionAndWait(9, FACING_WEST);
  t42 = ((s32)Actor_GetById(9));
  FIELD_AT(t42, u8, 90) = (254 & FIELD_AT(t42, u8, 90));
  t43 = ((s32)Actor_GetById(9));
  v6 = t43;
  v8 = FIELD_AT(v6, s32, 16);
  v7 = FIELD_AT(v6, s32, 8);
  Actor_SetSpeed(9, 0x26666, 0x13333);
  Actor_Jump(9, 6, 0);
  Actor_MoveToAndWait(9, 200, 0x29e);
  Actor_Jump(9, 4, 0);
  Actor_SetMoveTarget(v6, v7, 0, v8);
  Event_Wait(10);
  Actor_ArmCallbackAndRefresh(11, 0xb000);
  t51 = ((s32)Actor_GetById(11));
  v5 = (254 & FIELD_AT(t51, u8, 90));
  FIELD_AT(t51, u8, 90) = v5;
  t52 = ((s32)Actor_GetById(11));
  v6 = t52;
  v8 = FIELD_AT(v6, s32, 16);
  v7 = FIELD_AT(v6, s32, 8);
  Actor_SetSpeed(11, 0x26666, 0x13333);
  Actor_Jump(11, 6, 0);
  Actor_MoveToAndWait(11, 200, 0x29e);
  Actor_Jump(11, 4, 0);
  Actor_SetMoveTarget(v6, v7, 0, v8);
  Actor_WaitForMoveById(11);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 184, 686);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 186, 674);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
  t63 = ((s32)Actor_GetById(8));
  FIELD_AT(t63, u8, 90) = (FIELD_AT(t63, u8, 90) | 1);
  t64 = ((s32)Actor_GetById(10));
  FIELD_AT(t64, u8, 90) = (FIELD_AT(t64, u8, 90) | 1);
  t65 = ((s32)Actor_GetById(9));
  FIELD_AT(t65, u8, 90) = (FIELD_AT(t65, u8, 90) | 1);
  t66 = ((s32)Actor_GetById(11));
  v5 = (1 | FIELD_AT(t66, u8, 90));
  FIELD_AT(t66, u8, 90) = v5;
  Actor_ArmCallback(8, 0x5000, 0);
  Actor_FaceDirection(10, FACING_WEST, 0);
  Actor_FaceDirection(9, FACING_WEST, 0);
  Actor_FaceDirection(11, FACING_WEST, 20);
  Event_ShowMessageThenWait(8, 0, 20);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Sound_Play(123);
  Event_Wait(40);
  Import_EventSetExitRequest(15);
}
