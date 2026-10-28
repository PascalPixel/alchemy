// Draft of overlay 6ad's Func_02000e7c, translated from its listing: differ
// 179 of 559. Left: its shape: 570 instructions against the listing's 559.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Camera_MoveWithinMapBounds();
s32 Event_StartScreenTransition();
s32 Event_WaitTransitionFrames();
s32 FieldEvent_Wait();
s32 Func_020002ec();
s32 Func_020003b4();
s32 Func_02000444();
s32 Func_02000480();
s32 Func_020004bc();
s32 Func_020006f4();
s32 Func_020009ec();
s32 Func_02000c80();
s32 Func_02000d80();
s32 Func_020015a4();
s32 Func_020025b0();
s32 Func_02002604();
s32 Func_02002624();
s32 Func_020027cc();
s32 Func_02003d80();
s32 Func_02004938();
s32 Func_02005284();
s32 Func_0200538c();
s32 Func_020053bc();
s32 Func_020054e8();
s32 Func_020054f0();
s32 Func_02005560();
s32 Func_02005598();
s32 Func_020055b8();
s32 Func_020055f0();
s32 Func_02005638();
s32 Func_02005648();
s32 Func_02005670();
s32 Func_02005678();
s32 Func_020056b0();
s32 Func_020056b8();
s32 Func_020056c0();
s32 GameFlag_SetBit();
s32 GameFlag_Test();
s32 Actor_GetById();
s32 Task_Add();
extern u8 Func_02000514[];
extern u8 Func_02000e68[];
extern u8 Func_02001468[];
extern u8 Func_02001490[];
extern u8 gData_02005bf0[];
extern u8 gData_02005bf6[];
extern u8 gData_02005bfc[];
extern u8 gData_02005c0e[];
extern u8 gData_02005c14[];
extern u8 gData_02005c26[];
extern u8 gData_02005c2c[];
extern u8 gData_02005c32[];
extern u8 gData_02005c38[];
extern u8 gData_02005c46[];
extern u8 gData_02005c4c[];
extern u8 gData_02005c5a[];
extern u8 gData_02005c60[];
extern u8 gData_02005c6e[];
extern u8 gMainRoutineWork[];

s32 Func_02000e7c(void) {
  s32 v_r5, v_r6, v_r8, v_r7, v_r10, v_r0, t0, t1, t2, t6;
  s32 t8, t11, t13, t15, t17, t26, t34, t45, t46, t58;
  s32 t59, t60, t61, t62, t64, t67, t70, t74, t79, t84;
  s32 t89, t90, t92, t94, t97, t100, t101, t105, t108, t109;
  s32 t111, t112, t113, t114, t115, t116, t117, t118, t119, t120;
  s32 t121;

  t0 = FIELD_AT((s32)gMainRoutineWork, s16, 480);
  t1 = FIELD_AT((s32)gMainRoutineWork, s16, 482);
  t2 = FIELD_AT(0x3000000, s32, 108);
  FIELD_AT(t2, s32, 428) = 0x204;
  Call1((void (*)())GameFlag_SetBit, 0x112);
  Func_020025b0();
  v_r10 = (s32)gMainRoutineWork;
  v_r7 = t1;
  v_r8 = 0x204;
  v_r6 = 0x3000000;
  v_r5 = t0;
  if (v_r5 != 0x124)
    goto L_02008f8e;
  Call1((void (*)())Func_02005638, 0);
  t6 = FIELD_AT(v_r6, s32, 108);
  FIELD_AT(t6, s32, 428) = v_r8;
  if ((u32)(v_r7 - 1) > (u32)1)
    goto L_02008f64;
  Call4((void (*)())Func_02002624, (s32)gData_02005bf0, (s32)gData_02005bf6,
        (s32)Func_02000514, 88);
  t8 = Value1((s32 (*)())GameFlag_Test, 0xa42);
  if (t8 == 0)
    goto L_02008f00;
  Call3((void (*)())Func_02005598, 10, 0, 0);
  goto L_02008f0a;
L_02008f00:
  Call3((void (*)())Func_02005598, 9, 0, 0);
L_02008f0a:
  t11 = Value1((s32 (*)())GameFlag_Test, 0xa43);
  if (t11 == 0)
    goto L_02008f1e;
  Call1((void (*)())Func_020006f4, 0);
L_02008f1e:
  t13 = Value1((s32 (*)())GameFlag_Test, 0xa44);
  if (t13 == 0)
    goto L_02008f34;
  Call2((void (*)())Func_02000444, 1, 0);
L_02008f34:
  t15 = Value1((s32 (*)())GameFlag_Test, 0xa45);
  if (t15 == 0)
    goto L_02008f4a;
  Call2((void (*)())Func_02000480, 1, 0);
L_02008f4a:
  t17 = Value1((s32 (*)())GameFlag_Test, 0xa46);
  if (t17 != 0)
    goto L_02008f5a;
  goto L_020093e0;
L_02008f5a:
  Call2((void (*)())Func_020004bc, 1, 0);
  goto L_020093e0;
L_02008f64:
  Call4((void (*)())Func_02002624, (s32)gData_02005bfc, (s32)gData_02005c0e, 0,
        88);
  Call1((void (*)())Func_020055b8, 15);
  Call1((void (*)())Func_020055b8, 16);
  Call1((void (*)())Func_02004938, 15);
  Call1((void (*)())Func_02004938, 16);
  Func_020003b4();
  goto L_020093e0;
L_02008f8e:
  if (v_r5 == 0x125)
    goto L_02008f96;
  goto L_0200913c;
L_02008f96:
  Call1((void (*)())Func_02005638, 0);
  t26 = FIELD_AT(v_r6, s32, 108);
  FIELD_AT(t26, s32, 428) = v_r8;
  Call4((void (*)())Func_02002624, (s32)gData_02005c14, (s32)gData_02005c26,
        (s32)Func_02001468, 88);
  Call2((void (*)())Func_020027cc, 3, 1);
  if (v_r7 != 2)
    goto L_02008fc4;
  Func_02002604();
L_02008fc4:
  if (v_r7 != 9)
    goto L_0200904a;
  Call1((void (*)())GameFlag_SetBit, 0x351);
  Func_02005560();
  Call1((void (*)())Func_02005648, 0);
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  t34 = FIELD_AT(v_r10, s32, 532);
  Call3((void (*)())Func_02005598, t34, 0, 0);
  Call3((void (*)())Func_02005598, 17, 0, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Call1((void (*)())FieldEvent_Wait, 30);
  Call3((void (*)())Func_02005598, 18, 0, 0);
  Call3((void (*)())Func_02005598, 17, 0x1080000, 0x2080000);
  Call1((void (*)())Func_020056c0, 136);
  Call1((void (*)())FieldEvent_Wait, 60);
  Call1((void (*)())GameFlag_SetBit, 0x109);
  t45 = Value1((s32 (*)())Func_020055f0, 3);
  v_r0 = t45;
  goto L_020093e2;
L_0200904a:
  t46 = Value1((s32 (*)())GameFlag_Test, 0x350);
  if (t46 == 0)
    goto L_02009092;
  Call3((void (*)())Func_02005598, 8, 0x2580000, 0x900000);
  Call3((void (*)())Func_02005598, 9, 0x2200000, 0xd00000);
  Call3((void (*)())Func_02005598, 10, 0x2200000, 0xf00000);
  Call1((void (*)())GameFlag_SetBit, 0x220);
  Call1((void (*)())GameFlag_SetBit, 0x221);
L_02009092:
  Call1((void (*)())Func_020055b8, 8);
  Call1((void (*)())Func_020055b8, 9);
  Call1((void (*)())Func_020055b8, 10);
  Call1((void (*)())Func_02004938, 8);
  Call1((void (*)())Func_02004938, 9);
  Call1((void (*)())Func_02004938, 10);
  t58 = Value1((s32 (*)())Actor_GetById, 11);
  t59 = FIELD_AT(t58, u8, 35);
  FIELD_AT(t58, u8, 35) = (t59 | 2);
  t60 = Value1((s32 (*)())Actor_GetById, 12);
  t61 = FIELD_AT(t60, u8, 35);
  v_r5 = (2 | t61);
  FIELD_AT(t60, u8, 35) = v_r5;
  t62 = Value1((s32 (*)())GameFlag_Test, 0x220);
  if (t62 == 0)
    goto L_020090e6;
  Call1((void (*)())Func_02000c80, 11);
L_020090e6:
  t64 = Value1((s32 (*)())GameFlag_Test, 0x221);
  if (t64 == 0)
    goto L_020090fa;
  Call1((void (*)())Func_02000c80, 12);
L_020090fa:
  Func_020009ec();
  t67 = Value1((s32 (*)())GameFlag_Test, 0x350);
  if (t67 == 0)
    goto L_02009116;
  Call3((void (*)())Func_02005598, 18, 0, 0);
  goto L_02009120;
L_02009116:
  Call3((void (*)())Func_02005598, 17, 0, 0);
L_02009120:
  t70 = Value1((s32 (*)())GameFlag_Test, 0xa40);
  if (t70 == 0)
    goto L_02009130;
  Func_02000d80();
L_02009130:
  Call2((void (*)())Task_Add, (s32)Func_02000e68, 0x480);
  goto L_020093e0;
L_0200913c:
  if (v_r5 == 0x126)
    goto L_02009144;
  goto L_020092b8;
L_02009144:
  Call1((void (*)())Func_02005638, 0);
  t74 = FIELD_AT(v_r6, s32, 108);
  FIELD_AT(t74, s32, 428) = v_r8;
  Call1((void (*)())Func_020015a4, 1);
  Func_02005284();
  Call1((void (*)())Func_020053bc, 2);
  Call2((void (*)())Func_0200538c, 11, 6);
  t79 = Value1((s32 (*)())GameFlag_Test, 0x358);
  if (t79 == 0)
    goto L_020091ba;
  Call6((void (*)())Func_020054f0, 71, 24, 7, 1, 71, 5);
  Call6((void (*)())Func_020054e8, 7, 24, 7, 1, 7, 7);
  Call6((void (*)())Func_020054e8, 7, 88, 7, 1, 7, 71);
  Call3((void (*)())Func_02005598, 11, 0, 0);
L_020091ba:
  t84 = Value1((s32 (*)())GameFlag_Test, 0xa37);
  if (t84 == 0)
    goto L_020091d6;
  Call3((void (*)())Func_02005598, 12, 0x780000, 0x1280000);
L_020091d6:
  if ((u32)(v_r7 - 14) > (u32)1)
    goto L_02009234;
  Call4((void (*)())Func_02002624, (s32)gData_02005c2c, (s32)gData_02005c32,
        (s32)Func_02001490, 88);
  Call2((void (*)())Func_020027cc, 0, 1);
  goto L_02009240;
L_02009234:
  Call4((void (*)())Func_02002624, (s32)gData_02005c38, (s32)gData_02005c46, 0,
        88);
L_02009240:
  t89 = Value1((s32 (*)())GameFlag_Test, 0x109);
  if (t89 == 0)
    goto L_0200924e;
  goto L_020093e0;
L_0200924e:
  if (v_r7 == 2)
    goto L_0200926e;
  if (v_r7 == 4)
    goto L_0200926e;
  if (v_r7 == 6)
    goto L_0200926e;
  if (v_r7 == 7)
    goto L_0200926e;
  if (v_r7 == 8)
    goto L_0200926e;
  if (v_r7 == 10)
    goto L_0200926e;
  if (v_r7 == 11)
    goto L_0200926e;
  if (v_r7 != 13)
    goto L_0200927c;
L_0200926e:
  t90 = FIELD_AT((s32)gMainRoutineWork, s32, 532);
  Call1((void (*)())Func_020056b0, t90);
L_0200927c:
  if (v_r7 == 3)
    goto L_02009290;
  if (v_r7 == 5)
    goto L_02009290;
  if (v_r7 == 9)
    goto L_02009290;
  if (v_r7 == 12)
    goto L_02009290;
  if (v_r7 != 14)
    goto L_0200929e;
L_02009290:
  t92 = FIELD_AT((s32)gMainRoutineWork, s32, 532);
  Call1((void (*)())Func_020056b8, t92);
L_0200929e:
  if (v_r7 == 16)
    goto L_020092a4;
  goto L_020093e0;
L_020092a4:
  t94 = Value1((s32 (*)())GameFlag_Test, 0x109);
  if (t94 == 0)
    goto L_020092b2;
  goto L_020093e0;
L_020092b2:
  Func_02003d80();
  goto L_020093e0;
L_020092b8:
  if (v_r5 == 0x127)
    goto L_020092c0;
  goto L_020093e0;
L_020092c0:
  Call1((void (*)())Func_02005638, 0);
  t97 = FIELD_AT(v_r6, s32, 108);
  FIELD_AT(t97, s32, 428) = v_r8;
  Func_02005670();
  Call3((void (*)())Func_02005678, 0, 8, 9);
  t100 = Value1((s32 (*)())Actor_GetById, 8);
  FIELD_AT(t100, u8, 98) = 1;
  t101 = Value1((s32 (*)())Actor_GetById, 9);
  FIELD_AT(t101, u8, 98) = 1;
  if ((u32)(v_r7 - 5) <= (u32)1)
    goto L_02009300;
  if (v_r7 != 8)
    goto L_02009312;
L_02009300:
  Call4((void (*)())Func_02002624, (s32)gData_02005c4c, (s32)gData_02005c5a, 0,
        88);
  Func_020002ec();
  goto L_02009332;
L_02009312:
  if (v_r7 == 11)
    goto L_02009326;
  if (v_r7 == 13)
    goto L_02009326;
  if (v_r7 == 14)
    goto L_02009326;
  if (v_r7 == 1)
    goto L_02009326;
  if (v_r7 != 15)
    goto L_02009332;
L_02009326:
  Call4((void (*)())Func_02002624, (s32)gData_02005c60, (s32)gData_02005c6e, 0,
        88);
L_02009332:
  t105 = Value1((s32 (*)())GameFlag_Test, 0x109);
  if (t105 != 0)
    goto L_02009362;
  if (v_r7 != 15)
    goto L_02009350;
  Call3((void (*)())Func_02005598, 23, 0x2980000, 0x2c80000);
L_02009350:
  if (v_r7 != 11)
    goto L_02009362;
  Call3((void (*)())Func_02005598, 17, 0x2180000, 0x2280000);
L_02009362:
  t108 = Value1((s32 (*)())Actor_GetById, 16);
  FIELD_AT(t108, u8, 85) = 0;
  t109 = Value1((s32 (*)())GameFlag_Test, 0xa38);
  v_r5 = 0;
  if (t109 == v_r5)
    goto L_0200939a;
  Call3((void (*)())Func_02005598, 16, 0x3280000, 0x1380000);
  t111 = Value1((s32 (*)())Actor_GetById, 16);
  FIELD_AT(t111, s32, 12) = v_r5;
  t112 = Value1((s32 (*)())Actor_GetById, 16);
  FIELD_AT(t112, s32, 20) = v_r5;
L_0200939a:
  t113 = FIELD_AT((s32)gMainRoutineWork, s32, 532);
  t114 = Value1((s32 (*)())Actor_GetById, t113);
  t115 = FIELD_AT(t114, u8, 35);
  FIELD_AT(t114, u8, 35) = (t115 | 64);
  t116 = Value1((s32 (*)())Actor_GetById, 10);
  t117 = FIELD_AT(t116, u8, 35);
  FIELD_AT(t116, u8, 35) = (t117 | 192);
  t118 = Value1((s32 (*)())Actor_GetById, 11);
  t119 = FIELD_AT(t118, u8, 35);
  FIELD_AT(t118, u8, 35) = (t119 | 128);
  t120 = Value1((s32 (*)())Actor_GetById, 12);
  t121 = FIELD_AT(t120, u8, 35);
  FIELD_AT(t120, u8, 35) = (t121 | 128);
L_020093e0:
  v_r0 = 0;
L_020093e2:
  return v_r0;
}
