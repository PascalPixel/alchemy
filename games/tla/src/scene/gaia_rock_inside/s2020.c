#include "types.h"
#include "main_routine_work.h"
#include "event_inline.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
void Func_02004e3c();
void Func_02004e5c();
void Func_02004e64();
void Func_02004ec4();
s32 Random16Far();
extern u8 MsgGaiaRockInside01[];

void Func_02002020(void) {
  s32 t1, t2, t3, t26, t27, t28, t34, t35, t37, t38, t43, t44, t50, v_r10, v_r5,
      v_r6, v_r7, v_r8;

  t1 = ((s32)Actor_Get(8));
  v_r7 = t1;
  t2 = ((s32)Actor_Get(9));
  v_r8 = t2;
  t3 = ((s32)Actor_Get(13));
  v_r10 = t3;
  Func_02004e5c();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered((s32)&MsgGaiaRockInside01, 1);
  Event_PlaySound(193);
  Func_02004e3c(v_r10, 1);
  v_r5 = (s32)&gMainRoutineWork;
  v_r5 = (v_r5 + 0x214);
  Func_02004ec4(13, FIELD_AT(v_r5, s32, 0));
  Actor_SetDestination(13, 496, 712);
  FIELD_AT(v_r10, s32, 40) = 0x60000;
  FieldActor_WaitForMoveById(13);
  FieldEvent_Wait(45);
  Actor_FaceActor(8, FIELD_AT(v_r5, s32, 0), 0);
  Actor_FaceActor(9, FIELD_AT(v_r5, s32, 0), 0);
  Event_PlaySound(0x240);
  Actor_SetAnimation(8, 5);
  Actor_SetAnimation(9, 5);
  System_Wait(30);
  Actor_SetAnimation(8, 2);
  Actor_SetAnimation(9, 2);
  FieldEvent_Wait(30);
  Actor_SetAnimation(8, 3);
  Actor_SetAnimation(9, 3);
  FieldEvent_Wait(40);
  Event_PlaySound(191);
  t26 = Event_GetFlag(0xa62);
  v_r5 = 119;
  if (t26 != 0)
    goto L_0200a1d8;
  v_r5 = 19;
L_0200a10a:
  t27 = Random16Far();
  FIELD_AT(v_r7, s32, 40) = t27;
  t28 = Random16Far();
  FIELD_AT(v_r8, s32, 40) = t28;
  v_r5 = (v_r5 + -1);
  System_Wait(1);
  if (v_r5 >= 0)
    goto L_0200a10a;
  Actor_SetSpeed(8, 0x4000, 0xccc);
  Actor_SetSpeed(9, 0x4000, 0xccc);
  Actor_SetDestinationOffset(8, 24, 0);
  Actor_SetDestinationOffset(9, -24, 0);
  v_r6 = v_r7;
  v_r6 = (v_r6 + 86);
  v_r5 = 0;
  for (; v_r5 < 300 && FIELD_AT(v_r6, u8, 0) != 0; v_r5++) {
    t34 = Random16Far();
    FIELD_AT(v_r7, s32, 40) = t34;
    t35 = Random16Far();
    FIELD_AT(v_r8, s32, 40) = t35;
    System_Wait(1);
  }
  v_r5 = 9;
L_0200a18c:
  t37 = Random16Far();
  FIELD_AT(v_r7, s32, 40) = t37;
  t38 = Random16Far();
  FIELD_AT(v_r8, s32, 40) = t38;
  v_r5 = (v_r5 + -1);
  System_Wait(1);
  if (v_r5 >= 0)
    goto L_0200a18c;
  v_r5 = 40;
  MapGrid_CopyCells(119, 31, 1, 2, 89, v_r5);
  MapGrid_CopyCells(119, 31, 1, 2, 100, v_r5);
  Event_SetFlag(0xa62);
  goto L_0200a1f8;
L_0200a1d8:
  t43 = Random16Far();
  FIELD_AT(v_r7, s32, 40) = t43;
  t44 = Random16Far();
  FIELD_AT(v_r8, s32, 40) = t44;
  v_r5 = (v_r5 + -1);
  System_Wait(1);
  if (v_r5 >= 0)
    goto L_0200a1d8;
  FieldEvent_Wait(30);
L_0200a1f8:
  Actor_SetAnimation(8, 4);
  Actor_SetAnimation(9, 4);
  FieldEvent_Wait(30);
  t50 = ((s32)Actor_Get(gMainRoutineWork.currentOwner));
  if (t50 == 0)
    goto L_0200a22e;
  Actor_SetDestination(13, FIELD_AT(t50, s16, 10), FIELD_AT(t50, s16, 18));
L_0200a22e:
  FIELD_AT(v_r10, s32, 40) = 0x60000;
  FieldActor_WaitForMoveById(13);
  Actor_SetPosition(13, 0, 0);
  Func_02004e64();
}
