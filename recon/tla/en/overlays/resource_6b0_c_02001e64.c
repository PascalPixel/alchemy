// Draft of overlay 6b0's Func_02001e64, translated from its listing: differ
// 263 of 913. Left: its shape: 926 instructions against the listing's 913.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_02000038();
s32 Func_02000a78();
s32 Func_02001d68();
s32 Func_02001df4();
s32 Func_02002a74();
s32 Func_02003190();
s32 Func_02003328();
s32 Func_02003380();
s32 Func_020033e0();
s32 Func_02003490();
s32 GameFlag_SetBit();
s32 GameFlag_Test();
s32 Actor_SetSpritePriority();
s32 Actor_GetById();
s32 Actor_SetModeById();
extern u8 gData_020036e4[];
extern u8 gData_020036ea[];
extern u8 gData_020036f6[];
extern u8 gData_0200384c[];
extern u8 gData_0200385c[];
extern u8 gData_02003888[];
extern u8 gData_020038b4[];
extern u8 gAdventure[];

s32 Func_02001e64(void) {
  s32 v_r1, v_r6, v_r7, v_r2, v_r5, v_r3, v_r0, t0, t1, t3;
  s32 t6, t9, t12, t15, t19, t22, t25, t26, t27, t28;
  s32 t29, t30, t31, t32, t35, t36, t38, t39, t41, t42;
  s32 t43, t45, t47, t49, t51, t53, t55, t57, t58, t59;
  s32 t60, t61, t62, t63, t64, t65, t66, t68, t72, t74;
  s32 t75, t76, t77, t78, t79, t80, t84, t87, t88, t90;
  s32 t91, t92, t93, t94, t95, t96, t105, t106, t107, t109;
  s32 t111, t113, t115, t117, t119, t123, t126, t127, t128, t130;
  s32 t131, t132, t134, t135, t136, t138, t139, t141, t142, t144;
  s32 t149, t154, t155;

  t0 = FIELD_AT(0x3000000, s32, 108);
  FIELD_AT(t0, s32, 428) = 0x204;
  t1 = FIELD_AT((s32)gAdventure, s16, 480);
  v_r2 = 0x3000000;
  v_r7 = 0x204;
  v_r6 = (s32)gAdventure;
  v_r1 = t1;
  if (v_r1 != 0x12f)
    goto L_02009f86;
  Call4((void (*)())Func_02000038, (s32)gData_0200384c, (s32)gData_0200385c,
        (s32)gData_02003888, (s32)gData_020038b4);
  t3 = Value1((s32 (*)())GameFlag_Test, 0xa87);
  if (t3 == 0)
    goto L_02009ec6;
  Call3((void (*)())Func_02000a78, 0x208, 48, 0);
  Call6((void (*)())Func_02003328, 85, 34, 85, 8, 9, 8);
L_02009ec6:
  t6 = Value1((s32 (*)())GameFlag_Test, 0xa88);
  if (t6 == 0)
    goto L_02009ef4;
  Call3((void (*)())Func_02000a78, 0x228, 48, 3);
  Call6((void (*)())Func_02003328, 101, 34, 101, 8, 9, 8);
L_02009ef4:
  t9 = Value1((s32 (*)())GameFlag_Test, 0xa89);
  if (t9 == 0)
    goto L_02009f22;
  Call3((void (*)())Func_02000a78, 0x1f8, 40, 10);
  Call6((void (*)())Func_02003328, 85, 46, 85, 20, 9, 8);
L_02009f22:
  t12 = Value1((s32 (*)())GameFlag_Test, 0xa8a);
  if (t12 == 0)
    goto L_02009f50;
  Call3((void (*)())Func_02000a78, 0x238, 40, 2);
  Call6((void (*)())Func_02003328, 101, 46, 101, 20, 9, 8);
L_02009f50:
  t15 = Value1((s32 (*)())GameFlag_Test, 0xa8b);
  if (t15 != 0)
    goto L_02009f60;
  goto L_0200a6e6;
L_02009f60:
  Call6((void (*)())Func_02003328, 54, 7, 33, 7, 1, 2);
  Call4((void (*)())Func_02003380, 0x2180000, 0x880000, 0, 0);
  goto L_0200a6e6;
L_02009f86:
  if (v_r1 == 0x130)
    goto L_02009f8e;
  goto L_0200a1b4;
L_02009f8e:
  Call2((void (*)())Actor_SetSpritePriority, 8, 1);
  t19 = Value1((s32 (*)())GameFlag_Test, 0xa80);
  if (t19 == 0)
    goto L_02009fb8;
  Call3((void (*)())Func_020033e0, 8, 0x1280000, 0xf80000);
  Call2((void (*)())Actor_SetSpritePriority, 8, 2);
L_02009fb8:
  t22 = Value1((s32 (*)())GameFlag_Test, 0xa81);
  if (t22 == 0)
    goto L_02009fd4;
  Call3((void (*)())Func_020033e0, 10, 0x1b80000, 0x2f80000);
L_02009fd4:
  Call1((void (*)())Func_02003490, (s32)gData_020036e4);
  t25 = FIELD_AT(v_r6, s32, 532);
  t26 = Value1((s32 (*)())Actor_GetById, t25);
  t27 = FIELD_AT(t26, u8, 35);
  FIELD_AT(t26, u8, 35) = (64 | t27);
  t28 = Value1((s32 (*)())Actor_GetById, 9);
  FIELD_AT(t28, u8, 85) = 0;
  t29 = Value1((s32 (*)())Actor_GetById, 9);
  t30 = FIELD_AT(t29, u8, 35);
  FIELD_AT(t29, u8, 35) = (128 | t30);
  t31 = FIELD_AT(v_r6, s16, 482);
  if (t31 != 17)
    goto L_0200a03c;
  t32 = Value1((s32 (*)())GameFlag_Test, 0x300);
  if (t32 == 0)
    goto L_0200a03c;
  Call2((void (*)())Actor_SetModeById, 9, 2);
  Call3((void (*)())Func_020033e0, 9, 0x2d80000, 0x780000);
L_0200a03c:
  t35 = FIELD_AT((s32)gAdventure, s16, 482);
  if (t35 != 18)
    goto L_0200a05c;
  t36 = Value1((s32 (*)())GameFlag_Test, 0x300);
  if (t36 != 0)
    goto L_0200a05c;
  Func_02001d68();
L_0200a05c:
  t38 = FIELD_AT((s32)gAdventure, s16, 482);
  v_r5 = (s32)gAdventure;
  if (t38 == 10)
    goto L_0200a06e;
  goto L_0200a6e6;
L_0200a06e:
  t39 = Value1((s32 (*)())GameFlag_Test, 0x300);
  if (t39 == 0)
    goto L_0200a084;
  Call3((void (*)())Func_020033e0, 9, 0, 0);
L_0200a084:
  t41 = Value1((s32 (*)())GameFlag_Test, 0x207);
  if (t41 == 0)
    goto L_0200a09e;
  t42 = Value1((s32 (*)())Actor_GetById, 9);
  FIELD_AT(t42, u8, 85) = 4;
L_0200a09e:
  t43 = Value1((s32 (*)())GameFlag_Test, 0x200);
  if (t43 == 0)
    goto L_0200a0bc;
  Call6((void (*)())Func_02003328, 54, 87, 44, 86, 1, 1);
L_0200a0bc:
  t45 = Value1((s32 (*)())GameFlag_Test, 0x201);
  if (t45 == 0)
    goto L_0200a0dc;
  Call6((void (*)())Func_02003328, 54, 87, 46, 86, 1, 1);
L_0200a0dc:
  t47 = Value1((s32 (*)())GameFlag_Test, 0x202);
  if (t47 == 0)
    goto L_0200a0fc;
  Call6((void (*)())Func_02003328, 54, 87, 43, 87, 1, 1);
L_0200a0fc:
  t49 = Value1((s32 (*)())GameFlag_Test, 0x203);
  if (t49 == 0)
    goto L_0200a11c;
  Call6((void (*)())Func_02003328, 54, 87, 45, 87, 1, 1);
L_0200a11c:
  t51 = Value1((s32 (*)())GameFlag_Test, 0x204);
  if (t51 == 0)
    goto L_0200a13a;
  Call6((void (*)())Func_02003328, 54, 87, 47, 87, 1, 1);
L_0200a13a:
  t53 = Value1((s32 (*)())GameFlag_Test, 0x205);
  if (t53 == 0)
    goto L_0200a15a;
  Call6((void (*)())Func_02003328, 54, 87, 44, 88, 1, 1);
L_0200a15a:
  t55 = Value1((s32 (*)())GameFlag_Test, 0x206);
  if (t55 == 0)
    goto L_0200a17a;
  Call6((void (*)())Func_02003328, 54, 87, 46, 88, 1, 1);
L_0200a17a:
  t57 = FIELD_AT(v_r5, s32, 532);
  t58 = Value1((s32 (*)())Actor_GetById, t57);
  t59 = FIELD_AT(t58, u8, 35);
  FIELD_AT(t58, u8, 35) = (32 | t59);
  goto L_0200a6e6;
L_0200a1b4:
  if (v_r1 == 0x131)
    goto L_0200a1bc;
  goto L_0200a2ce;
L_0200a1bc:
  t60 = Value1((s32 (*)())Actor_GetById, 14);
  FIELD_AT(t60, u8, 85) = 2;
  FIELD_AT(t60, s32, 20) = 0xfff20000;
  FIELD_AT(t60, s32, 12) = 0xfff20000;
  t61 = FIELD_AT(t60, u8, 35);
  FIELD_AT(t60, u8, 35) = (128 | t61);
  t62 = FIELD_AT(v_r6, s32, 532);
  t63 = Value1((s32 (*)())Actor_GetById, t62);
  t64 = FIELD_AT(t63, u8, 35);
  FIELD_AT(t63, u8, 35) = (64 | t64);
  t65 = Value1((s32 (*)())Actor_GetById, 9);
  FIELD_AT(t65, u8, 98) = 1;
  t66 = Value1((s32 (*)())GameFlag_Test, 0xa82);
  v_r7 = 2;
  v_r5 = 1;
  if (t66 == 0)
    goto L_0200a218;
  Call3((void (*)())Func_020033e0, 8, 0x380000, 0x1a80000);
L_0200a218:
  t68 = Value1((s32 (*)())GameFlag_Test, 0xa83);
  if (t68 == 0)
    goto L_0200a254;
  Call3((void (*)())Func_020033e0, 10, 0x2180000, 0x1680000);
  Call6((void (*)())Func_02003328, 51, 20, 31, 20, v_r5, v_r7);
  Call4((void (*)())Func_02003380, 0x1f00000, 0x1500000, 0, 0);
L_0200a254:
  t72 = Value1((s32 (*)())GameFlag_Test, 0xa84);
  if (t72 == 0)
    goto L_0200a270;
  Call3((void (*)())Func_020033e0, 13, 0x2e80000, 0x1c80000);
L_0200a270:
  t74 = Value1((s32 (*)())GameFlag_Test, 0xa85);
  if (t74 == 0)
    goto L_0200a2c6;
  t75 = Value1((s32 (*)())GameFlag_Test, 0x109);
  if (t75 != 0)
    goto L_0200a2b8;
  t76 = Value1((s32 (*)())GameFlag_Test, 0xa84);
  if (t76 != 0)
    goto L_0200a2b8;
  t77 = Value1((s32 (*)())Actor_GetById, 12);
  t78 = FIELD_AT(t77, s16, 10);
  t79 = Value1((s32 (*)())Actor_GetById, 12);
  t80 = FIELD_AT(t79, s16, 18);
  v_r5 = (t78 << 16);
  Call3((void (*)())Func_020033e0, 13, v_r5, (t80 << 16));
L_0200a2b8:
  Call3((void (*)())Func_020033e0, 12, 0x2d80000, 0x1d80000);
L_0200a2c6:
  Call1((void (*)())Func_02003490, (s32)gData_020036ea);
  goto L_0200a6e6;
L_0200a2ce:
  if (v_r1 == 0x132)
    goto L_0200a2d6;
  goto L_0200a52c;
L_0200a2d6:
  t84 = Value1((s32 (*)())GameFlag_Test, 0xa86);
  if (t84 == 0)
    goto L_0200a2f2;
  Call3((void (*)())Func_020033e0, 9, 0x1080000, 0x1680000);
L_0200a2f2:
  Call1((void (*)())Func_02003490, (s32)gData_020036f6);
  t87 = FIELD_AT(v_r6, u16, 482);
  t88 = FIELD_AT(v_r6, s16, 482);
  v_r3 = t87;
  v_r2 = t88;
  if (v_r2 == 1)
    goto L_0200a30a;
  goto L_0200a4a2;
L_0200a30a:
  Func_02002a74(33, 0x280, 0x380, 18, 68);
  t90 = Value1((s32 (*)())GameFlag_Test, 0x304);
  v_r5 = t90;
  if (v_r5 != 0)
    goto L_0200a35e;
  t91 = FIELD_AT(v_r6, s32, 532);
  t92 = Value1((s32 (*)())Actor_GetById, t91);
  t93 = FIELD_AT(t92, u8, 35);
  FIELD_AT(t92, u8, 35) = (64 | t93);
  t94 = Value1((s32 (*)())Actor_GetById, 8);
  FIELD_AT(t94, u8, 85) = v_r5;
  t95 = Value1((s32 (*)())Actor_GetById, 8);
  t96 = FIELD_AT(t95, u8, 35);
  FIELD_AT(t95, u8, 35) = (128 | t96);
  goto L_0200a3a8;
L_0200a35e:
  Call3((void (*)())Func_020033e0, 8, 0, 0);
  Call1((void (*)())GameFlag_SetBit, 0x200);
  Call1((void (*)())GameFlag_SetBit, 0x201);
  Call1((void (*)())GameFlag_SetBit, 0x202);
  Call1((void (*)())GameFlag_SetBit, 0x203);
  Call1((void (*)())GameFlag_SetBit, v_r7);
  Call1((void (*)())GameFlag_SetBit, 0x205);
  Call1((void (*)())GameFlag_SetBit, 0x206);
L_0200a3a8:
  t105 = Value1((s32 (*)())GameFlag_Test, 0x207);
  if (t105 == 0)
    goto L_0200a3c2;
  t106 = Value1((s32 (*)())Actor_GetById, 8);
  FIELD_AT(t106, u8, 85) = 4;
L_0200a3c2:
  t107 = Value1((s32 (*)())GameFlag_Test, 0x200);
  if (t107 == 0)
    goto L_0200a3e0;
  Call6((void (*)())Func_02003328, 17, 71, 8, 69, 1, 1);
L_0200a3e0:
  t109 = Value1((s32 (*)())GameFlag_Test, 0x205);
  if (t109 == 0)
    goto L_0200a400;
  Call6((void (*)())Func_02003328, 17, 71, 6, 71, 1, 1);
L_0200a400:
  t111 = Value1((s32 (*)())GameFlag_Test, 0x201);
  if (t111 == 0)
    goto L_0200a420;
  Call6((void (*)())Func_02003328, 17, 71, 5, 75, 1, 1);
L_0200a420:
  t113 = Value1((s32 (*)())GameFlag_Test, 0x202);
  if (t113 == 0)
    goto L_0200a440;
  Call6((void (*)())Func_02003328, 17, 71, 8, 75, 1, 1);
L_0200a440:
  t115 = Value1((s32 (*)())GameFlag_Test, 0x203);
  if (t115 == 0)
    goto L_0200a460;
  Call6((void (*)())Func_02003328, 17, 71, 11, 75, 1, 1);
L_0200a460:
  t117 = Value1((s32 (*)())GameFlag_Test, 0x204);
  if (t117 == 0)
    goto L_0200a47e;
  Call6((void (*)())Func_02003328, 17, 71, 8, 73, 1, 1);
L_0200a47e:
  t119 = Value1((s32 (*)())GameFlag_Test, 0x206);
  if (t119 != 0)
    goto L_0200a48e;
  goto L_0200a6e6;
L_0200a48e:
  Call6((void (*)())Func_02003328, 17, 71, 10, 71, 1, 1);
  goto L_0200a6e6;
L_0200a4a2:
  if (v_r2 != 11)
    goto L_0200a4c0;
  Func_02002a74(33, 0x280, 0x380, 18, 68);
  v_r1 = 0;
  v_r0 = 22;
  goto L_0200a4ec;
L_0200a4c0:
  if (v_r2 != 12)
    goto L_0200a4f4;
  Func_02002a74(35, 0x280, 0x380, 18, 68);
  t123 = Value1((s32 (*)())GameFlag_Test, 0x109);
  if (t123 == 0)
    goto L_0200a4e6;
  goto L_0200a6e6;
L_0200a4e6:
  v_r1 = -0x24;
  v_r0 = 36;
L_0200a4ec:
  Call3((void (*)())Func_02003190, v_r0, v_r1, v_r7);
  goto L_0200a6e6;
L_0200a4f4:
  if ((u32)((v_r3 - 4) << 16) <= (u32)0x10000)
    goto L_0200a502;
  goto L_0200a6e6;
L_0200a502:
  Func_02002a74(35, 0x280, 0x380, 18, 68);
  goto L_0200a6e6;
L_0200a52c:
  if (v_r1 == 0x133)
    goto L_0200a534;
  goto L_0200a6e6;
L_0200a534:
  t126 = FIELD_AT(v_r2, s32, 32);
  t127 = FIELD_AT(v_r6, s16, 482);
  v_r5 = t126;
  v_r3 = (t127 - 1);
  if ((u32)v_r3 <= (u32)11)
    goto L_0200a548;
  goto L_0200a6e6;
L_0200a548:
  switch (v_r3) {
  case 0:
    goto L_0200a606;
  case 1:
    goto L_0200a606;
  case 2:
    goto L_0200a63a;
  case 3:
    goto L_0200a63a;
  case 4:
    goto L_0200a686;
  case 5:
    goto L_0200a686;
  case 6:
    goto L_0200a580;
  case 7:
    goto L_0200a6e6;
  case 8:
    goto L_0200a6e6;
  case 9:
    goto L_0200a6e6;
  case 10:
    goto L_0200a6e6;
  case 11:
    goto L_0200a596;
  }
L_0200a580:
  t128 = Value1((s32 (*)())GameFlag_Test, 0x304);
  if (t128 != 0)
    goto L_0200a596;
  Call3((void (*)())Func_020033e0, 11, 0, 0);
L_0200a596:
  t130 = FIELD_AT((s32)gAdventure, s32, 532);
  t131 = Value1((s32 (*)())Actor_GetById, t130);
  t132 = FIELD_AT(t131, u8, 35);
  FIELD_AT(t131, u8, 35) = (64 | t132);
  Call2((void (*)())Actor_SetModeById, 11, 2);
  t134 = Value1((s32 (*)())Actor_GetById, 11);
  FIELD_AT(t134, u8, 85) = 0;
  t135 = Value1((s32 (*)())Actor_GetById, 11);
  t136 = FIELD_AT(t135, u8, 35);
  FIELD_AT(t135, u8, 35) = (128 | t136);
  Call2((void (*)())Actor_SetModeById, 11, 2);
  t138 = Value1((s32 (*)())GameFlag_Test, 0x304);
  v_r6 = (s32)gAdventure;
  if (t138 != 0)
    goto L_0200a5fa;
  t139 = FIELD_AT(v_r6, s16, 482);
  if (t139 != 12)
    goto L_0200a5fa;
  Func_02001df4();
  goto L_0200a6e6;
L_0200a5fa:
  t141 = Value1((s32 (*)())Actor_GetById, 11);
  FIELD_AT(t141, s32, 12) = 0xfff00000;
  goto L_0200a6e6;
L_0200a606:
  t142 = Value1((s32 (*)())GameFlag_Test, 0x301);
  if (t142 == 0)
    goto L_0200a61e;
  v_r1 = 0x780000;
  v_r0 = 8;
  v_r2 = 240;
  goto L_0200a650;
L_0200a61e:
  Call6((void (*)())Func_02003328, 18, 1, 11, 4, 1, 2);
  v_r0 = 0xb80000;
  v_r1 = 176;
  goto L_0200a67a;
L_0200a63a:
  t144 = Value1((s32 (*)())GameFlag_Test, 0x302);
  if (t144 == 0)
    goto L_0200a660;
  v_r1 = 0x2280000;
  v_r0 = 9;
  v_r2 = 240;
L_0200a650:
  Call3((void (*)())Func_020033e0, v_r0, v_r1, (v_r2 << 15));
  Call1((void (*)())GameFlag_SetBit, 0x200);
  goto L_0200a6e6;
L_0200a660:
  Call6((void (*)())Func_02003328, 18, 1, 26, 4, 1, 2);
  v_r0 = 0x1a80000;
  v_r1 = 176;
L_0200a67a:
  Call4((void (*)())Func_02003380, v_r0, (v_r1 << 15), 0, 2);
  goto L_0200a6e6;
L_0200a686:
  t149 = Value1((s32 (*)())GameFlag_Test, 0x303);
  if (t149 == 0)
    goto L_0200a6ac;
  Call3((void (*)())Func_020033e0, 10, 0x3180000, 0x780000);
  Call1((void (*)())GameFlag_SetBit, 0x200);
  goto L_0200a6d0;
L_0200a6ac:
  Call6((void (*)())Func_02003328, 18, 1, 53, 4, 1, 2);
  Call4((void (*)())Func_02003380, 0x3580000, 0x580000, 0, 2);
L_0200a6d0:
  t154 = FIELD_AT(v_r5, s32, 236);
  FIELD_AT(v_r5, s32, 236) = (t154 + 0x80000);
  t155 = FIELD_AT(v_r5, s32, 244);
  FIELD_AT(v_r5, s32, 244) = (t155 + 0x80000);
L_0200a6e6:
  return 0;
}
