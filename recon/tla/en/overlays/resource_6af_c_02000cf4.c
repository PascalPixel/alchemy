// Draft of overlay 6af's Func_02000cf4, translated from its listing: differ
// 90 of 241. Left: instruction order and register choice.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_020003fc();
s32 Func_02000fec();
s32 Func_0200105c();
s32 Func_02001064();
s32 GameFlag_SetBit();
s32 GameFlag_Test();
s32 Object_GetById();
s32 Scheduler_AddOrUpdateCallback();
extern u8 Func_02000ba0[];
extern u8 gData_0200117c[];
extern u8 gData_020011cc[];
extern u8 gPartyState[];

s32 Func_02000cf4(void) {
  s32 v_r5, t0, t2, t6, t8, t10, t12, t15, t17, t19;
  s32 t21, t23, t25, t27, t30, t32, t34, t36, t38, t39;
  s32 t44, t45, t46;

  t0 = FIELD_AT(0x3000000, s32, 108);
  FIELD_AT(t0, s32, 428) = 0x204;
  Func_0200105c();
  t2 = FIELD_AT((s32)gPartyState, s16, 480);
  if (t2 != 0x12a)
    goto L_02008d8c;
  Call1((void (*)())GameFlag_SetBit, 0x300);
  Call1((void (*)())GameFlag_SetBit, 0x301);
  Call1((void (*)())GameFlag_SetBit, 0x302);
  t6 = Value1((s32 (*)())GameFlag_Test, 0x300);
  if (t6 == 0)
    goto L_02008d54;
  Call3((void (*)())Func_02000fec, 8, 0x1080000, 0x1b80000);
L_02008d54:
  t8 = Value1((s32 (*)())GameFlag_Test, 0x301);
  if (t8 == 0)
    goto L_02008d70;
  Call3((void (*)())Func_02000fec, 9, 0x1480000, 0x1b80000);
L_02008d70:
  t10 = Value1((s32 (*)())GameFlag_Test, 0x302);
  if (t10 == 0)
    goto L_02008d8c;
  Call3((void (*)())Func_02000fec, 10, 0x1b80000, 0x1580000);
L_02008d8c:
  t12 = FIELD_AT((s32)gPartyState, s16, 480);
  if (t12 != 0x12b)
    goto L_02008e38;
  Call2((void (*)())Scheduler_AddOrUpdateCallback, (s32)Func_02000ba0, 0x480);
  Call3((void (*)())Func_02001064, 0, 8, 9);
  t15 = Value1((s32 (*)())GameFlag_Test, 0x308);
  if (t15 == 0)
    goto L_02008dcc;
  Call3((void (*)())Func_02000fec, 10, 0x1080000, 0xb80000);
L_02008dcc:
  t17 = Value1((s32 (*)())GameFlag_Test, 0x303);
  if (t17 == 0)
    goto L_02008de2;
  Call2((void (*)())Func_020003fc, (s32)gData_0200117c, 0);
L_02008de2:
  t19 = Value1((s32 (*)())GameFlag_Test, 0x304);
  if (t19 == 0)
    goto L_02008df6;
  Call2((void (*)())Func_020003fc, (s32)gData_0200117c, 1);
L_02008df6:
  t21 = Value1((s32 (*)())GameFlag_Test, 0x305);
  if (t21 == 0)
    goto L_02008e0c;
  Call2((void (*)())Func_020003fc, (s32)gData_0200117c, 2);
L_02008e0c:
  t23 = Value1((s32 (*)())GameFlag_Test, 0x306);
  if (t23 == 0)
    goto L_02008e22;
  Call2((void (*)())Func_020003fc, (s32)gData_0200117c, 3);
L_02008e22:
  t25 = Value1((s32 (*)())GameFlag_Test, 0x307);
  if (t25 == 0)
    goto L_02008e38;
  Call2((void (*)())Func_020003fc, (s32)gData_0200117c, 4);
L_02008e38:
  t27 = FIELD_AT((s32)gPartyState, s16, 480);
  if (t27 != 0x12c)
    goto L_02008eba;
  Call2((void (*)())Scheduler_AddOrUpdateCallback, (s32)Func_02000ba0, 0x480);
  Call3((void (*)())Func_02001064, 0, 8, 9);
  t30 = Value1((s32 (*)())GameFlag_Test, 0x309);
  if (t30 == 0)
    goto L_02008e7a;
  Call3((void (*)())Func_02000fec, 10, 0x2980000, 0xf80000);
L_02008e7a:
  t32 = Value1((s32 (*)())GameFlag_Test, 0x30a);
  if (t32 == 0)
    goto L_02008e90;
  Call2((void (*)())Func_020003fc, (s32)gData_020011cc, 0);
L_02008e90:
  t34 = Value1((s32 (*)())GameFlag_Test, 0x30b);
  if (t34 == 0)
    goto L_02008ea6;
  Call2((void (*)())Func_020003fc, (s32)gData_020011cc, 1);
L_02008ea6:
  t36 = Value1((s32 (*)())GameFlag_Test, 0x30c);
  if (t36 == 0)
    goto L_02008eba;
  Call2((void (*)())Func_020003fc, (s32)gData_020011cc, 2);
L_02008eba:
  t38 = FIELD_AT((s32)gPartyState, s16, 480);
  if (t38 != 0x12d)
    goto L_02008f22;
  t39 = Value1((s32 (*)())GameFlag_Test, 0x30d);
  if (t39 == 0)
    goto L_02008ee8;
  Call3((void (*)())Func_02000fec, 8, 0x2b80000, 0x2580000);
L_02008ee8:
  Func_02001064(0, 17, 18);
  Func_02001064(1, 19, 20);
  Call3((void (*)())Func_02001064, 2, 21, 22);
  v_r5 = 0;
L_02008f08:
  t44 = Value1((s32 (*)())Object_GetById, (v_r5 + 9));
  t45 = FIELD_AT(t44, s32, 80);
  t46 = FIELD_AT(t45, u8, 37);
  FIELD_AT(t45, u8, 37) = (t46 | 12);
  v_r5 = (v_r5 + 1);
  if (v_r5 <= 7)
    goto L_02008f08;
L_02008f22:
  return 0;
}
