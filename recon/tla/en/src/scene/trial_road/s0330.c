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
s32 Event_AskYesNoWithActorAnimation();
void FieldEvent_Wait();
void Func_02005868();
void Func_02005880();
void Func_02005960();
void Func_02005990();
void Func_020059e8();
void Func_02005a68();
void GameFlag_SetBit();
s32 GameFlag_Test();
void Actor_ArmCallback();
void Actor_SetPositionAndReset();
s32 Actor_GetById();
void Actor_SetModeById();
void FieldMessage_Show();
extern u8 MsgTrialRoadGiveUpPrompt[];

s32 Func_02000330(void) {
  s32 t1, t2, t9, v_r5, v_r6;

  t1 = Actor_GetById(gMainRoutineWork.currentOwner);
  v_r5 = t1;
  v_r6 = FIELD_AT(0x3000000, s32, 108);
  t2 = GameFlag_Test(SPLIT_CONSTANT(0x900, 255));
  if (t2 == 0)
    goto L_02008400;
  Func_02005880();
  Func_020059e8(0);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner,
                            (((FIELD_AT(v_r5, s32, 8) >> 20) << 4) + 8),
                            (((FIELD_AT(v_r5, s32, 16) >> 20) << 4) + 8));
  Func_02005a68(229);
  Call3((void (*)())Actor_ArmCallback, gMainRoutineWork.currentOwner,
        FACING_SOUTH, 0);
  v_r5 = (s32)&MsgTrialRoadGiveUpPrompt;
  FieldMessage_Show(v_r5, 13);
  v_r5 = (v_r5 + 1);
  FIELD_AT((v_r6 + 0xcb0), s16, 0) = v_r5;
  FIELD_AT((v_r6 + 0xcb2), s16, 0) = 4;
  FIELD_AT((v_r6 + 0xcb4), s16, 0) = 1;
  t9 = Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 1);
  if (t9 != 0)
    goto L_02008400;
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  FieldEvent_Wait(10);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 22);
  FieldEvent_Wait(10);
  Func_02005960(gMainRoutineWork.currentOwner, 0x109, 0);
  FieldEvent_Wait(30);
  Func_02005868();
  Func_02005990(9);
  return 1;
L_02008400:
  GameFlag_SetBit(SPLIT_CONSTANT(0x182, 255));
  return 0;
}
