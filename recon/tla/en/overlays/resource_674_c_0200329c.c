// Draft of overlay 674's Func_0200329c, translated from its listing.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_02003e64();
s32 Func_02003fa4();
s32 GameFlag_ClearBit();
s32 Func_02003410();
s32 PaletteQueue_Add();
s32 GameFlag_Test();
s32 Func_02003edc();
s32 Actor_GetById();
s32 Actor_SetSingleChildFlags();
s32 Task_Add();
s32 GameFlag_SetBit();
s32 Func_02003ee4();
s32 wait();
s32 Func_02002830();
s32 Func_020029c4();
s32 Event_StartScreenTransition();
s32 Event_WaitTransitionFrames();
s32 Func_020034d8();
s32 Func_02003e6c();
extern u8 gMainRoutineWork[];
extern u8 Func_02001c4c[];

void Func_0200329c(s32 p0) {
  s32 v_r0, v_r2, v_r3, v_r5;

  v_r5 = FIELD_AT(0x3000000, s32, 32);
  Call1((void (*)())Func_02003e64, p0);
  Call1((void (*)())Func_02003fa4, 0);
  v_r2 = FIELD_AT(v_r5, u8, 23);
  v_r3 = -13 & v_r2;
  v_r3 = v_r3 | 4;
  FIELD_AT(v_r5, u8, 23) = v_r3;
  v_r3 = (s32)gMainRoutineWork + 0x1e2;
  v_r3 = FIELD_AT(v_r3, s16, 0);
  if (v_r3 != 2)
    goto L_ja_0200b1f4;
  Call1((void (*)())GameFlag_ClearBit, 0x12f);
L_ja_0200b1f4:
  ;
  Func_02003410();
  PaletteQueue_Add(3, 12, 5, 4);
  Call4((void (*)())PaletteQueue_Add, 4, 12, 5, 4);
  v_r0 = GameFlag_Test(0xaa5);
  if (v_r0 == 0)
    goto L_ja_0200b252;
  v_r0 = GameFlag_Test(0x109);
  v_r5 = v_r0;
  if (v_r5 != 0)
    goto L_ja_0200b252;
  Call3((void (*)())Func_02003edc, 10, 0x1180000, 0x1d40000);
  v_r0 = Actor_GetById(10);
  v_r0 = v_r0 + 85;
  FIELD_AT(v_r0, u8, 0) = v_r5;
  Call3((void (*)())Func_02003edc, 9, 0x1280000, 0x1c40000);
L_ja_0200b252:
  ;
  v_r0 = Actor_GetById(9);
  Actor_SetSingleChildFlags();
  v_r0 = Actor_GetById(11);
  Actor_SetSingleChildFlags();
  v_r0 = Actor_GetById(12);
  Actor_SetSingleChildFlags();
  Call2((void (*)())Task_Add, (s32)Func_02001c4c, 0x480);
  v_r3 = (s32)gMainRoutineWork + 0x1e2;
  v_r3 = FIELD_AT(v_r3, u16, 0);
  v_r3 = v_r3 - 4;
  v_r3 = v_r3 << 16;
  if ((u32)v_r3 > (u32)0x10000)
    goto L_ja_0200b2d0;
  Call1((void (*)())GameFlag_SetBit, 0x217);
  v_r0 = GameFlag_Test(0x109);
  if (v_r0 != 0)
    goto L_ja_0200b2d0;
  Call4((void (*)())Func_02003ee4, 9, 0x980000, 0x3000000, 0x4000);
  Call3((void (*)())Func_02003edc, 10, 0xb00000, 0x3090000);
  Call1((void (*)())wait, 1);
L_ja_0200b2d0:
  ;
  v_r0 = GameFlag_Test(0x109);
  if (v_r0 != 0)
    goto L_ja_0200b30a;
  v_r3 = (s32)gMainRoutineWork + 0x1e2;
  v_r3 = FIELD_AT(v_r3, s16, 0);
  if (v_r3 != 3)
    goto L_ja_0200b2f2;
  Func_02002830();
  goto L_ja_0200b316;
L_ja_0200b2f2:
  ;
  if (v_r3 != 4)
    goto L_ja_0200b2fc;
  Func_020029c4();
  goto L_ja_0200b316;
L_ja_0200b2fc:
  ;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_020034d8();
  goto L_ja_0200b316;
L_ja_0200b30a:
  ;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_020034d8();
L_ja_0200b316:
  ;
  Func_02003e6c();
}
