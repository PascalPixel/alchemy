// Draft of overlay 664's Func_020051f8, translated from its listing.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Actor_GetById();
s32 GameFlag_SetBit();
s32 Func_02002770();
s32 wait();
s32 Func_02005d84();
s32 Task_Add();
s32 Func_02004eb0();
s32 GameFlag_Test();
s32 Func_02005e6c();
s32 Func_02005f6c();
s32 Actor_SetSpritePriority();
s32 Actor_SetPartAttribute();
s32 Func_02001084();
s32 Func_02005d9c();
s32 Func_02001420();
s32 Actor_SetSingleChildFlags();
s32 Func_0200505c();
s32 Func_02004cc8();
s32 Func_02005f4c();
s32 Func_02005f54();
s32 Func_02005f5c();
s32 Func_02005dac();
s32 Func_0200062c();
s32 Actor_SetActionCallback();
s32 Func_02000908();
extern u8 gMainRoutineWork[];
extern u8 Func_02004f3c[];
extern u8 gData_02006050[];
extern u8 gData_02006054[];
extern u8 Func_02001690[];
extern u8 gData_0200605a[];
extern u8 gData_02006064[];
extern u8 gData_0200621c[];
extern u8 gData_02006068[];
extern u8 gData_02006072[];
extern u8 gData_0200607e[];

void Func_020051f8(void) {
  u8 frame[-8];
  s32 v_r0, v_r1, v_r2, v_r3, v_r5, v_r6;

  v_r3 = FIELD_AT(0x3000000, s32, 108);
  v_r3 = v_r3 + 0x1ac;
  v_r5 = (s32)gMainRoutineWork;
  FIELD_AT(v_r3, s32, 0) = 0x204;
  v_r6 = v_r5 + 0x214;
  v_r0 = FIELD_AT(v_r6, s32, 0);
  v_r0 = Actor_GetById(v_r0, 0x1ac, 0x214, v_r3);
  v_r0 = v_r0 + 35;
  v_r2 = FIELD_AT(v_r0, u8, 0);
  v_r3 = 32 | v_r2;
  FIELD_AT(v_r0, u8, 0) = v_r3;
  Call1((void (*)())GameFlag_SetBit, 0x300);
  Call1((void (*)())GameFlag_SetBit, 0x89f);
  v_r3 = v_r5 + 0x1e0;
  v_r2 = FIELD_AT(v_r3, s16, 0);
  if (v_r2 == 85)
    goto L_ja_0200d234;
  goto L_ja_0200d3fc;
L_ja_0200d234:
  ;
  Func_02002770();
  v_r5 = v_r5 + 0x1e2;
  v_r3 = FIELD_AT(v_r5, s16, 0);
  if (v_r3 != 16)
    goto L_ja_0200d262;
  v_r0 = FIELD_AT(v_r6, s32, 0);
  v_r0 = Actor_GetById(v_r0, 0, 0x1e2, v_r3);
  v_r0 = v_r0 + 34;
  FIELD_AT(v_r0, u8, 0) = 1;
  Call1((void (*)())wait, 8);
  Func_02005d84();
  Call1((void (*)())wait, 1);
L_ja_0200d262:
  ;
  v_r3 = FIELD_AT(v_r5, s16, 0);
  if (v_r3 <= 11)
    goto L_ja_0200d26e;
  if (v_r3 != 16)
    goto L_ja_0200d28c;
L_ja_0200d26e:
  ;
  Call4((void (*)())Task_Add, (s32)Func_02004f3c, 0x480, v_r2, v_r3);
  FIELD_AT(0x4000052, u16, 0) = 0x1008;
  FIELD_AT(0x4000050, u16, 0) = 0x3f10;
  Func_02004eb0();
L_ja_0200d28c:
  ;
  v_r3 = (s32)gMainRoutineWork + 0x1e2;
  v_r3 = FIELD_AT(v_r3, s16, 0);
  if (v_r3 <= 13)
    goto L_ja_0200d2d4;
  v_r0 = GameFlag_Test(0xa8e, 0x1e2, 0, v_r3);
  if (v_r0 == 0)
    goto L_ja_0200d2ce;
  goto L_ja_0200d2c0;
L_ja_0200d2c0:
  ;
  Call3((void (*)())Func_02005e6c, 12, 0x3380000, 0x1480000);
L_ja_0200d2ce:
  ;
  Call1((void (*)())Func_02005f6c, (s32)gData_02006050);
L_ja_0200d2d4:
  ;
  v_r0 = Actor_GetById(10);
  v_r5 = 0;
  v_r0 = v_r0 + 85;
  FIELD_AT(v_r0, u8, 0) = v_r5;
  v_r0 = Actor_GetById(10);
  FIELD_AT(v_r0, s32, 12) = v_r5;
  v_r0 = Actor_GetById(10);
  v_r0 = v_r0 + 35;
  FIELD_AT(v_r0, u8, 0) = 2;
  Call2((void (*)())Actor_SetSpritePriority, 10, 3);
  v_r0 = GameFlag_Test(0x89a);
  if (v_r0 != 0)
    goto L_ja_0200d318;
  v_r0 = Actor_GetById(10);
  Actor_SetPartAttribute();
  goto L_ja_0200d346;
L_ja_0200d318:
  ;
  v_r0 = Actor_GetById(10);
  Func_02001084();
  v_r0 = Actor_GetById(11);
  Actor_SetPartAttribute();
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02005d9c, 64, 78, 18, 105);
L_ja_0200d346:
  ;
  v_r0 = GameFlag_Test(0x899);
  if (v_r0 != 0)
    goto L_ja_0200d37a;
  v_r0 = Actor_GetById(8);
  Actor_SetPartAttribute();
  v_r0 = Actor_GetById(9);
  Actor_SetPartAttribute();
  v_r0 = Actor_GetById(12);
  Actor_SetPartAttribute();
  goto L_ja_0200d3a4;
L_ja_0200d37a:
  ;
  v_r0 = Actor_GetById(8);
  Func_02001084();
  v_r0 = Actor_GetById(9);
  Func_02001084();
  v_r0 = Actor_GetById(12);
  Func_02001420();
L_ja_0200d3a4:
  ;
  v_r0 = Actor_GetById(8);
  Actor_SetSingleChildFlags();
  v_r0 = Actor_GetById(9);
  Actor_SetSingleChildFlags();
  v_r0 = Actor_GetById(12);
  Actor_SetSingleChildFlags();
  v_r3 = (s32)gMainRoutineWork + 0x1e2;
  v_r3 = FIELD_AT(v_r3, s16, 0);
  if (v_r3 == 11)
    goto L_ja_0200d3dc;
L_ja_0200d3dc:
  ;
  v_r0 = GameFlag_Test(0x109, v_r1, v_r2, v_r3);
  if (v_r0 == 0)
    goto L_ja_0200d3ec;
L_ja_0200d3ec:
  ;
  Func_0200505c();
L_ja_0200d3fc:
  ;
  if (v_r2 != 86)
    goto L_ja_0200d4e8;
  FIELD_AT(0x4000052, u16, 0) = 0x1008;
  FIELD_AT(0x4000050, u16, 0) = 0x3f10;
  Func_02002770();
  v_r0 = GameFlag_Test(0x897);
  if (v_r0 == 0)
    goto L_ja_0200d440;
  Call3((void (*)())Func_02005e6c, 9, 0x1480000, 0x1880000);
  goto L_ja_0200d440;
L_ja_0200d440:
  ;
  v_r0 = GameFlag_Test(0x898);
  if (v_r0 == 0)
    goto L_ja_0200d45c;
  Call3((void (*)())Func_02005e6c, 11, 0x680000, 0x1880000);
L_ja_0200d45c:
  ;
  Call1((void (*)())Func_02005f6c, (s32)gData_02006054);
  v_r0 = GameFlag_Test(0x899);
  if (v_r0 != 0)
    goto L_ja_0200d47e;
  v_r0 = Actor_GetById(8);
  Actor_SetPartAttribute();
  goto L_ja_0200d4ac;
L_ja_0200d47e:
  ;
  v_r0 = Actor_GetById(8);
  Func_02001084();
  v_r0 = Actor_GetById(10);
  Actor_SetPartAttribute();
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02005d9c, 68, 84, 12, 77);
L_ja_0200d4ac:
  ;
  v_r0 = Actor_GetById(8);
  Actor_SetSingleChildFlags();
  v_r3 = (s32)gMainRoutineWork + 0x1e2;
  v_r3 = FIELD_AT(v_r3, s16, 0);
  if (v_r3 == 3)
    goto L_ja_0200d4ca;
  goto L_ja_0200dc7c;
L_ja_0200d4ca:
  ;
  v_r0 = GameFlag_Test(0x109, v_r1, v_r2, v_r3);
  if (v_r0 == 0)
    goto L_ja_0200d4d8;
  goto L_ja_0200dc7c;
L_ja_0200d4d8:
  ;
  Func_02004cc8();
  goto L_ja_0200dc7c;
L_ja_0200d4e8:
  ;
  if (v_r2 == 87)
    goto L_ja_0200d4f0;
  goto L_ja_0200d61c;
L_ja_0200d4f0:
  ;
  Func_02002770();
  Call1((void (*)())Func_02005f4c, 245);
  FIELD_AT(0x4000052, u16, 0) = 0xc08;
  FIELD_AT(0x4000050, u16, 0) = 0x3f10;
  Call4((void (*)())Task_Add, (s32)Func_02001690, 0x480, 0x4000050, 0x3f10);
  v_r0 = GameFlag_Test(0x89b);
  if (v_r0 == 0)
    goto L_ja_0200d544;
  Call3((void (*)())Func_02005e6c, 8, 0x1080000, 0x2780000);
  goto L_ja_0200d544;
L_ja_0200d544:
  ;
  v_r0 = GameFlag_Test(0x89c);
  if (v_r0 == 0)
    goto L_ja_0200d560;
  Call3((void (*)())Func_02005e6c, 11, 0x2680000, 0x2380000);
L_ja_0200d560:
  ;
  v_r0 = GameFlag_Test(0x89d);
  if (v_r0 == 0)
    goto L_ja_0200d57c;
  Call3((void (*)())Func_02005e6c, 12, 0x1d80000, 0x2080000);
L_ja_0200d57c:
  ;
  Call1((void (*)())Func_02005f6c, (s32)gData_0200605a);
  v_r0 = GameFlag_Test(0x899);
  if (v_r0 != 0)
    goto L_ja_0200d5b6;
  v_r0 = Actor_GetById(9);
  Actor_SetPartAttribute();
  v_r0 = Actor_GetById(10);
  Actor_SetPartAttribute();
  v_r0 = Actor_GetById(13);
  Actor_SetPartAttribute();
  goto L_ja_0200d5e0;
L_ja_0200d5b6:
  ;
  v_r0 = Actor_GetById(9);
  Func_02001084();
  v_r0 = Actor_GetById(10);
  Func_02001084();
  v_r0 = Actor_GetById(13);
  Func_02001084();
L_ja_0200d5e0:
  ;
  v_r0 = Actor_GetById(9);
  v_r5 = 0;
  v_r0 = v_r0 + 90;
  FIELD_AT(v_r0, u8, 0) = v_r5;
  v_r0 = Actor_GetById(10);
  v_r0 = v_r0 + 90;
  FIELD_AT(v_r0, u8, 0) = v_r5;
  v_r0 = Actor_GetById(13);
  v_r0 = v_r0 + 90;
  FIELD_AT(v_r0, u8, 0) = v_r5;
  v_r0 = Actor_GetById(9);
  Actor_SetSingleChildFlags();
  v_r0 = Actor_GetById(10);
  Actor_SetSingleChildFlags();
  goto L_ja_0200db9e;
L_ja_0200d61c:
  ;
  if (v_r2 == 88)
    goto L_ja_0200d624;
  goto L_ja_0200d794;
L_ja_0200d624:
  ;
  Call1((void (*)())Func_02005f54, v_r0);
  Call3((void (*)())Func_02005f5c, 0, 11, 12);
  Call3((void (*)())Func_02005f5c, 1, 13, 14);
  Call3((void (*)())Func_02005f5c, 2, 15, 16);
  Call3((void (*)())Func_02005f5c, 3, 17, 18);
  Call3((void (*)())Func_02005f5c, 4, 19, 20);
  v_r0 = Actor_GetById(18);
  v_r0 = v_r0 + 98;
  FIELD_AT(v_r0, u8, 0) = 1;
  v_r0 = GameFlag_Test(0x301);
  if (v_r0 == 0)
    goto L_ja_0200d6c6;
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02005d9c, 69, 64, 29, 85);
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02005dac, 46, 20, 5, 5);
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02005dac, 32, 89, 1, 1);
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02005dac, 50, 86, 1, 1);
  goto L_ja_0200d716;
L_ja_0200d6c6:
  ;
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02005d9c, 64, 64, 29, 85);
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02005dac, 52, 20, 5, 5);
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02005dac, 33, 88, 1, 1);
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02005dac, 48, 88, 1, 1);
L_ja_0200d716:
  ;
  v_r0 = GameFlag_Test(0x300);
  if (v_r0 == 0)
    goto L_ja_0200d770;
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02005d9c, 90, 25, 86, 25);
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02005d9c, 21, 88, 24, 89);
  v_r5 = 23;
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02005dac, 23, 92, 2, 1);
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02005dac, 18, 33, 2, 1);
L_ja_0200d770:
  ;
  v_r0 = GameFlag_Test(0x89e);
  if (v_r0 == 0)
    goto L_ja_0200d78c;
  Call3((void (*)())Func_02005e6c, 9, 0x2880000, 0x1280000);
L_ja_0200d78c:
  ;
  Call1((void (*)())Func_02005f6c, (s32)gData_02006064);
  goto L_ja_0200dc7c;
L_ja_0200d794:
  ;
  if (v_r2 != 89)
    goto L_ja_0200d816;
  Call1((void (*)())GameFlag_SetBit, 0x144);
  Func_02002770();
  v_r6 = 0;
L_ja_0200d7a8:
  ;
  v_r0 = v_r6 + 0x202;
  v_r0 = GameFlag_Test(v_r0);
  if (v_r0 == 0)
    goto L_ja_0200d7e0;
  v_r3 = v_r6 << 1;
  v_r5 = 24;
  v_r5 = v_r5 - v_r3;
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02005d9c, 65, 1, 80, v_r5);
  Call4((void (*)())Func_0200062c, 0, 16, v_r5, 0xff00);
L_ja_0200d7e0:
  ;
  v_r6 = v_r6 + 1;
  if (v_r6 != 6)
    goto L_ja_0200d7a8;
  v_r0 = Actor_GetById(8);
  v_r0 = v_r0 + 85;
  FIELD_AT(v_r0, u8, 0) = 0;
  v_r0 = Actor_GetById(8);
  v_r5 = v_r0;
  v_r0 = Actor_GetById(8);
  FIELD_AT(v_r0, s32, 20) = 0x100000;
  FIELD_AT(v_r5, s32, 12) = 0x100000;
  v_r0 = Actor_GetById(8);
  Actor_SetActionCallback();
  goto L_ja_0200dc7c;
L_ja_0200d816:
  ;
  if (v_r2 == 90)
    goto L_ja_0200d81e;
  goto L_ja_0200d976;
L_ja_0200d81e:
  ;
  Func_02002770();
  Func_02005f54();
  Call3((void (*)())Func_02005f5c, 0, 11, 12);
  Call3((void (*)())Func_02005f5c, 1, 13, 14);
  Call3((void (*)())Func_02005f5c, 2, 15, 16);
  v_r0 = GameFlag_Test(0x302);
  if (v_r0 != 0)
    goto L_ja_0200d8b8;
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02005d9c, 69, 64, 32, 79);
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02005dac, 49, 15, 5, 5);
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02005dac, 33, 82, 1, 1);
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02005dac, 52, 81, 1, 1);
  goto L_ja_0200d908;
L_ja_0200d8b8:
  ;
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02005d9c, 64, 64, 32, 79);
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02005dac, 49, 21, 5, 5);
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02005dac, 35, 81, 1, 1);
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02005dac, 50, 82, 1, 1);
L_ja_0200d908:
  ;
  v_r0 = GameFlag_Test(0x303);
  if (v_r0 == 0)
    goto L_ja_0200d924;
  Call3((void (*)())Func_02005e6c, 8, 0x2080000, 0x1280000);
L_ja_0200d924:
  ;
  v_r0 = GameFlag_Test(0x89f);
  if (v_r0 != 0)
    goto L_ja_0200d934;
  goto L_ja_0200dc7c;
L_ja_0200d934:
  ;
  Call3((void (*)())Func_02005e6c, 8, 0x1380000, 0x1280000);
  Call2((void (*)())Actor_SetSpritePriority, 8, 2);
  v_r0 = Actor_GetById(8);
  v_r0 = v_r0 + 85;
  FIELD_AT(v_r0, u8, 0) = 2;
  v_r0 = Actor_GetById(8);
  FIELD_AT(v_r0, s32, 20) = -0x200000;
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02005dac, 19, 16, 1, 1);
  goto L_ja_0200dc7c;
L_ja_0200d976:
  ;
  if (v_r2 == 91)
    goto L_ja_0200d97e;
  goto L_ja_0200da90;
L_ja_0200d97e:
  ;
  Func_02002770();
  v_r0 = Actor_GetById(12);
  v_r6 = 1;
  v_r0 = v_r0 + 98;
  FIELD_AT(v_r0, u8, 0) = v_r6;
  v_r0 = Actor_GetById(13);
  v_r0 = v_r0 + 98;
  FIELD_AT(v_r0, u8, 0) = v_r6;
  v_r0 = Actor_GetById(14);
  v_r0 = v_r0 + 98;
  FIELD_AT(v_r0, u8, 0) = v_r6;
  v_r0 = Actor_GetById(15);
  v_r0 = v_r0 + 98;
  FIELD_AT(v_r0, u8, 0) = v_r6;
  v_r0 = GameFlag_Test(0x109);
  if (v_r0 != 0)
    goto L_ja_0200d9cc;
  Call1((void (*)())GameFlag_SetBit, 0x201);
  Call1((void (*)())GameFlag_SetBit, 0x202);
L_ja_0200d9cc:
  ;
  Call1((void (*)())Func_02000908, (s32)gData_0200621c);
  v_r0 = GameFlag_Test(0x8d0);
  if (v_r0 == 0)
    goto L_ja_0200d9ec;
  Call3((void (*)())Func_02005e6c, 8, 0x1580000, 0x2b80000);
L_ja_0200d9ec:
  ;
  Call1((void (*)())Func_02005f6c, (s32)gData_02006068);
  v_r0 = GameFlag_Test(0x304);
  if (v_r0 == 0)
    goto L_ja_0200da36;
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02005d9c, 39, 82, 38, 82);
  v_r5 = 38;
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02005dac, 39, 82, 1, 1);
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02005dac, 38, 19, 1, 1);
L_ja_0200da36:
  ;
  v_r0 = GameFlag_Test(0x305);
  if (v_r0 != 0)
    goto L_ja_0200da46;
  goto L_ja_0200dc7c;
L_ja_0200da46:
  ;
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02005d9c, 39, 82, 38, 99);
  v_r5 = 38;
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02005dac, 38, 98, 1, 1);
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02005dac, 38, 35, 1, 1);
  goto L_ja_0200dc7c;
L_ja_0200da90:
  ;
  if (v_r2 == 92)
    goto L_ja_0200da98;
  goto L_ja_0200dbaa;
L_ja_0200da98:
  ;
  Call1((void (*)())Func_02005f4c, 245);
  FIELD_AT(0x4000052, u16, 0) = 0xc08;
  FIELD_AT(0x4000050, u16, 0) = 0x3f10;
  Call4((void (*)())Task_Add, (s32)Func_02001690, 0x480, 0x4000050, 0x3f10);
  v_r0 = GameFlag_Test(0x8d1);
  if (v_r0 == 0)
    goto L_ja_0200dae8;
  Call3((void (*)())Func_02005e6c, 8, 0x1180000, 0x1d80000);
  goto L_ja_0200dae8;
L_ja_0200dae8:
  ;
  v_r0 = GameFlag_Test(0x8d2);
  if (v_r0 == 0)
    goto L_ja_0200db04;
  Call3((void (*)())Func_02005e6c, 9, 0x1680000, 0x3480000);
L_ja_0200db04:
  ;
  v_r0 = GameFlag_Test(0xa8f);
  if (v_r0 == 0)
    goto L_ja_0200db28;
  Call3((void (*)())Func_02005e6c, 12, 0xb80000, 0x2680000);
  Call1((void (*)())GameFlag_SetBit, 0x200);
L_ja_0200db28:
  ;
  Call2((void (*)())Actor_SetSpritePriority, 9, 2);
  Call1((void (*)())Func_02005f6c, (s32)gData_02006072);
  v_r0 = Actor_GetById(8);
  v_r5 = FIELD_AT(v_r0, s32, 8);
  v_r0 = Actor_GetById(8);
  v_r2 = FIELD_AT(v_r0, s32, 16);
  v_r5 = v_r5 >> 20;
  v_r2 = v_r2 >> 20;
  Call4((void (*)())Func_0200062c, 2, v_r5, v_r2, 0);
  v_r0 = Actor_GetById(9);
  v_r5 = FIELD_AT(v_r0, s32, 8);
  v_r0 = Actor_GetById(9);
  v_r2 = FIELD_AT(v_r0, s32, 16);
  v_r5 = v_r5 >> 20;
  v_r2 = v_r2 >> 20;
  Call4((void (*)())Func_0200062c, 2, v_r5, v_r2, 0);
  v_r0 = GameFlag_Test(0x899);
  if (v_r0 != 0)
    goto L_ja_0200db8e;
  v_r0 = Actor_GetById(11);
  Actor_SetPartAttribute();
  goto L_ja_0200db9c;
L_ja_0200db8e:
  ;
  v_r0 = Actor_GetById(11);
  Func_02001084();
L_ja_0200db9c:
  ;
L_ja_0200db9e:
  ;
  v_r0 = Actor_GetById(v_r0);
  Actor_SetSingleChildFlags();
  goto L_ja_0200dc7c;
L_ja_0200dbaa:
  ;
  if (v_r2 != 93)
    goto L_ja_0200dc7c;
  Func_02002770();
  v_r0 = GameFlag_Test(0x8d3);
  if (v_r0 == 0)
    goto L_ja_0200dbd0;
  Call3((void (*)())Func_02005e6c, 9, 0x1680000, 0x1080000);
L_ja_0200dbd0:
  ;
  Call1((void (*)())Func_02005f6c, (s32)gData_0200607e);
  v_r0 = Actor_GetById(10);
  v_r5 = FIELD_AT(v_r0, s32, 8);
  v_r0 = Actor_GetById(10);
  v_r2 = FIELD_AT(v_r0, s32, 16);
  v_r5 = v_r5 >> 20;
  v_r2 = v_r2 >> 20;
  Call4((void (*)())Func_0200062c, 2, v_r5, v_r2, 0);
  v_r0 = Actor_GetById(10);
  v_r0 = v_r0 + 90;
  FIELD_AT(v_r0, u8, 0) = 0;
  v_r0 = GameFlag_Test(0x899);
  if (v_r0 != 0)
    goto L_ja_0200dc1c;
  v_r0 = Actor_GetById(10);
  Actor_SetPartAttribute();
  goto L_ja_0200dc2a;
L_ja_0200dc1c:
  ;
  v_r0 = Actor_GetById(10);
  Func_02001084();
L_ja_0200dc2a:
  ;
  v_r0 = Actor_GetById(10);
  Actor_SetSingleChildFlags();
  v_r0 = GameFlag_Test(0x306);
  if (v_r0 == 0)
    goto L_ja_0200dc7c;
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02005d9c, 15, 80, 15, 77);
  v_r5 = 15;
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02005dac, 15, 80, 1, 1);
  /* XXX stack argument */;
  /* XXX stack argument */;
  Call4((void (*)())Func_02005dac, 17, 15, 2, 1);
L_ja_0200dc7c:
  ;
}
