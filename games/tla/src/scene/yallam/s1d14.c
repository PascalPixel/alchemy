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
void Func_02001ec0();
extern u8 MsgIfYouWant[];

void Func_02001d14(void) {
  s32 t1, t33, v5, v6, v7;

  t1 = ((s32)Actor_Get(8));
  v7 = t1;
  Actor_FaceDirection(8, FACING_WEST, 0);
  Actor_FaceDirection(10, FACING_WEST, 0);
  Actor_FaceDirection(9, 0xb000, 0);
  Actor_FaceDirection(11, 0xb000, 0);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0);
  FieldEvent_SetMessageId((s32)&MsgIfYouWant);
  v5 = v7;
  Event_ShowMessage(8, 0);
  v5 = (v5 + 102);
  Func_0200182c();
  FIELD_AT(v5, s16, 0) = 1;
L_02009d78:
  System_Wait(1);
  v6 = v7;
  v6 = (v6 + 100);
  if (FIELD_AT(v6, s16, 0) != 114)
    goto L_02009d78;
  FIELD_AT(v5, s16, 0) = 2;
  System_Wait(1);
  Event_ShowMessage(8, 0);
  FIELD_AT(v5, s16, 0) = 1;
L_02009da0:
  System_Wait(1);
  if (FIELD_AT(v6, s16, 0) != 150)
    goto L_02009da0;
  FIELD_AT(v5, s16, 0) = 2;
  System_Wait(1);
  Event_ShowMessage(8, 0);
  FIELD_AT(v5, s16, 0) = 1;
L_02009dc4:
  System_Wait(1);
  if (FIELD_AT(v6, s16, 0) != 154)
    goto L_02009dc4;
  FIELD_AT(v5, s16, 0) = 2;
  System_Wait(1);
  Event_ShowMessage(8, 0);
  FIELD_AT(v5, s16, 0) = 1;
L_02009de8:
  System_Wait(1);
  if (FIELD_AT(v6, s16, 0) != 216)
    goto L_02009de8;
  FIELD_AT(v5, s16, 0) = 3;
  System_Wait(1);
  Event_ShowMessage(8, 0);
  Func_02001730();
  FieldActor_Jump(8, 4, 0);
  FieldActor_Jump(10, 4, 0);
  FieldActor_Jump(9, 4, 0);
  FieldActor_Jump(11, 4, 40);
  v5 = (s32)&gMainRoutineWork;
  v5 = (v5 + 0x214);
  Actor_FaceDirection(FIELD_AT(v5, s32, 0), FACING_SOUTH, 0);
  Actor_FaceDirection(8, 0xd000, 0);
  Actor_FaceDirection(10, 0xd000, 0);
  Actor_FaceDirection(9, 0xd000, 0);
  Actor_FaceDirectionAndWait(11, 0xd000);
  Event_OpenMessage(8, 0);
  t33 = FieldEvent_AskYesNoWithActorAnimation(FIELD_AT(v5, s32, 0), 0);
  if (t33 != 0)
    goto L_02009e90;
  Func_02001ec0();
  goto L_02009eb4;
L_02009e90:
  FieldEvent_ShowMessageThenWait(8, 0, 20);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(123);
  FieldEvent_Wait(40);
  FieldEvent_SetExitRequest(15);
L_02009eb4:
}
