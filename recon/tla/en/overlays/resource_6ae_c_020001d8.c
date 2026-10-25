// Draft of overlay 6ae's Func_020001d8, translated from its listing: differ
// 102 of 124. Left: its shape: 120 instructions against the listing's 124.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Animation_ApplyChildValues();
s32 Animation_SetStateFlags();
s32 Func_02004ef0();
s32 Func_0200567c();
s32 GameFlag_Test();
s32 ObjectDispatch_SetSingleChildFlags();
s32 ObjectMotion_SetActionVariant();
s32 Object_GetById();
s32 Scheduler_AddOrUpdateCallback();
s32 WaitFrames();
extern u8 Data_0200eb60[];
extern u8 Func_020004a4[];
extern u8 gData_02005aa4[];
extern u8 gData_02005ab4[];
extern u8 gData_02005ae0[];
extern u8 gData_02005b0c[];
extern u8 gPartyState[];

void Func_020001d8(void) {
  s32 v_r5, t1, t2, t3, t4, t5, t7, t12, t15, t17;
  s32 t20;

  Call4((void (*)())Func_02004ef0, (s32)gData_02005aa4, (s32)gData_02005ab4,
        (s32)gData_02005ae0, (s32)gData_02005b0c);
  t1 = FIELD_AT((s32)gPartyState, s32, 532);
  t2 = Value1((s32 (*)())Object_GetById, t1);
  t3 = FIELD_AT(t2, u8, 35);
  FIELD_AT(t2, u8, 35) = (64 | t3);
  v_r5 = 10;
L_02008206:
  t4 = Value1((s32 (*)())Object_GetById, v_r5);
  t5 = FIELD_AT(t4, u8, 35);
  FIELD_AT(t4, u8, 35) = (128 | t5);
  Call2((void (*)())Animation_ApplyChildValues, t4, 5);
  v_r5 = (v_r5 + 1);
  if (v_r5 <= 16)
    goto L_02008206;
  t7 = Value1((s32 (*)())GameFlag_Test, 0xa3c);
  if (t7 == 0)
    goto L_020082ec;
  Call3((void (*)())Func_0200567c, 8, 0x2000000, 0x1080000);
  Call3((void (*)())Func_0200567c, 9, 0x2000000, 0x10f0000);
  Call1((void (*)())WaitFrames, 1);
  Call2((void (*)())ObjectMotion_SetActionVariant, 8, 1);
  t12 = Value1((s32 (*)())Object_GetById, 8);
  FIELD_AT(t12, u8, 85) = 4;
  FIELD_AT(t12, s32, 12) = 0x700000;
  Call2((void (*)())Animation_ApplyChildValues, t12, 5);
  Call2((void (*)())ObjectDispatch_SetSingleChildFlags, t12, 0);
  t15 = FIELD_AT(t12, u8, 35);
  FIELD_AT(t12, u8, 35) = (t15 | 32);
  Call2((void (*)())ObjectMotion_SetActionVariant, 9, 2);
  t17 = Value1((s32 (*)())Object_GetById, 9);
  FIELD_AT(t17, u8, 85) = 4;
  FIELD_AT(t17, s32, 24) = 0x14ccc;
  FIELD_AT(t17, s32, 28) = 0x14ccc;
  FIELD_AT(t17, s32, 12) = 0x700000;
  Call2((void (*)())Animation_ApplyChildValues, t17, 5);
  Call2((void (*)())ObjectDispatch_SetSingleChildFlags, t17, 0);
  t20 = FIELD_AT(t17, u8, 35);
  FIELD_AT(t17, u8, 35) = ((t20 | 32) | 2);
  Call2((void (*)())Animation_SetStateFlags, t17, 1);
  *(s32 *)(s32)Data_0200eb60 = 0;
  Call2((void (*)())Scheduler_AddOrUpdateCallback, (s32)Func_020004a4, 0x480);
L_020082ec:
  return;
}
