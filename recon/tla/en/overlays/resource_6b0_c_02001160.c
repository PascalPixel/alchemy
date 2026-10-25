// Draft of overlay 6b0's Func_02001160, translated from its listing: differ
// 97 of 243. Left: its shape: 240 instructions against the listing's 243.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 FieldEvent_WaitFrames();
s32 Func_02003328();
s32 Func_02003378();
s32 Func_02003380();
s32 Func_02003398();
s32 Func_020033a0();
s32 Func_020033e0();
s32 Func_02003488();
s32 Func_020034a0();
s32 Func_020034b8();
s32 GameFlag_SetBit();
s32 GameFlag_Test();
s32 ObjectMotion_SetActionVariant();
s32 Object_GetById();
extern u8 gPartyState[];

void Func_02001160(void) {
  s32 v_r7, v_r8, v_r5, v_r2, t0, t1, t5, t6, t7, t10;
  s32 t15, t32, t33, t34, t35, t36, t40;

  t0 = FIELD_AT(0x3000000, s32, 108);
  t1 = *(s32 *)(t0 + 0xcbc);
  Func_02003398();
  Call1((void (*)())Func_02003488, 0);
  Func_020034a0();
  t5 = FIELD_AT(t1, s32, 8);
  t6 = FIELD_AT(t1, s32, 16);
  t7 = *(s16 *)((s32)gPartyState + 0x1e0);
  v_r8 = (t6 >> 19);
  v_r7 = (t5 >> 19);
  if (t7 != 0x131)
    goto L_020091c6;
  Call4((void (*)())Func_02003378, 2, 0x480000, 0x1b80000, 232);
  if (v_r7 == 8)
    goto L_020091c6;
  Call4((void (*)())Func_02003378, 2, 0x480000, 0x1a80000, 232);
L_020091c6:
  v_r5 = ((s32)gPartyState + 0x1e0);
  t10 = *(s16 *)v_r5;
  if (t10 != 0x132)
    goto L_02009218;
  Call4((void (*)())Func_02003378, 2, 0x1180000, 0x1580000, 0);
  Call4((void (*)())Func_02003378, 2, 0x1180000, 0x1780000, 232);
  Call4((void (*)())Func_02003378, 2, 0x1280000, 0x1680000, 0);
  Call4((void (*)())Func_02003378, 2, 0x2080000, 0x2780000, 232);
L_02009218:
  t15 = *(s16 *)v_r5;
  v_r2 = t15;
  if (v_r2 != 0x130)
    goto L_02009256;
  if (v_r7 != 37)
    goto L_0200923c;
  if (v_r8 != 31)
    goto L_0200923c;
  Call1((void (*)())GameFlag_SetBit, 0xa80);
  Call2((void (*)())ObjectMotion_SetActionVariant, 8, 2);
L_0200923c:
  if (v_r7 == 55)
    goto L_02009242;
  goto L_02009356;
L_02009242:
  if (v_r8 == 95)
    goto L_0200924a;
  goto L_02009356;
L_0200924a:
  Call1((void (*)())GameFlag_SetBit, 0xa81);
  goto L_02009356;
L_02009256:
  if (v_r2 != 0x131)
    goto L_02009356;
  if (v_r7 != 7)
    goto L_02009270;
  if (v_r8 != 53)
    goto L_02009270;
  Call1((void (*)())GameFlag_SetBit, 0xa82);
L_02009270:
  if (v_r7 != 67)
    goto L_020092f2;
  if (v_r8 != 45)
    goto L_020092f2;
  Call6((void (*)())Func_02003328, 50, 22, 33, 22, 1, 1);
  Call3((void (*)())Func_020033e0, 14, 0, 0);
  Call1((void (*)())Func_020034b8, 0x248);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call1((void (*)())Func_020034b8, 188);
  Call6((void (*)())Func_02003328, 50, 20, 31, 20, 1, 2);
  Call1((void (*)())FieldEvent_WaitFrames, 5);
  Call6((void (*)())Func_02003328, 51, 20, 31, 20, 1, 2);
  Call1((void (*)())FieldEvent_WaitFrames, 5);
  Call4((void (*)())Func_02003380, 0x1f00000, 0x1500000, 0, 0);
  Call1((void (*)())GameFlag_SetBit, 0xa83);
L_020092f2:
  if (v_r7 != 93)
    goto L_02009306;
  if (v_r8 != 57)
    goto L_02009306;
  Call1((void (*)())GameFlag_SetBit, 0xa84);
L_02009306:
  if (v_r7 != 91)
    goto L_02009356;
  if (v_r8 != 59)
    goto L_02009356;
  t32 = Value1((s32 (*)())GameFlag_Test, 0xa84);
  if (t32 != 0)
    goto L_0200934c;
  t33 = Value1((s32 (*)())Object_GetById, 12);
  t34 = FIELD_AT(t33, s16, 10);
  t35 = Value1((s32 (*)())Object_GetById, 12);
  t36 = FIELD_AT(t35, s16, 18);
  v_r5 = (t34 << 16);
  Call3((void (*)())Func_020033e0, 13, v_r5, (t36 << 16));
  Call3((void (*)())Func_020033e0, 12, 0x2d80000, 0x1d80000);
L_0200934c:
  Call1((void (*)())GameFlag_SetBit, 0xa85);
L_02009356:
  t40 = *(s16 *)((s32)gPartyState + 0x1e0);
  if (t40 != 0x132)
    goto L_0200937c;
  if (v_r7 != 33)
    goto L_0200937c;
  if (v_r8 != 45)
    goto L_0200937c;
  Call1((void (*)())GameFlag_SetBit, 0xa86);
L_0200937c:
  Func_020033a0();
  return;
}
