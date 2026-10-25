// Draft of overlay 6ac's Func_02000e84, translated from its listing: differ
// 128 of 237. Left: its shape: 227 instructions against the listing's 237.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_02000058();
s32 Func_02000934();
s32 Func_02002118();
s32 Func_02003150();
s32 Func_02003170();
s32 Func_02003178();
s32 Func_020031b8();
s32 Func_020031f8();
s32 Func_02003270();
s32 Func_020032b0();
s32 GameFlag_Test();
s32 Object_GetById();
s32 Object_SetPartAttribute();
s32 Scheduler_AddOrUpdateCallback();
extern u8 Func_020000c0[];
extern u8 Func_020003c4[];
extern u8 gPartyState[];

s32 Func_02000e84(void) {
  s32 v_r5, v_r6, t0, t1, t4, t9, t14, t19, t24, t27;
  s32 t36, t38, t40, t42, t44, t45;

  t0 = *(s16 *)((s32)gPartyState + 0x1e0);
  t1 = *(s16 *)((s32)gPartyState + 0x1e2);
  v_r6 = t1;
  v_r5 = t0;
  if (v_r5 == 0x128)
    goto L_02008ebc;
  if (v_r5 == 0x120)
    goto L_02008ebc;
  if (v_r5 == 0x121)
    goto L_02008ebc;
  if (v_r5 == 0x122)
    goto L_02008ebc;
  if (v_r5 != 0x123)
    goto L_02008ee2;
L_02008ebc:
  Call1((void (*)())Func_020032b0, 1);
  Call7((void (*)())Func_02003270, 1, 0x10000, 0x40000, 0x20000, 0x10000,
        0x20000, 0x40000);
L_02008ee2:
  if (v_r5 != 0x128)
    goto L_02008fa6;
  if (v_r6 == 2)
    goto L_02008f22;
  if (v_r6 > 2)
    goto L_02008ef6;
  if (v_r6 == 1)
    goto L_02008f00;
  goto L_02008f86;
L_02008ef6:
  if (v_r6 == 3)
    goto L_02008f44;
  if (v_r6 == 4)
    goto L_02008f66;
  goto L_02008f86;
L_02008f00:
  t4 = Value1((s32 (*)())Object_GetById, 8);
  Call2((void (*)())Object_SetPartAttribute, t4, 1);
  Call2((void (*)())Func_02000058, 9, 4);
  Call2((void (*)())Func_02003150, 4, 0);
  Call2((void (*)())Func_02003150, 5, 0);
  goto L_02008f86;
L_02008f22:
  t9 = Value1((s32 (*)())Object_GetById, 8);
  Call2((void (*)())Object_SetPartAttribute, t9, 3);
  Call2((void (*)())Func_02000058, 9, 5);
  Call2((void (*)())Func_02003150, 6, 0);
  Call2((void (*)())Func_02003150, 7, 0);
  goto L_02008f86;
L_02008f44:
  t14 = Value1((s32 (*)())Object_GetById, 8);
  Call2((void (*)())Object_SetPartAttribute, t14, 0);
  Call2((void (*)())Func_02000058, 9, 2);
  Call2((void (*)())Func_02003150, 0, 0);
  Call2((void (*)())Func_02003150, 1, 0);
  goto L_02008f86;
L_02008f66:
  t19 = Value1((s32 (*)())Object_GetById, 8);
  Call2((void (*)())Object_SetPartAttribute, t19, 4);
  Call2((void (*)())Func_02000058, 9, 3);
  Call2((void (*)())Func_02003150, 2, 0);
  Call2((void (*)())Func_02003150, 3, 0);
L_02008f86:
  t24 = Value1((s32 (*)())Object_GetById, 10);
  *(u8 *)(t24 + 85) = 0;
  FIELD_AT(t24, s32, 16) = 0x680000;
  Call2((void (*)())Scheduler_AddOrUpdateCallback, (s32)Func_020000c0, 0x480);
  goto L_02009088;
L_02008fa6:
  if (v_r5 != 0x11f)
    goto L_02009088;
  if ((u32)(v_r6 - 1) <= (u32)1)
    goto L_02008fc2;
  if (v_r6 == 8)
    goto L_02008fc2;
  if (v_r6 == 10)
    goto L_02008fc2;
  if (v_r6 == 12)
    goto L_02008fc2;
  if (v_r6 != 14)
    goto L_02009088;
L_02008fc2:
  Func_02002118();
  t27 = Value1((s32 (*)())GameFlag_Test, 0xa31);
  if (t27 == 0)
    goto L_02008ff8;
  Call6((void (*)())Func_02003170, 30, 12, 4, 3, 12, 12);
  Call6((void (*)())Func_02003178, 64, 64, 4, 3, 3, 12);
L_02008ff8:
  Call1((void (*)())Func_02000934, v_r6);
  Call2((void (*)())Scheduler_AddOrUpdateCallback, (s32)Func_020003c4, 0x480);
  Call2((void (*)())Func_02000058, 12, 4);
  Call2((void (*)())Func_02000058, 13, 5);
  Call2((void (*)())Func_02000058, 14, 2);
  Call2((void (*)())Func_02000058, 15, 3);
  t36 = Value1((s32 (*)())GameFlag_Test, 0xa32);
  if (t36 != 0)
    goto L_02009040;
  Call3((void (*)())Func_020031f8, 12, 0, 0);
L_02009040:
  t38 = Value1((s32 (*)())GameFlag_Test, 0xa33);
  if (t38 != 0)
    goto L_02009058;
  Call3((void (*)())Func_020031f8, 13, 0, 0);
L_02009058:
  t40 = Value1((s32 (*)())GameFlag_Test, 0xa34);
  if (t40 != 0)
    goto L_02009070;
  Call3((void (*)())Func_020031f8, 14, 0, 0);
L_02009070:
  t42 = Value1((s32 (*)())GameFlag_Test, 0xa35);
  if (t42 != 0)
    goto L_02009088;
  Call3((void (*)())Func_020031f8, 15, 0, 0);
L_02009088:
  t44 = Value1((s32 (*)())GameFlag_Test, 0xa30);
  if (t44 == 0)
    goto L_020090b0;
  t45 = *(s16 *)((s32)gPartyState + 0x1fa);
  Call2((void (*)())Func_020031b8, (t45 + 19), 0x5000000);
L_020090b0:
  return 0;
}
