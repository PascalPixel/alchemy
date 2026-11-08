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
void Func_02000b78();
void Func_02000bac();
void Func_02002ec4();
void Func_0200306c();
void Func_020032c8();
void Func_02005230();
void Func_02005338();
void Func_020053b8();
void GameFlag_ClearBit();
s32 GameFlag_Test();
void Actor_SetSpritePriority();
s32 Actor_GetById();
void Actor_SetPositionAndResetMotion();
void Task_Add();
void Func_02000bf4();
void Func_02000c34();
void Func_02000f60();
void Func_020012c4();
void Func_020019d4();
void Func_02001cc4();
extern u8 gData_020054e0[];
extern u8 gData_02006298[];
extern u8 gData_0200629c[];
extern u8 gData_020062a0[];
extern u8 gData_020062a4[];
extern u8 gData_020062a8[];
extern u8 gData_020062aa[];

void Func_02002c1c(void) {
  s32 t1, t2, t9, t19, t21, t39, t44, p_r3;

  t1 = GameFlag_Test(0x109);
  if (t1 == 0)
    goto L_0200acd2;
  t2 = Actor_GetById(9);
  Call4((void (*)())Actor_SetPositionAndResetMotion, t2, PIXELS(216), 0,
        PIXELS(216));
  Func_02005230(11, 0, 0);
  Func_02005230(13, 0, 0);
  Func_02005230(14, 0, 0);
  Func_02005230(15, 0, 0);
  Func_02005230(16, 0, 0);
  t9 = Actor_GetById(10);
  Actor_SetPositionAndResetMotion(t9, PIXELS(376), 0, PIXELS(248));
  Func_02005230(12, 0, 0);
  Func_02005230(17, 0, 0);
  Func_02005230(18, 0, 0);
  Func_02005230(19, 0, 0);
  Func_02005230(20, 0, 0);
  GameFlag_ClearBit(SPLIT_CONSTANT(0x20e));
  GameFlag_ClearBit(0x20c);
  GameFlag_ClearBit(SPLIT_CONSTANT(0x20d));
L_0200acd2:
  t19 = GameFlag_Test(0x300);
  if (t19 == 0)
    goto L_0200acee;
  Call3((void (*)())Func_02005230, 8, PIXELS(216), PIXELS(216));
  goto L_0200ad0a;
L_0200acee:
  t21 = GameFlag_Test(0x301);
  if (t21 == 0)
    goto L_0200ad0a;
  Call3((void (*)())Func_02005230, 8, PIXELS(376), PIXELS(248));
L_0200ad0a:
  (*(s16 *)0x4000052) = 0xc08;
  (*(s16 *)0x4000050) = 0x3f10;
  FIELD_AT(gData_02006298, s32, 0) = 0;
  FIELD_AT(gData_0200629c, s32, 0) = 0;
  Value2((s32 (*)())Task_Add, (s32)Func_02000bf4, 0x480);
  Value2((s32 (*)())Task_Add, (s32)Func_02000c34, 0x480);
  Task_Add((s32)Func_02001cc4, 0x480);
  Func_02000b78(9);
  Func_02000b78(10);
  Func_02000bac(11);
  Func_02000bac(13);
  Func_02000bac(14);
  Func_02000bac(15);
  Func_02000bac(16);
  Func_02000bac(12);
  Func_02000bac(17);
  Func_02000bac(18);
  Func_02000bac(19);
  Func_02000bac(20);
  Func_020053b8((s32)&gData_020054e0);
  t39 = GameFlag_Test(0x109);
  if (t39 != 0)
    goto L_0200adda;
  if (FIELD_AT(&gMainRoutineWork, s16, 482) != 3) {
    p_r3 = FIELD_AT(&gMainRoutineWork, s16, 482);
    goto L_0200add2;
  }
  Func_0200306c();
  goto L_0200adda;
L_0200add2:
  if (p_r3 != 4)
    goto L_0200adda;
  Func_020032c8();
L_0200adda:
  FIELD_AT(gData_020062a0, s32, 0) = 0;
  FIELD_AT(gData_020062a4, s32, 0) = 0xfa0;
  FIELD_AT(gData_020062a8, s16, 0) = 0;
  FIELD_AT(gData_020062aa, s16, 0) = 0;
  Value2((s32 (*)())Task_Add, (s32)Func_02000f60, 0x480);
  Task_Add((s32)Func_020012c4, 0x480);
  t44 = GameFlag_Test(0x109);
  if (t44 != 0)
    goto L_0200ae24;
  if (FIELD_AT(&gMainRoutineWork, s16, 482) != 5)
    goto L_0200ae24;
  Func_02002ec4();
L_0200ae24:
  Value2((s32 (*)())Task_Add, (s32)Func_020019d4, 0x480);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
  Func_02005338(0);
}
