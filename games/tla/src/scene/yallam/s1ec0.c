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

  t1 = ((s32)Actor_Get(8));
  v6 = t1;
  Actor_FaceDirection(8, 0x3000, 0);
  Actor_FaceDirection(10, 0, 0);
  Actor_FaceDirection(9, 0x3000, 0);
  Actor_FaceDirection(11, 0x5000, 0);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_SOUTHWEST);
  FieldEvent_SetMessageId((s32)&MsgIfYouWantTo);
  v5 = v6;
  Event_ShowMessage(8, 0);
  v5 = (v5 + 102);
  Func_0200182c();
  FIELD_AT(v5, s16, 0) = 1;
L_02009f28:
  System_Wait(1);
  v7 = v6;
  v7 = (v7 + 100);
  if (FIELD_AT(v7, s16, 0) != 218)
    goto L_02009f28;
  FIELD_AT(v5, s16, 0) = 2;
  System_Wait(1);
  Event_ShowMessage(8, 0);
  FIELD_AT(v5, s16, 0) = 1;
L_02009f50:
  System_Wait(1);
  if (FIELD_AT(v7, s16, 0) != 254)
    goto L_02009f50;
  FIELD_AT(v5, s16, 0) = 2;
  System_Wait(1);
  Event_ShowMessage(8, 0);
  FIELD_AT(v5, s16, 0) = 1;
L_02009f74:
  System_Wait(1);
  if (FIELD_AT(v7, s16, 0) != 0x102)
    goto L_02009f74;
  FIELD_AT(v5, s16, 0) = 2;
  System_Wait(1);
  Event_ShowMessage(8, 0);
  FIELD_AT(v5, s16, 0) = 1;
L_02009f9c:
  System_Wait(1);
  if (FIELD_AT(v7, s16, 0) != 0x144)
    goto L_02009f9c;
  FIELD_AT(v5, s16, 0) = 3;
  System_Wait(1);
  Event_ShowMessage(8, 0);
  Func_02001730();
  Actor_FaceDirectionAndWait(8, 0x5000);
  t24 = ((s32)Actor_Get(8));
  FIELD_AT(t24, u8, 90) = (254 & FIELD_AT(t24, u8, 90));
  t25 = ((s32)Actor_Get(8));
  v6 = t25;
  v8 = FIELD_AT(v6, s32, 16);
  v7 = FIELD_AT(v6, s32, 8);
  Actor_SetSpeed(8, 0x26666, 0x13333);
  FieldActor_Jump(8, 6, 0);
  Actor_MoveToAndWait(8, 200, 0x29e);
  FieldActor_Jump(8, 4, 0);
  FieldActor_SetMoveTarget(v6, v7, 0, v8);
  FieldEvent_Wait(5);
  Actor_FaceDirectionAndWait(10, 0x5000);
  t33 = ((s32)Actor_Get(10));
  FIELD_AT(t33, u8, 90) = (254 & FIELD_AT(t33, u8, 90));
  t34 = ((s32)Actor_Get(10));
  v6 = t34;
  v8 = FIELD_AT(v6, s32, 16);
  v7 = FIELD_AT(v6, s32, 8);
  Actor_SetSpeed(10, 0x26666, 0x13333);
  FieldActor_Jump(10, 6, 0);
  Actor_MoveToAndWait(10, 200, 0x29e);
  FieldActor_Jump(10, 4, 0);
  FieldActor_SetMoveTarget(v6, v7, 0, v8);
  FieldEvent_Wait(5);
  Actor_FaceDirectionAndWait(9, FACING_WEST);
  t42 = ((s32)Actor_Get(9));
  FIELD_AT(t42, u8, 90) = (254 & FIELD_AT(t42, u8, 90));
  t43 = ((s32)Actor_Get(9));
  v6 = t43;
  v8 = FIELD_AT(v6, s32, 16);
  v7 = FIELD_AT(v6, s32, 8);
  Actor_SetSpeed(9, 0x26666, 0x13333);
  FieldActor_Jump(9, 6, 0);
  Actor_MoveToAndWait(9, 200, 0x29e);
  FieldActor_Jump(9, 4, 0);
  FieldActor_SetMoveTarget(v6, v7, 0, v8);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(11, 0xb000);
  t51 = ((s32)Actor_Get(11));
  v5 = (254 & FIELD_AT(t51, u8, 90));
  FIELD_AT(t51, u8, 90) = v5;
  t52 = ((s32)Actor_Get(11));
  v6 = t52;
  v8 = FIELD_AT(v6, s32, 16);
  v7 = FIELD_AT(v6, s32, 8);
  Actor_SetSpeed(11, 0x26666, 0x13333);
  FieldActor_Jump(11, 6, 0);
  Actor_MoveToAndWait(11, 200, 0x29e);
  FieldActor_Jump(11, 4, 0);
  FieldActor_SetMoveTarget(v6, v7, 0, v8);
  FieldActor_WaitForMoveById(11);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 184, 686);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 186, 674);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
  t63 = ((s32)Actor_Get(8));
  FIELD_AT(t63, u8, 90) = (FIELD_AT(t63, u8, 90) | 1);
  t64 = ((s32)Actor_Get(10));
  FIELD_AT(t64, u8, 90) = (FIELD_AT(t64, u8, 90) | 1);
  t65 = ((s32)Actor_Get(9));
  FIELD_AT(t65, u8, 90) = (FIELD_AT(t65, u8, 90) | 1);
  t66 = ((s32)Actor_Get(11));
  v5 = (1 | FIELD_AT(t66, u8, 90));
  FIELD_AT(t66, u8, 90) = v5;
  Actor_FaceDirection(8, 0x5000, 0);
  Actor_FaceDirection(10, FACING_WEST, 0);
  Actor_FaceDirection(9, FACING_WEST, 0);
  Actor_FaceDirection(11, FACING_WEST, 20);
  FieldEvent_ShowMessageThenWait(8, 0, 20);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(123);
  FieldEvent_Wait(40);
  FieldEvent_SetExitRequest(15);
}
