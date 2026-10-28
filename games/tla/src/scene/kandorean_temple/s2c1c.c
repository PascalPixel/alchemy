#include "types.h"
#include "adventure.h"
#include "call.h"
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

  t1 = Value1((s32 (*)())GameFlag_Test, 0x109);
  if (t1 == 0)
    goto L_0200acd2;
  t2 = Value1((s32 (*)())Actor_GetById, 9);
  Call4((void (*)())Actor_SetPositionAndResetMotion, t2, 0xd80000, 0,
        0xd80000);
  Call3((void (*)())Func_02005230, 11, 0, 0);
  Call3((void (*)())Func_02005230, 13, 0, 0);
  Call3((void (*)())Func_02005230, 14, 0, 0);
  Call3((void (*)())Func_02005230, 15, 0, 0);
  Call3((void (*)())Func_02005230, 16, 0, 0);
  t9 = Value1((s32 (*)())Actor_GetById, 10);
  Call4((void (*)())Actor_SetPositionAndResetMotion, t9, 0x1780000, 0,
        0xf80000);
  Call3((void (*)())Func_02005230, 12, 0, 0);
  Call3((void (*)())Func_02005230, 17, 0, 0);
  Call3((void (*)())Func_02005230, 18, 0, 0);
  Call3((void (*)())Func_02005230, 19, 0, 0);
  Call3((void (*)())Func_02005230, 20, 0, 0);
  Call1((void (*)())GameFlag_ClearBit, 0x20e);
  Call1((void (*)())GameFlag_ClearBit, 0x20c);
  Call1((void (*)())GameFlag_ClearBit, 0x20d);
L_0200acd2:
  t19 = Value1((s32 (*)())GameFlag_Test, 0x300);
  if (t19 == 0)
    goto L_0200acee;
  Call3((void (*)())Func_02005230, 8, 0xd80000, 0xd80000);
  goto L_0200ad0a;
L_0200acee:
  t21 = Value1((s32 (*)())GameFlag_Test, 0x301);
  if (t21 == 0)
    goto L_0200ad0a;
  Call3((void (*)())Func_02005230, 8, 0x1780000, 0xf80000);
L_0200ad0a:
  (*(s16 *)0x4000052) = 0xc08;
  (*(s16 *)0x4000050) = 0x3f10;
  FIELD_AT(gData_02006298, s32, 0) = 0;
  FIELD_AT(gData_0200629c, s32, 0) = 0;
  Value2((s32 (*)())Task_Add, (s32)Func_02000bf4, 0x480);
  Value2((s32 (*)())Task_Add, (s32)Func_02000c34, 0x480);
  Call2((void (*)())Task_Add, (s32)Func_02001cc4, 0x480);
  Call1((void (*)())Func_02000b78, 9);
  Call1((void (*)())Func_02000b78, 10);
  Call1((void (*)())Func_02000bac, 11);
  Call1((void (*)())Func_02000bac, 13);
  Call1((void (*)())Func_02000bac, 14);
  Call1((void (*)())Func_02000bac, 15);
  Call1((void (*)())Func_02000bac, 16);
  Call1((void (*)())Func_02000bac, 12);
  Call1((void (*)())Func_02000bac, 17);
  Call1((void (*)())Func_02000bac, 18);
  Call1((void (*)())Func_02000bac, 19);
  Call1((void (*)())Func_02000bac, 20);
  Call1((void (*)())Func_020053b8, (s32)&gData_020054e0);
  t39 = Value1((s32 (*)())GameFlag_Test, 0x109);
  if (t39 != 0)
    goto L_0200adda;
  if (FIELD_AT(&gAdventure, s16, 482) != 3) {
    p_r3 = FIELD_AT(&gAdventure, s16, 482);
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
  Call2((void (*)())Task_Add, (s32)Func_020012c4, 0x480);
  t44 = Value1((s32 (*)())GameFlag_Test, 0x109);
  if (t44 != 0)
    goto L_0200ae24;
  if (FIELD_AT(&gAdventure, s16, 482) != 5)
    goto L_0200ae24;
  Func_02002ec4();
L_0200ae24:
  Value2((s32 (*)())Task_Add, (s32)Func_020019d4, 0x480);
  Call2((void (*)())Actor_SetSpritePriority, gAdventure.currentOwner, 2);
  Call1((void (*)())Func_02005338, 0);
}
