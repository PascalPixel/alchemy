// Draft of overlay 6b1's Func_02000588, translated from its listing: differ
// 57 of 157. Left: its shape: 152 instructions against the listing's 157.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_02000388();
s32 Func_02000c74();
s32 Func_02000c7c();
s32 Func_02000c8c();
s32 Func_02000d44();
s32 Func_02000d64();
s32 GameFlag_Test();
s32 ObjectDispatch_SetSingleChildFlags();
s32 ObjectMotion_ArmCallback();
s32 Object_GetById();
s32 Object_SetModeById();
s32 Random16Far();
s32 WaitFrames();
extern u8 Func_020004b4[];
extern u8 gData_02000eec[];
extern u8 gPartyState[];

void Func_02000588(void) {
  s32 v_r6, v_r5, v_r7, v_r0, t0, t1, t2, t4, t5, t6;
  s32 t7, t8, t9, t11, t12, t13, t15, t19, t20, t21;
  s32 t23, t24, t27, t31, t32, t33, t38, t39, t43;

  v_r6 = ((s32)gPartyState + 0x214);
  t0 = *(s32 *)v_r6;
  t1 = Value1((s32 (*)())Object_GetById, t0);
  t2 = FIELD_AT(t1, s32, 16);
  v_r7 = 0x1300000;
  v_r5 = t1;
  if ((t2 >> 20) > 9)
    goto L_020085a8;
  v_r7 = -v_r7;
L_020085a8:
  Call1((void (*)())Func_02000d44, 0);
  t4 = Value1((s32 (*)())GameFlag_Test, 0x200);
  if (t4 != 0)
    goto L_020085bc;
  goto L_020086f8;
L_020085bc:
  t5 = *(u8 *)(v_r5 + 34);
  t6 = FIELD_AT(v_r5, s32, 8);
  t7 = FIELD_AT(v_r5, s32, 16);
  t8 = Value3((s32 (*)())Func_02000c8c, t5, t6, t7);
  if (t8 == 77)
    goto L_020085d0;
  goto L_020086f8;
L_020085d0:
  t9 = *(s32 *)v_r6;
  Call2((void (*)())Object_SetModeById, t9, 1);
  t11 = FIELD_AT(v_r5, s32, 8);
  t12 = FIELD_AT(v_r5, s32, 16);
  t13 = FIELD_AT(v_r5, s32, 12);
  Call4((void (*)())Func_02000c74, v_r5, ((t11 & 0xfff00000) + 0x80000), t13,
        ((t12 & 0xfff00000) + 0x80000));
  t15 = *(s32 *)v_r6;
  v_r0 = t15;
  if (v_r7 <= 0)
    goto L_02008604;
  Call3((void (*)())ObjectMotion_ArmCallback, v_r0, 0xc000, 0);
  goto L_0200860e;
L_02008604:
  Call3((void (*)())ObjectMotion_ArmCallback, v_r0, 0x4000, 0);
L_0200860e:
  Call1((void (*)())WaitFrames, 10);
  v_r6 = 0;
L_02008616:
  t19 = *(s8 *)((s32)gData_02000eec + v_r6);
  t20 = Value1((s32 (*)())Object_GetById, t19);
  *(u8 *)(t20 + 85) = 0;
  t21 = FIELD_AT(t20, s32, 12);
  FIELD_AT(t20, s32, 12) = (t21 + 0x8000);
  FIELD_AT(t20, s32, 52) = 0x1000;
  FIELD_AT(t20, s32, 48) = 0x20000;
  Call2((void (*)())ObjectDispatch_SetSingleChildFlags, t20, 1);
  t23 = FIELD_AT(t20, s32, 8);
  t24 = FIELD_AT(t20, s32, 16);
  Call4((void (*)())Func_02000c74, t20, t23, 0xfff80000, t24);
  FIELD_AT(t20, s32, 108) = (s32)Func_020004b4;
  Call1((void (*)())Func_02000d64, 152);
  t27 = Value0((s32 (*)())Random16Far);
  Call1((void (*)())WaitFrames, (((u32)(t27 << 2) >> 16) + 3));
  v_r6 = (v_r6 + 1);
  if (v_r6 <= 6)
    goto L_02008616;
  Call1((void (*)())WaitFrames, 40);
  Call1((void (*)())Func_02000d64, 198);
  v_r6 = 9;
L_0200867c:
  t31 = Value1((s32 (*)())Object_GetById, v_r6);
  t32 = FIELD_AT(t31, s32, 16);
  t33 = FIELD_AT(t31, s32, 8);
  Call4((void (*)())Func_02000c74, t31, t33, 0xfff80000, (t32 - v_r7));
  v_r6 = (v_r6 + 1);
  if (v_r6 <= 14)
    goto L_0200867c;
  Call1((void (*)())Object_GetById, 9);
  Func_02000c7c();
  Call1((void (*)())WaitFrames, 40);
  v_r6 = 9;
L_020086a8:
  t38 = Value1((s32 (*)())Object_GetById, v_r6);
  t39 = FIELD_AT(t38, s32, 16);
  FIELD_AT(t38, s32, 16) = ((t39 & 0xfff00000) + 0x80000);
  *(u8 *)(t38 + 85) = 3;
  FIELD_AT(t38, s32, 108) = 0;
  v_r6 = (v_r6 + 1);
  if (v_r6 <= 14)
    goto L_020086a8;
  Call1((void (*)())WaitFrames, 3);
  Call1((void (*)())Func_02000d64, 188);
  Call1((void (*)())WaitFrames, 10);
  t43 = Value1((s32 (*)())GameFlag_Test, 0xa9f);
  if (t43 != 0)
    goto L_020086f8;
  if (v_r7 <= 0)
    goto L_020086f8;
  Func_02000388();
L_020086f8:
  return;
}
