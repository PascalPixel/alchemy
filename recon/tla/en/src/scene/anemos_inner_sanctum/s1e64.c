#include "types.h"
#include "call.h"
#include "main_routine_work.h"
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
extern char SceneId_Ov6B0Room1;
extern char SceneId_Ov6B0Room2;
extern char SceneId_Ov6B0Room3;
extern char SceneId_Ov6B0Room4;
extern char SceneId_Ov6B0Room5;

s32 Func_02001e64(void) {
  s32 v_r1, v_r6, v_r7, v_r2, v_r5, v_r3, v_r0, t0, t1, t3;
  s32 t6, t9, t12, t15, t27;
  s32 t30, t35, t36, t38, t42;
  s32 t59;
  s32 t60, t61, t64;
  s32 t78, t80;
  s32 t91, t92, t93, t94, t95, t96, t106;
  s32 t126;
  s32 t132, t136;
  s32 v_two;

  v_r2 = 0x3000000;
  v_r7 = 0x204;
  v_r6 = (s32)&gMainRoutineWork;
  FIELD_AT(FIELD_AT(v_r2, s32, 108), s32, 428) = v_r7;
  v_r1 = gMainRoutineWork.scene;
  if (v_r1 == (s32)&SceneId_Ov6B0Room1) {
    Call4((void (*)())Func_02000038, (s32)gData_0200384c, (s32)gData_0200385c,
          (s32)gData_02003888, (s32)gData_020038b4);
    t3 = Value1((s32 (*)())GameFlag_Test, SPLIT_CONSTANT(0xa87));
    if (t3 != 0) {
      Call3((void (*)())Func_02000a78, 0x208, 48, 0);
      Call6((void (*)())Func_02003328, 85, 34, 85, 8, 9, 8);
    }
    t6 = Value1((s32 (*)())GameFlag_Test, SPLIT_CONSTANT(0xa88));
    if (t6 != 0) {
      Call3((void (*)())Func_02000a78, 0x228, 48, 3);
      Call6((void (*)())Func_02003328, 101, 34, 101, 8, 9, 8);
    }
    t9 = Value1((s32 (*)())GameFlag_Test, SPLIT_CONSTANT(0xa89));
    if (t9 != 0) {
      Call3((void (*)())Func_02000a78, 0x1f8, 40, 10);
      Call6((void (*)())Func_02003328, 85, 46, 85, 20, 9, 8);
    }
    t12 = Value1((s32 (*)())GameFlag_Test, SPLIT_CONSTANT(0xa8a));
    if (t12 != 0) {
      Call3((void (*)())Func_02000a78, 0x238, 40, 2);
      Call6((void (*)())Func_02003328, 101, 46, 101, 20, 9, 8);
    }
    t15 = Value1((s32 (*)())GameFlag_Test, SPLIT_CONSTANT(0xa8b));
    if (t15 == 0)
      goto L_0200a6e6;
    Call6((void (*)())Func_02003328, 54, 7, 33, 7, 1, 2);
    Call4((void (*)())Func_02003380, 0x2180000, 0x880000, 0, 0);
    goto L_0200a6e6;
  }
  if (v_r1 != (s32)&SceneId_Ov6B0Room2)
    goto L_0200a1b4;
  Call2((void (*)())Actor_SetSpritePriority, 8, 1);
  if ((Value1((s32 (*)())GameFlag_Test, 0xa80)) != 0) {
    Call3((void (*)())Func_020033e0, 8, 0x1280000, 0xf80000);
    Call2((void (*)())Actor_SetSpritePriority, 8, 2);
  }
  if ((Value1((s32 (*)())GameFlag_Test, SPLIT_CONSTANT(0xa81))) != 0) {
    Call3((void (*)())Func_020033e0, 10, 0x1b80000, 0x2f80000);
  }
  Call1((void (*)())Func_02003490, (s32)gData_020036e4);
  FIELD_AT((Value1((s32 (*)())Actor_GetById, (FIELD_AT(v_r6, s32, 532)))), u8,
           35) |= 64;
  FIELD_AT((Value1((s32 (*)())Actor_GetById, 9)), u8, 85) = 0;
  FIELD_AT((Value1((s32 (*)())Actor_GetById, 9)), u8, 35) |= 128;
  if ((FIELD_AT(v_r6, s16, 482)) != 17)
    goto L_0200a03c;
  if ((Value1((s32 (*)())GameFlag_Test, 0x300)) == 0)
    goto L_0200a03c;
  Call2((void (*)())Actor_SetModeById, 9, 2);
  Call3((void (*)())Func_020033e0, 9, 0x2d80000, 0x780000);
L_0200a03c:
  if (gMainRoutineWork.entrance == 18 && GameFlag_Test(0x300) == 0)
    Func_02001d68();
  {
    struct MainRoutineWork *work = &gMainRoutineWork;
    if (work->entrance != 10)
      goto L_0200a6e6;
    if ((Value1((s32 (*)())GameFlag_Test, 0x300)) != 0) {
      Call3((void (*)())Func_020033e0, 9, 0, 0);
    }
    if ((Value1((s32 (*)())GameFlag_Test, SPLIT_CONSTANT(0x207))) != 0) {
      t42 = Value1((s32 (*)())Actor_GetById, 9);
      FIELD_AT(t42, u8, 85) = 4;
    }
    if ((Value1((s32 (*)())GameFlag_Test, 0x200)) != 0) {
      Call6((void (*)())Func_02003328, 54, 87, 44, 86, 1, 1);
    }
    if ((Value1((s32 (*)())GameFlag_Test, SPLIT_CONSTANT(0x201))) != 0) {
      Call6((void (*)())Func_02003328, 54, 87, 46, 86, 1, 1);
    }
    if ((Value1((s32 (*)())GameFlag_Test, SPLIT_CONSTANT(0x202))) != 0) {
      Call6((void (*)())Func_02003328, 54, 87, 43, 87, 1, 1);
    }
    if ((Value1((s32 (*)())GameFlag_Test, SPLIT_CONSTANT(0x203))) != 0) {
      Call6((void (*)())Func_02003328, 54, 87, 45, 87, 1, 1);
    }
    if ((Value1((s32 (*)())GameFlag_Test, 0x204)) != 0) {
      Call6((void (*)())Func_02003328, 54, 87, 47, 87, 1, 1);
    }
    if ((Value1((s32 (*)())GameFlag_Test, SPLIT_CONSTANT(0x205))) != 0) {
      Call6((void (*)())Func_02003328, 54, 87, 44, 88, 1, 1);
    }
    if ((Value1((s32 (*)())GameFlag_Test, SPLIT_CONSTANT(0x206))) != 0) {
      Call6((void (*)())Func_02003328, 54, 87, 46, 88, 1, 1);
    }
    FIELD_AT((Value1((s32 (*)())Actor_GetById, (work->currentOwner))), u8,
             35) |= 32;
  }
  goto L_0200a6e6;
L_0200a1b4:
  if (v_r1 != (s32)&SceneId_Ov6B0Room3)
    goto L_0200a2ce;
  t60 = Value1((s32 (*)())Actor_GetById, 14);
  v_two = 2;
  v_r5 = 1;
  FIELD_AT(t60, u8, 85) = v_two;
  FIELD_AT(t60, s32, 20) = 0xfff20000;
  FIELD_AT(t60, s32, 12) = 0xfff20000;
  FIELD_AT(t60, u8, 35) |= 128;
  FIELD_AT((Value1((s32 (*)())Actor_GetById, (FIELD_AT(v_r6, s32, 532)))), u8,
           35) |= 64;
  FIELD_AT((Value1((s32 (*)())Actor_GetById, 9)), u8, 98) = v_r5;
  if ((Value1((s32 (*)())GameFlag_Test, SPLIT_CONSTANT(0xa82))) != 0) {
    Call3((void (*)())Func_020033e0, 8, 0x380000, 0x1a80000);
  }
  if ((Value1((s32 (*)())GameFlag_Test, SPLIT_CONSTANT(0xa83))) != 0) {
    Call3((void (*)())Func_020033e0, 10, 0x2180000, 0x1680000);
    Call6((void (*)())Func_02003328, 51, 20, 31, 20, v_r5, v_two);
    Call4((void (*)())Func_02003380, 0x1f00000, 0x1500000, 0, 0);
  }
  if ((Value1((s32 (*)())GameFlag_Test, SPLIT_CONSTANT(0xa84))) != 0) {
    Call3((void (*)())Func_020033e0, 13, 0x2e80000, 0x1c80000);
  }
  if ((Value1((s32 (*)())GameFlag_Test, SPLIT_CONSTANT(0xa85))) == 0)
    goto L_0200a2c6;
  if ((Value1((s32 (*)())GameFlag_Test, 0x109)) != 0)
    goto L_0200a2b8;
  if ((Value1((s32 (*)())GameFlag_Test, SPLIT_CONSTANT(0xa84))) != 0)
    goto L_0200a2b8;
  t78 = FIELD_AT((Value1((s32 (*)())Actor_GetById, 12)), s16, 10);
  t80 = FIELD_AT((Value1((s32 (*)())Actor_GetById, 12)), s16, 18);
  v_r5 = (t78 << 16);
  Call3((void (*)())Func_020033e0, 13, v_r5, (t80 << 16));
L_0200a2b8:
  Call3((void (*)())Func_020033e0, 12, 0x2d80000, 0x1d80000);
L_0200a2c6:
  Call1((void (*)())Func_02003490, (s32)gData_020036ea);
  goto L_0200a6e6;
L_0200a2ce:
  if (v_r1 != (s32)&SceneId_Ov6B0Room4)
    goto L_0200a52c;
  if ((Value1((s32 (*)())GameFlag_Test, 0xa86)) != 0) {
    Call3((void (*)())Func_020033e0, 9, 0x1080000, 0x1680000);
  }
  Call1((void (*)())Func_02003490, (s32)gData_020036f6);
  v_r3 = (FIELD_AT(v_r6, u16, 482));
  v_r2 = (FIELD_AT(v_r6, s16, 482));
  if (v_r2 != 1)
    goto L_0200a4a2;
  Func_02002a74(33, 0x280, 0x380, 18, 68);
  v_r5 = (Value1((s32 (*)())GameFlag_Test, 0x304));
  if (v_r5 == 0) {
    t91 = FIELD_AT(v_r6, s32, 532);
    t92 = Value1((s32 (*)())Actor_GetById, t91);
    FIELD_AT(t92, u8, 35) |= 64;
    t94 = Value1((s32 (*)())Actor_GetById, 8);
    FIELD_AT(t94, u8, 85) = v_r5;
    t95 = Value1((s32 (*)())Actor_GetById, 8);
    FIELD_AT(t95, u8, 35) |= 128;
    goto L_0200a3a8;
  }
  Call3((void (*)())Func_020033e0, 8, 0, 0);
  Call1((void (*)())GameFlag_SetBit, 0x200);
  Call1((void (*)())GameFlag_SetBit, SPLIT_CONSTANT(0x201));
  Call1((void (*)())GameFlag_SetBit, SPLIT_CONSTANT(0x202));
  Call1((void (*)())GameFlag_SetBit, SPLIT_CONSTANT(0x203));
  Call1((void (*)())GameFlag_SetBit, v_r7);
  Call1((void (*)())GameFlag_SetBit, SPLIT_CONSTANT(0x205));
  Call1((void (*)())GameFlag_SetBit, SPLIT_CONSTANT(0x206));
L_0200a3a8:
  if ((Value1((s32 (*)())GameFlag_Test, SPLIT_CONSTANT(0x207))) != 0) {
    t106 = Value1((s32 (*)())Actor_GetById, 8);
    FIELD_AT(t106, u8, 85) = 4;
  }
  if ((Value1((s32 (*)())GameFlag_Test, 0x200)) != 0) {
    Call6((void (*)())Func_02003328, 17, 71, 8, 69, 1, 1);
  }
  if ((Value1((s32 (*)())GameFlag_Test, SPLIT_CONSTANT(0x205))) != 0) {
    Call6((void (*)())Func_02003328, 17, 71, 6, 71, 1, 1);
  }
  if ((Value1((s32 (*)())GameFlag_Test, SPLIT_CONSTANT(0x201))) != 0) {
    Call6((void (*)())Func_02003328, 17, 71, 5, 75, 1, 1);
  }
  if ((Value1((s32 (*)())GameFlag_Test, SPLIT_CONSTANT(0x202))) != 0) {
    Call6((void (*)())Func_02003328, 17, 71, 8, 75, 1, 1);
  }
  if ((Value1((s32 (*)())GameFlag_Test, SPLIT_CONSTANT(0x203))) != 0) {
    Call6((void (*)())Func_02003328, 17, 71, 11, 75, 1, 1);
  }
  if ((Value1((s32 (*)())GameFlag_Test, 0x204)) != 0) {
    Call6((void (*)())Func_02003328, 17, 71, 8, 73, 1, 1);
  }
  if ((Value1((s32 (*)())GameFlag_Test, SPLIT_CONSTANT(0x206))) == 0)
    goto L_0200a6e6;
  Call6((void (*)())Func_02003328, 17, 71, 10, 71, 1, 1);
  goto L_0200a6e6;
L_0200a4a2:
  if (v_r2 == 11) {
    Func_02002a74(33, 0x280, 0x380, 18, 68);
    Func_02003190(22, 0, v_r7);
    goto L_0200a6e6;
  }
  if (v_r2 != 12)
    goto L_0200a4f4;
  Func_02002a74(35, 0x280, 0x380, 18, 68);
  if ((Value1((s32 (*)())GameFlag_Test, 0x109)) != 0)
    goto L_0200a6e6;
  Func_02003190(36, -0x24, v_r7);
  goto L_0200a6e6;
L_0200a4f4:
  if ((u32)((v_r3 - 4) << 16) > (u32)0x10000)
    goto L_0200a6e6;
  Func_02002a74(35, 0x280, 0x380, 18, 68);
  goto L_0200a6e6;
L_0200a52c:
  if (v_r1 != (s32)&SceneId_Ov6B0Room5)
    goto L_0200a6e6;
  t126 = FIELD_AT(v_r2, s32, 32);
  v_r5 = t126;
  v_r3 = ((FIELD_AT(v_r6, s16, 482)) - 1);
  if ((u32)v_r3 > (u32)11)
    goto L_0200a6e6;
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
  if ((Value1((s32 (*)())GameFlag_Test, 0x304)) != 0)
    goto L_0200a596;
  Call3((void (*)())Func_020033e0, 11, 0, 0);
L_0200a596:
  FIELD_AT((Value1((s32 (*)())Actor_GetById, (gMainRoutineWork.currentOwner))),
           u8, 35) |= 64;
  Call2((void (*)())Actor_SetModeById, 11, 2);
  FIELD_AT((Value1((s32 (*)())Actor_GetById, 11)), u8, 85) = 0;
  FIELD_AT((Value1((s32 (*)())Actor_GetById, 11)), u8, 35) |= 128;
  Call2((void (*)())Actor_SetModeById, 11, 2);
  /* FAKEMATCH(dead): v_r6 set again to the address it already holds; without
   * it, 4 of 913 instructions differ in tla-en. */
  v_r6 = (s32)&gMainRoutineWork;
  if ((Value1((s32 (*)())GameFlag_Test, 0x304)) != 0)
    goto L_0200a5fa;
  if ((FIELD_AT(v_r6, s16, 482)) != 12)
    goto L_0200a5fa;
  Func_02001df4();
  goto L_0200a6e6;
L_0200a5fa:
  FIELD_AT((Value1((s32 (*)())Actor_GetById, 11)), s32, 12) = 0xfff00000;
  goto L_0200a6e6;
L_0200a606:
  if ((Value1((s32 (*)())GameFlag_Test, SPLIT_CONSTANT(0x301))) != 0) {
    v_r2 = 240;
    v_r0 = 8;
    v_r1 = 0x780000;
    goto L_0200a650;
  }
  Call6((void (*)())Func_02003328, 18, 1, 11, 4, 1, 2);
  v_r1 = 176;
  v_r0 = 0xb80000;
  goto L_0200a67a;
L_0200a63a:
  if ((Value1((s32 (*)())GameFlag_Test, SPLIT_CONSTANT(0x302))) == 0)
    goto L_0200a660;
  v_r2 = 240;
  v_r0 = 9;
  v_r1 = 0x2280000;
L_0200a650:
  Call3((void (*)())Func_020033e0, v_r0, v_r1, (v_r2 << 15));
  Call1((void (*)())GameFlag_SetBit, 0x200);
  goto L_0200a6e6;
L_0200a660:
  Call6((void (*)())Func_02003328, 18, 1, 26, 4, 1, 2);
  v_r1 = 176;
  v_r0 = 0x1a80000;
L_0200a67a:
  Call4((void (*)())Func_02003380, v_r0, (v_r1 << 15), 0, 2);
  goto L_0200a6e6;
L_0200a686:
  if ((Value1((s32 (*)())GameFlag_Test, SPLIT_CONSTANT(0x303))) != 0) {
    Call3((void (*)())Func_020033e0, 10, 0x3180000, 0x780000);
    Call1((void (*)())GameFlag_SetBit, 0x200);
    goto L_0200a6d0;
  }
  Call6((void (*)())Func_02003328, 18, 1, 53, 4, 1, 2);
  Call4((void (*)())Func_02003380, 0x3580000, 0x580000, 0, 2);
L_0200a6d0:
  FIELD_AT(v_r5, s32, 236) = ((FIELD_AT(v_r5, s32, 236)) + 0x80000);
  FIELD_AT(v_r5, s32, 244) = ((FIELD_AT(v_r5, s32, 244)) + 0x80000);
L_0200a6e6:
  return 0;
}
