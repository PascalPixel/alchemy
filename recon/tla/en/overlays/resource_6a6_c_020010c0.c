// Draft of overlay 6a6's Func_020010c0, translated from its listing: differ
// 31 of 121. Left: its shape: 120 instructions against the listing's 121.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_02001080();
s32 Func_020010a0();
s32 Func_02001348();
s32 Func_02001350();
s32 GameFlag_Test();
s32 ObjectMotion_EnableActionAndSetCallback();
s32 Object_GetById();
s32 Object_SetWideSprite();
extern u8 gPartyState[];

s32 Func_020010c0(s32 p0) {
  s32 v_r0, v_r1, v_r5, t0, t1, t3, t5, t9, t12, t19;
  s32 t20, t21, t22, t23, t24;

  t0 = *(s16 *)((s32)gPartyState + 0x1e0);
  v_r1 = 0;
  v_r0 = p0;
  if (t0 != 0x10a)
    goto L_0200911a;
  t1 = Value1((s32 (*)())GameFlag_Test, 0x300);
  if (t1 != 0)
    goto L_020090e8;
  Call2((void (*)())Object_SetWideSprite, 64, 0);
L_020090e8:
  t3 = Value1((s32 (*)())GameFlag_Test, 0x301);
  if (t3 != 0)
    goto L_020090fe;
  Call2((void (*)())Object_SetWideSprite, 65, 1);
L_020090fe:
  t5 = Value1((s32 (*)())GameFlag_Test, 0x302);
  if (t5 != 0)
    goto L_02009114;
  Call2((void (*)())Object_SetWideSprite, 66, 1);
L_02009114:
  Func_02001080();
  goto L_0200911e;
L_0200911a:
  Call2((void (*)())Func_020010a0, v_r0, v_r1);
L_0200911e:
  t9 = Value1((s32 (*)())GameFlag_Test, 0xa5f);
  if (t9 == 0)
    goto L_020091dc;
  Call3((void (*)())Func_02001348, 20, 0, 0);
  Call4((void (*)())Func_02001350, 14, 0xf80000, 0x1e00000, 0);
  t12 = Value1((s32 (*)())GameFlag_Test, 0x90a);
  if (t12 != 0)
    goto L_02009174;
  Call3((void (*)())Func_02001348, 17, 0, 0);
  Call3((void (*)())Func_02001348, 16, 0, 0);
  Call3((void (*)())Func_02001348, 18, 0, 0);
  goto L_020091dc;
L_02009174:
  Call4((void (*)())Func_02001350, 16, 0x1cb0000, 0xfe0000, 0xe000);
  Call4((void (*)())Func_02001350, 18, 0x1ba0000, 0xcb0000, 0x8000);
  Call4((void (*)())Func_02001350, 17, 0x1ea0000, 0xbc0000, 0x2000);
  t19 = Value1((s32 (*)())Object_GetById, 17);
  t20 = Value1((s32 (*)())Object_GetById, 17);
  t21 = FIELD_AT(t20, s16, 10);
  v_r5 = (t19 + 100);
  *(u16 *)v_r5 = t21;
  t22 = Value1((s32 (*)())Object_GetById, 17);
  t23 = Value1((s32 (*)())Object_GetById, 17);
  t24 = FIELD_AT(t23, s16, 18);
  v_r5 = (t22 + 102);
  *(u16 *)v_r5 = t24;
  Call2((void (*)())ObjectMotion_EnableActionAndSetCallback, 17, 2);
L_020091dc:
  return 0;
}
